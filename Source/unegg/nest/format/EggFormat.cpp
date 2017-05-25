/*
 * EggFormat.cpp
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EggFormat.h"
#include <errno.h>
#include <vector>
#include <nest/format/egg/EggTypes.h>
#include <nest/format/egg/WindowsField.h>
#include <nest/format/egg/PosixField.h>
#include <nest/format/egg/CommentField.h>
#include <nest/format/egg/SplitField.h>
#include <nest/algorithm/DeflateCoder.h>
#include <nest/algorithm/StoreCoder.h>
#include <nest/algorithm/BZipCoder.h>
#include <nest/algorithm/LZMACoder.h>
#include <nest/algorithm/AZOCoder.h>
#include <nest/algorithm/crc32.hpp>
#include <nest/encryption/ZipDecoder.h>
#include <nest/encryption/AESDecoder.h>
#include <nest/stream/InputStream.h>
#include <nest/stream/OutputStream.h>
#include <nest/Utils.h>

namespace nest
{

EggFormat::EggFormat()
{
}

EggFormat::EggFormat(Events* evnt) :
    Format(evnt)
{

}

EggFormat::~EggFormat()
{
    Close();
}

int EggFormat::Open(InputStream* stream)
{
    if (opened_)
    {
        Close();
    }

    istream_.Initialize(stream->GetVirtualName());
    istream_.PushStream(stream);
    int res = istream_.Open();

    if (res == Result::Success)
    {
        res = CheckHeaderInternal(&istream_, &globalHeader_);

        if (res == Result::Success)
        {
            if (globalHeader_.GetVersion() > 0x0100)
            {
                res = OnError(SIZE_T_MAX, Result::Unsupported);
            }
        }
    }

    opened_ = res == Result::Success;

    return res;
}

void EggFormat::Close()
{
    istream_.Finalize();
    opened_ = false;
}

int EggFormat::CheckHeader(InputStream* stream)
{
    return CheckHeaderInternal(stream, NULL);
}

int EggFormat::Scan()
{
    int res = Result::NotReady;

    if (opened_)
    {
        res = OnWorkStart(WorkCode::Scan);

        if (res == Result::Success)
        {
            res = ScanHeaders();

            if (res == Result::Success)
            {
                isSpanned_ = headerExtras_.GetExtra(egg::Magic::SPLIT) != NULL;
                isSolid_ = headerExtras_.GetExtra(egg::Magic::SOLID) != NULL;

                if (isSpanned_)
                {
                    PrepareSpannedStreams();
                }

                res = ScanFiles();
                if (res == Result::Success)
                {
                    res = ScanFooters();
                }
            }
        }
        OnWorkFinish(WorkCode::Scan, res);
    }

    return res;
}

int EggFormat::Extract()
{
    int res = Result::NotReady;

    if (opened_)
    {
        if (isSolid_)
        {
            res = ExtractSolidArchive();
        }
        else
        {
            res = ExtractNormalArchive();
        }
    }

    return res;
}

int EggFormat::CheckHeaderInternal(InputStream* stream, egg::Global* header)
{
    int res = OnWorkStart(WorkCode::Check);

    if (res == Result::Success)
    {
        if (header)
        {
            res = header->Read(stream);
        }
        else
        {
            egg::Global header;
            res = header.Read(stream);
        }
    }

    OnWorkFinish(WorkCode::Check, res);

    return res;
}

int EggFormat::ScanHeaders(InputStream* stream, egg::ExtraFieldWrap* header)
{
    if (!stream)
    {
        stream = &istream_;
    }
    if (!header)
    {
        header = &headerExtras_;
    }

    stream->Seek(14, SEEK_SET);
    InputStreamRewinder rewinder(stream);

    int res = header->Read(stream);

    if (res == Result::Success)
    {
        rewinder.Clear();
    }

    return res;
}

int EggFormat::ScanFiles()
{
    InputStreamRewinder rewinder(istream_);
    int res = Result::Invalid;

    for (;;)
    {
        egg::FileWrap* file = new egg::FileWrap(isSolid_);
        res = file->Read(&istream_);

        if (res == Result::Success)
        {
            files_.push_back(file);
            res = OnFileStart(files_.size() - 1);
            OnFileFinish(files_.size() - 1, Result::Success);

            if (res != Result::Success)
            {
                break;
            }
        }
        else if (res == Result::End)
        {
            delete file;
            res = Result::Success;

            break;
        }
        else
        {
            delete file;
            break;
        }
    }

    if (res == Result::Success)
    {
        if (isSolid_)
        {
            res = ReadSolidBlocks();
        }

        if (res == Result::Success)
        {
            rewinder.Clear();
        }
    }

    return res;
}

int EggFormat::ScanFooters()
{
    InputStreamRewinder rewinder(istream_);

    int res = footerExtras_.Read(&istream_);

    if (res == Result::Success)
    {
        rewinder.Clear();
    }

    return res;
}

int EggFormat::GenerateVolumeName(size_t index, char* buffer, size_t size)
{
    std::string name = istream_.GetVirtualName();
    size_t pos = name.rfind(".vol");
    if (pos != std::string::npos)
    {
        name = name.substr(0, pos);
    }
    snprintf(buffer, size, "%s.vol%zu.egg", name.c_str(), index + 1);

    return Result::Success;
}

int EggFormat::PrepareSpannedStreams(void)
{
    std::vector<SpannedVolume> volumes;
    InputStreamRewinder sr(istream_);

    int res = Result::Invalid;
    egg::SplitField* sh = (egg::SplitField*) headerExtras_.GetExtra(egg::Magic::SPLIT);

    if (sh)
    {
        size_t qc = 0;
        size_t ai = 0;

        unsigned int nextID = 0;

        res = Result::Success;
        while (res == Result::Success)
        {
            ++qc;
            InputStream* stream = NULL;
            char volumename[FILENAME_LENGTH_MAX] = { 0, };
            GenerateVolumeName(ai, volumename, FILENAME_LENGTH_MAX);
            res = OnQueryInputStream(ai, &stream, volumename, qc);

            if (res != Result::Success)
            {
                break;
            }

            if (stream)
            {
                stream->Open();
                egg::Global gh;
                res = CheckHeaderInternal(stream, &gh);

                if (res == Result::Success)
                {
                    stream->Seek(0, SEEK_SET);
                    egg::ExtraFieldWrap extra;
                    res = ScanHeaders(stream, &extra);

                    if (res == Result::Success)
                    {
                        res = Result::Invalid;
                        egg::SplitField* field = (egg::SplitField*) extra.GetExtra(egg::Magic::SPLIT);
                        if (field && ((ai == 0 && field->GetPrevFileID() == 0) || (ai && gh.GetID() == nextID)))
                        {
                            SpannedVolume vol;
                            vol.stream = stream;
                            vol.offset = ai ? stream->Tell() : 0;
                            vol.size = stream->Size() - vol.offset - CalculateDummySize(stream);
                            volumes.push_back(vol);

                            res = Result::Success;

                            nextID = field->GetNextFileID();
                            ++ai;
                            qc = 0;


                            if (!nextID)
                            {
                                break;
                            }
                        }
                    }
                }
                stream->Close();
            }
        }
    }

    if (res == Result::Success && volumes.size())
    {
        istream_.Close();
        istream_.PopAll();

        for (size_t i = 0; i < volumes.size(); ++i)
        {
            SpannedVolume& vol = volumes[i];
            istream_.PushStream(vol.stream, vol.offset, vol.size);
        }

        istream_.UpdateStreams();
        res = istream_.Open();
    }

    return res;
}

int EggFormat::ReadSolidBlocks()
{
    int res = Result::Success;

    solidBlocks_.clear();

    for (size_t i = 0; i < egg::MAX_BLOCKS; ++i)
    {
        egg::BlockWrap* bh = new egg::BlockWrap();
        res = bh->Read(&istream_);

        switch (res)
        {
        case Result::End:
            delete bh;
            return Result::Success;
        case Result::Success:
            solidBlocks_.push_back(bh);
            istream_.Seek(bh->GetBlock().GetCompressedSize(), SEEK_CUR);
            break;
        default:
            delete bh;
            return res;
        }
    }

    return res;
}

unsigned int EggFormat::CalculateDummySize(InputStream* stream)
{
    // scan dummy header
    const int DUMMY_SEARCH_SIZE = 1024;

    stream->Seek(DUMMY_SEARCH_SIZE, SEEK_END);
    char buffer[DUMMY_SEARCH_SIZE];
    stream->Read(buffer, DUMMY_SEARCH_SIZE);

    for (size_t i = DUMMY_SEARCH_SIZE - 3; i != 0; --i)
    {
        int* sig = (int*) (buffer + i);
        if (*sig == egg::Magic::DUMMY)
        {
            return static_cast<unsigned int> (100 - i);
        }
    }

    return 0;
}

InputStream* EggFormat::GetInputStream()
{
    return &istream_;
}

Info* EggFormat::GetInfo()
{
    return this;
}

int EggFormat::QueryExtractStream(size_t index, OutputStream** stream)
{
    egg::FilenameField* field = (egg::FilenameField*) files_[index]->GetExtra(egg::Magic::FILENAME);
    assert(field);

    return OnQueryOutputStream(index, stream, field->GetFileName());
}

int EggFormat::PrepareSolidBlock(egg::BlockWrap* block, BlockExtractInfo* info)
{
    int res = Result::Success;

    info->coder = NULL;
    info->dataHeaderSize = 0;
    info->crc = 0;
    info->dataRemained = block->GetBlock().GetUncompressedSize();
    CRC32Init(&info->crc);

    istream_.Seek(block->GetDataOffset(), SEEK_SET);

    GenerateCoder(block->GetBlock().GetCompressMethod(), &info->coder, &info->dataHeaderSize);

    if (info->coder)
    {
        unsigned char dataHeader[info->dataHeaderSize];
        istream_.Read(dataHeader, info->dataHeaderSize);
        res = info->coder->Initialize(dataHeader, info->dataHeaderSize);
    }
    else
    {
        res = Result::Unsupported;
    }
    info->rawDataRemained = block->GetBlock().GetCompressedSize() - info->dataHeaderSize;

    return res;
}

bool EggFormat::IsTargetDir(egg::FileWrap* file)
{
    bool res = false;
    egg::WindowsField *wfield = (egg::WindowsField*) file->GetExtra(egg::Magic::WINDOWS_FILEINFO);
    if (wfield)
    {
        res = wfield->GetAttribute() & 128;
    }
    else
    {
        egg::PosixField* pfield = (egg::PosixField*) file->GetExtra(egg::Magic::POSIX_FILEINFO);
        if (pfield)
        {
            res = pfield->GetMode() & 0x40000;
        }
    }

    return res;
}

void EggFormat::GenerateCoder(int method, Coder** coder, size_t* dataHeaderSize)
{
    *dataHeaderSize = 0;

    switch (method)
    {
    case CompressionMethod::Store:
        *coder = new StoreCoder();
        break;
    case CompressionMethod::Deflate:
        *coder = new DeflateCoder();
        break;
    case CompressionMethod::bzip:
        *coder = new BZipCoder();
        break;
    case CompressionMethod::lzma:
        *dataHeaderSize = 9;
        *coder = new LZMACoder();
        break;
    case CompressionMethod::azo:
        *coder = new AZOCoder();
        break;
    default:
        *coder = NULL;
        break;
    }
}

int EggFormat::ExtractNormalArchive()
{
    int res = Result::NotReady;

    if (opened_)
    {
        res = OnWorkStart(WorkCode::Extract);
        if (res == Result::Success)
        {
            res = istream_.Seek(0, SEEK_SET);

            if (res == Result::Success)
            {
                for (size_t i = 0; i < files_.size(); ++i)
                {
                    res = OnFileStart(i);
                    if (res == Result::Success)
                    {
                        OutputStream* stream = NULL;
                        res = QueryExtractStream(i, &stream);

                        if (res == Result::Success)
                        {
                            res = ExtractIndividualFile(i, stream);
                        }
                    }
                    OnFileFinish(i, res);

                    if (res != Result::Skip && res != Result::Success)
                    {
                        break;
                    }
                }

                istream_.Close();
            }
        }
        OnWorkFinish(WorkCode::Extract, res);
    }

    return res;
}

int EggFormat::ExtractSolidArchive()
{
    int res = OnWorkStart(WorkCode::Extract);

    if (res == Result::Success)
    {
        res = istream_.Seek(0, SEEK_SET);

        if (res == Result::Success)
        {
            BlockExtractInfo info;
            unsigned char input[egg::EXTRACT_CHUNK] = { 0, };
            unsigned char output[egg::EXTRACT_CHUNK] = { 0, };
            size_t blockIndex = 0;
            egg::BlockWrap* block = solidBlocks_[blockIndex];
            PrepareSolidBlock(block, &info);
            if (info.coder)
            {
                size_t ucs = 0;

                for (size_t i = 0; i < files_.size(); ++i)
                {
                    res = OnFileStart(i);

                    if (res == Result::Success)
                    {
                        bool skipThisFile = false;
                        egg::FileWrap* file = files_[i];
                        uint64 fileSize = file->GetFileHeader().GetLength();
                        uint64 fileRemained = fileSize;
                        OutputStream* stream = NULL;
                        res = QueryExtractStream(i, &stream);
                        skipThisFile = (res == Result::Skip) || !stream;

                        if (res == Result::Cancel)
                        {
                            break;
                        }

                        if (res == Result::Success || skipThisFile)
                        {
                            bool isDir = IsTargetDir(file);
                            if (isDir)
                            {
                                OnProgress(i, 0, PROGRESS_MAX);
                                OnProgress(i, PROGRESS_MAX, PROGRESS_MAX);
                            }
                            else
                            {
                                if (skipThisFile || res == Result::Success)
                                {
                                    res = OnProgress(i, 0, PROGRESS_MAX);

                                    if (res == Result::Success)
                                    {
                                        // Main Loop
                                        while (fileRemained)
                                        {
                                            while (res == Result::Success)
                                            {
                                                ucs = static_cast<size_t> (egg::EXTRACT_CHUNK > fileRemained ? fileRemained
                                                        : egg::EXTRACT_CHUNK);

                                                if (ucs)
                                                {
                                                    res = info.coder->Pop(output, &ucs);
                                                    ucs = static_cast<size_t> (ucs > info.dataRemained ? info.dataRemained : ucs);

                                                    if (ucs)
                                                    {
                                                        if (!skipThisFile)
                                                        {
                                                            stream->Write(output, ucs);
                                                        }
                                                        info.dataRemained -= ucs;
                                                        CRC32Update(&info.crc, output, static_cast<unsigned int> (ucs));
                                                        fileRemained -= ucs;
                                                    }
                                                }
                                                else
                                                {
                                                    res = Result::End;
                                                }
                                            }
                                            if (res == Result::End)
                                            {
                                                res = Result::Success;
                                            }

                                            if (info.rawDataRemained == 0)
                                            {
                                                CRC32Finish(&info.crc);
                                                if (info.crc == block->GetBlock().GetCRC())
                                                {
                                                    res = Result::Success;
                                                }
                                                else
                                                {
                                                    res = OnError(i, Result::InvalidCRC);
                                                }

                                                if (res == Result::Success)
                                                {
                                                    ++blockIndex;
                                                    delete info.coder;
                                                    info.coder = NULL;
                                                    if (blockIndex < solidBlocks_.size())
                                                    {
                                                        block = solidBlocks_[blockIndex];
                                                        PrepareSolidBlock(block, &info);
                                                    }
                                                    else if (fileRemained)
                                                    {
                                                        if (fileRemained)
                                                        {
                                                            res = OnError(i, Result::InsufficientBlock);
                                                        }
                                                    }
                                                }
                                            }

                                            if (fileRemained)
                                            {
                                                size_t
                                                        readSize =
                                                                static_cast<size_t> (egg::EXTRACT_CHUNK > info.rawDataRemained ? info.rawDataRemained
                                                                        : egg::EXTRACT_CHUNK);
                                                readSize = istream_.Read(input, readSize);
                                                if (readSize)
                                                {
                                                    info.coder->Push(input, readSize);
                                                    info.rawDataRemained -= readSize;
                                                }
                                                else
                                                {
                                                    res = OnError(i, Result::ReadError);
                                                }
                                            }

                                            if (res != Result::Success)
                                            {
                                                break;
                                            }
                                            res = OnProgress(i, 10000 - fileRemained * PROGRESS_MAX * 1.0f / fileSize, PROGRESS_MAX);
                                        }
                                    }

                                    if (res == Result::Success)
                                    {
                                        OnProgress(i, PROGRESS_MAX, PROGRESS_MAX);
                                    }
                                }
                                else
                                {
                                    res = OnError(i, Result::WriteError);
                                }

                                if (stream)
                                {
                                    stream->Close();
                                    delete stream;
                                    stream = NULL;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    OnFileFinish(i, res);

                    if (res != Result::Success)
                    {
                        break;
                    }
                }
            }
            else
            {
                res = Result::SolidError;
            }

            if (info.coder)
            {
                delete info.coder;
                info.coder = NULL;
            }
        }

        istream_.Close();
    }
    OnWorkFinish(WorkCode::Extract, res);

    return res;
}

int EggFormat::ExtractIndividualFile(size_t index, OutputStream* stream)
{
    int res = Result::Success;
    egg::FileWrap* file = files_[index];
    egg::EncryptField* field = (egg::EncryptField*) file->GetExtras().GetExtra(egg::Magic::ENCRYPT);

    bool isDir = IsTargetDir(file);

    if (isDir)
    {
        OnProgress(index, 0, PROGRESS_MAX);
        OnProgress(index, PROGRESS_MAX, PROGRESS_MAX);
        res = Result::Success;
    }
    else
    {
        bool isEncrypted = field != NULL;

        if (isEncrypted)
        {
            res = PreprocessDecrypt(index, field);
        }

        if (res == Result::Success)
        {
            if (stream)
            {
                if (file->GetBlockCount())
                {
                    for (size_t i = 0; i < file->GetBlockCount(); ++i)
                    {
                        egg::BlockWrap* block = file->GetBlock(i);
                        BlockExtractInfo info;
                        memset(&info, 0, sizeof(info));
                        istream_.Seek(block->GetDataOffset(), SEEK_SET);

                        GenerateCoder(block->GetBlock().GetCompressMethod(), &info.coder, &info.dataHeaderSize);

                        if (info.coder)
                        {
                            unsigned char dataHeader[info.dataHeaderSize];
                            istream_.Read(dataHeader, info.dataHeaderSize);
                            res = info.coder->Initialize(dataHeader, info.dataHeaderSize);

                            res = OnProgress(index, 0, PROGRESS_MAX);

                            if (res == Result::Success)
                            {
                                unsigned int crc = 0;
                                CRC32Init(&crc);
                                uint64 size = block->GetBlock().GetCompressedSize() - info.dataHeaderSize;
                                uint64 uncSize = block->GetBlock().GetUncompressedSize();
                                uint64 wholeSize = size;
                                unsigned char input[egg::EXTRACT_CHUNK] = { 0, };
                                unsigned char output[egg::EXTRACT_CHUNK] = { 0, };

                                while (size)
                                {
                                    size_t readsize = static_cast<size_t> (egg::EXTRACT_CHUNK > size ? size : egg::EXTRACT_CHUNK);
                                    readsize = istream_.Read(input, readsize);
                                    if (readsize)
                                    {
                                        if (isEncrypted)
                                        {
                                            decoder_->DecryptBuffer(input, readsize);
                                        }
                                        res = info.coder->Push(input, readsize);
                                        while (1)
                                        {
                                            size_t ucs = egg::EXTRACT_CHUNK;
                                            res = info.coder->Pop(output, &ucs);

                                            if (ucs)
                                            {
                                                ucs = static_cast<size_t> (ucs > uncSize ? uncSize : ucs);
                                                stream->Write(output, ucs);
                                                CRC32Update(&crc, output, static_cast<unsigned int> (ucs));
                                                uncSize -= ucs;
                                            }

                                            if (res == Result::End)
                                            {
                                                res = Result::Success;
                                                break;
                                            }
                                        }
                                        size -= readsize;
                                        res = OnProgress(index, 10000 - size * PROGRESS_MAX * 1.0f / wholeSize, PROGRESS_MAX);

                                        if (res != Result::Success)
                                        {
                                            break;
                                        }
                                    }
                                }
                                if (res == Result::Success)
                                {
                                    OnProgress(index, PROGRESS_MAX, PROGRESS_MAX);
                                    CRC32Finish(&crc);
                                    if (crc == block->GetBlock().GetCRC())
                                    {
                                        res = Result::Success;
                                    }
                                    else
                                    {
                                        res = Result::InvalidCRC;
                                    }

                                    if (isEncrypted)
                                    {
                                        decoder_->PostprocessDecrypt(field->GetAESFooter());
                                        if (!decoder_->VerifyNow(NULL))
                                        {
                                            res = OnError(index, Result::DecryptError);
                                        }
                                    }
                                }
                            }

                            delete info.coder;
                            info.coder = NULL;
                        }
                        else
                        {
                            res = OnError(index, Result::Unsupported);
                        }

                        if (res != Result::Success)
                        {
                            res = OnError(index, res);
                            if (res == Result::Success || res == Result::Skip)
                            {
                                continue;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    // 0 byte file
                    OnProgress(index, 0, PROGRESS_MAX);
                    OnProgress(index, PROGRESS_MAX, PROGRESS_MAX);
                    res = Result::Success;
                }
            }
            else
            {
                // todo
                res = OnError(index, Result::WriteError);
            }

            if (stream)
            {
                stream->Close();
            }
        }
    }

    return res;
}

int EggFormat::PreprocessDecrypt(size_t index, egg::EncryptField* header)
{
    char password[egg::MAX_PASSWORD_LEN];
    memset(password, 0, sizeof(password));

    decoder_ = NULL;
    switch (header->GetEncryptMethod())
    {
    case EncryptMethod::ZIP_COMPATIBLE:
        decoder_ = new ZipDecoder();
        break;
    case EncryptMethod::AES_128:
        decoder_ = new AESDecoder(AESDecoder::AES128);
        break;
    case EncryptMethod::AES_256:
        decoder_ = new AESDecoder(AESDecoder::AES256);
        break;
    }

    int res = Result::Success;
    if (decoder_)
    {
        res = Result::Success;
        size_t qc = password_.empty() ? 1 : 0;
        while (qc < 4)
        {
            if (!qc)
            {
                strncpy(password, password_.c_str(), egg::MAX_PASSWORD_LEN);
            }
            else
            {
                res = OnQueryPassword(index, password, egg::MAX_PASSWORD_LEN, qc);
            }

            if (res == Result::Success)
            {
                switch (header->GetEncryptMethod())
                {
                case EncryptMethod::ZIP_COMPATIBLE:
                    res = decoder_->PreprocessDecrypt(password, header->GetVerifyData());
                    if (res == Result::Success)
                    {
                        unsigned char vd = header->GetCRC() >> 24;
                        res = decoder_->VerifyNow(&vd) ? Result::Success : OnError(index, Result::DecryptError);
                    }
                    break;
                case EncryptMethod::AES_128:
                case EncryptMethod::AES_256:
                    res = decoder_->PreprocessDecrypt(password, header->GetAESHeader());
                    if (res == Result::Success)
                    {
                        res = decoder_->VerifyNow(NULL) ? Result::Success : OnError(index, Result::DecryptError);
                    }
                    break;
                }
            }
            else
            {
                break;
            }
            if (res == Result::Success)
            {
                password_ = password;
                break;
            }
            ++qc;
        }
    }
    else
    {
        res = Result::UnknownDecryptMethod;
    }

    return res;
}

int EggFormat::PostprocessDecrypt(size_t index, egg::EncryptField* header)
{
    return Result::Success;
}

int EggFormat::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::IS_SOLID, isSolid_);
    VERIFY_KEY(key::archive::IS_SPANNED, isSpanned_);

    bool handle = false;
    int res = globalHeader_.AsInt32(key, &handle);
    if (!handle)
    {
        res = headerExtras_.AsInt32(key, &handle);
    }
    if (!handle)
    {
        res = footerExtras_.AsInt32(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

int64 EggFormat::_As8Bytes(unsigned int key, bool* handled)
{
    bool handle = false;
    int64 res = globalHeader_.AsInt64(key, &handle);
    if (!handle)
    {
        res = headerExtras_.AsInt64(key, &handle);
    }
    if (!handle)
    {
        res = footerExtras_.AsInt64(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

size_t EggFormat::_AsSize(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::FILE_COUNT, files_.size());

    bool handle = false;
    size_t res = globalHeader_.AsSize(key, &handle);

    if (!handle)
    {
        res = headerExtras_.AsSize(key, &handle);
    }

    if (!handle)
    {
        res = footerExtras_.AsSize(key, &handle);
    }

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

const void* EggFormat::_AsPtr(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::format::NAME, "egg");

    bool handle = false;
    const void* res = globalHeader_.AsPtr(key, &handle);

    if (!handle)
    {
        res = headerExtras_.AsPtr(key, &handle);
    }

    if (!handle)
    {
        res = footerExtras_.AsPtr(key, &handle);
    }

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

Info* EggFormat::_AsChildInfo(unsigned int key, size_t index, bool* handled)
{
    VERIFY_KEY(key::file::OBJECT, files_[index]);
    VERIFY_KEY(key::block::OBJECT, solidBlocks_[index]);

    return Info::_AsChildInfo(key, index, handled);
}

int EggFormat::Initialize(Workspace* sender)
{
    if (event)
    {
        return event->Initialize(NULL);
    }

    return Result::Success;
}

void EggFormat::Finalize(int res)
{
    if (event)
    {
        event->Finalize(res);
    }
}

int EggFormat::OnWorkStart(int workCode)
{
    if (event)
    {
        return event->OnWorkStart(workCode);
    }

    return Result::Success;
}

void EggFormat::OnWorkFinish(int workCode, int res)
{
    if (event)
    {
        event->OnWorkFinish(workCode, res);
    }
}

int EggFormat::OnFileStart(size_t index)
{
    if (event)
    {
        return event->OnFileStart(index);
    }

    return Result::Success;
}

void EggFormat::OnFileFinish(size_t index, int res)
{
    if (event)
    {
        event->OnFileFinish(index, res);
    }
}

int EggFormat::OnError(size_t index, int errorCode)
{
    if (event)
    {
        return event->OnError(index, errorCode);
    }

    return errorCode;
}

int EggFormat::OnQueryInputStream(size_t index, InputStream** stream, const char* virtualName, size_t queryCount)
{
    if (event)
    {
        return event->OnQueryInputStream(index, stream, virtualName, queryCount);
    }

    return Result::Fail;
}

int EggFormat::OnQueryOutputStream(size_t index, OutputStream** stream, const char* virtualName)
{
    if (event)
    {
        return event->OnQueryOutputStream(index, stream, virtualName);
    }

    return Result::Success;
}

int EggFormat::OnQueryPassword(size_t index, char* buffer, size_t size, size_t queryCount)
{
    if (event)
    {
        return event->OnQueryPassword(index, buffer, size, queryCount);
    }

    return Result::Fail;
}

int EggFormat::OnProgress(size_t index, int current, int max)
{
    if (event)
    {
        return event->OnProgress(index, current, max);
    }

    return Result::Success;
}

}
