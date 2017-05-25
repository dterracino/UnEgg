/*
 * ALZFormat.h
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "ALZFormat.h"
#include <nest/stream/InputStreamRewinder.h>
#include <nest/stream/OutputStream.h>
#include <nest/algorithm/StoreCoder.h>
#include <nest/algorithm/DeflateCoder.h>
#include <nest/encryption/ZipDecoder.h>
#include <nest/algorithm/crc32.hpp>
#include <nest/Utils.h>

namespace nest
{

ALZFormat::ALZFormat() :
    opened_(false), spanned_(false)
{
    // TODO Auto-generated constructor stub

}

ALZFormat::ALZFormat(Events* evnt) :
    Format(evnt), opened_(false), spanned_(false)
{

}

ALZFormat::~ALZFormat()
{
    // TODO Auto-generated destructor stub
    Close();
}

int ALZFormat::Open(InputStream* stream)
{
    if (opened_)
    {
        Close();
    }

    int res = istream_.Initialize(stream->GetVirtualName());
    istream_.PushStream(stream);

    if (res == Result::Success)
    {
        res = istream_.Open();

        if (res == Result::Success)
        {
            res = CheckHeaderInternal(stream, &global_);
            if (res == Result::Success)
            {
                res = global_.GetVersion() == 10 ? Result::Success : Result::Invalid;
                opened_ = res == Result::Success;
            }
        }
    }

    return res;
}

void ALZFormat::Close()
{
    istream_.Close();
    istream_.Finalize();
    opened_ = false;
    spanned_ = false;
}

int ALZFormat::CheckHeader(InputStream* stream)
{
    alz::Global global;
    return CheckHeaderInternal(stream, &global);
}

int ALZFormat::CheckHeaderInternal(InputStream* stream, alz::Global* global)
{
    int res = OnWorkStart(WorkCode::Check);

    if (res == Result::Success)
    {
        stream->Seek(0, SEEK_SET);
        int res = Result::Invalid;

        if (global)
        {
            res = global->Read(stream);

            if (res == Result::Success)
            {
                res = global->GetVersion() == 10 ? Result::Success : Result::Invalid;
            }
        }
    }

    OnWorkFinish(WorkCode::Check, res);

    return res;
}

int ALZFormat::Scan()
{
    int res = Result::NotReady;

    if (opened_)
    {
        res = OnWorkStart(WorkCode::Scan);

        if (res == Result::Success)
        {
            unsigned int signature = 0;
            istream_.Seek(4, SEEK_END);
            istream_.Read(&signature, 4);
            spanned_ = signature == alz::Magic::SPLIT;
            if (spanned_)
            {
                res = PrepareSpannedStreams();
            }

            if (res == Result::Success)
            {
                istream_.Seek(0, SEEK_SET);

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

int ALZFormat::ScanFiles()
{
    istream_.Seek(8, SEEK_SET);
    InputStreamRewinder rewinder(istream_);
    int res = Result::Invalid;

    for (;;)
    {
        alz::File* file = new alz::File();
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
        else if (res == Result::NeedMoreStream)
        {
            res = Result::Success;
        }
        else
        {
            delete file;
            break;
        }
    }

    if (res == Result::Success)
    {
        rewinder.Clear();
    }

    return res;
}

int ALZFormat::ScanFooters()
{
    int res = Result::ReadError;
    unsigned char endheader[16];
    istream_.Seek(16, SEEK_END);

    size_t read = istream_.Read(endheader, 16);
    if (read == 16)
    {
        if ((*(unsigned int*) (endheader)) != alz::Magic::END)
        {
            res = Result::Invalid;
        }
        else
        {
            crc_ = *(unsigned int*) (endheader + 8);
            unsigned int size = *(unsigned int*) (endheader + 4);
            if (size)
            {
                istream_.Seek(size + 16, SEEK_END);

                unsigned int signature = 0;
                read = istream_.Read(&signature, 4);
                if (read == 4 && signature == alz::Magic::COMMENT)
                {
                    size -= 4;
                    unsigned int index = 0;
                    unsigned short len = 0;
                    char comment[65536] = { 0, };

                    int64 remained = size;
                    res = Result::Success;
                    while (remained > 6)
                    {
                        read = istream_.Read(&index, 4) + istream_.Read(&len, 2);
                        if (read == 6)
                        {
                            remained -= 6;
                            read = istream_.Read(comment, len);
                            if (read == len)
                            {
                                remained -= len;
                                comment[len] = 0;
                                if (index == 0xFFFFFFFF)
                                {
                                    utf8::euckr2utf8(comment, &comment_);
                                }
                                else if (files_.size() > index)
                                {
                                    std::string buf;
                                    utf8::euckr2utf8(comment, &buf);
                                    files_[index]->SetComment(buf.c_str());
                                }
                                else
                                {
                                    res = Result::Invalid;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                res = Result::Success;
            }
        }
    }

    return res;
}

int ALZFormat::GenerateVolumeName(size_t index, char* buffer, size_t size)
{
    std::string name = istream_.GetVirtualName();
    size_t pos = name.rfind('.');
    if (pos != std::string::npos)
    {
        name = name.substr(0, pos);
    }
    char extension[4];
    if (index == 0)
    {
        strcpy(extension, "alz");
    }
    else
    {
        --index;
        extension[0] = (index / 100) + 'a';
        extension[1] = (index % 100) / 10 + '0';
        extension[2] = (index % 100) % 10 + '0';
        extension[3] = 0;
    }
    snprintf(buffer, size, "%s.%s", name.c_str(), extension);

    return Result::Success;
}

int ALZFormat::PrepareSpannedStreams()
{
    std::vector<SpannedVolume> volumes;
    InputStreamRewinder sr(istream_);

    size_t qc = 0;
    size_t ai = 0;

    int res = Result::Success;
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
            alz::Global gh;
            res = CheckHeaderInternal(stream, &gh);

            if (res == Result::Success)
            {
                if (gh.GetDiskID() == ai)
                {
                    stream->Seek(stream->Size() - 4, SEEK_SET);
                    unsigned int signature = 0;
                    size_t read = stream->Read(&signature, 4);
                    if (4 == read)
                    {
                        SpannedVolume vol;
                        vol.offset = ai ? 8 : 0;
                        vol.stream = stream;
                        vol.size = stream->Size() - vol.offset;
                        switch (signature)
                        {
                        case alz::Magic::FILE_END:
                            res = Result::End;
                            volumes.push_back(vol);
                            break;
                        case alz::Magic::SPLIT:
                            vol.size -= 16;
                            volumes.push_back(vol);
                            qc = 0;
                            ++ai;
                            break;
                        }
                    }
                }
            }
            stream->Close();
        }
    }

    if ((res == Result::Success || res == Result::End) && volumes.size())
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

int ALZFormat::QueryExtractStream(size_t index, OutputStream** stream)
{
    return OnQueryOutputStream(index, stream, files_[index]->GetFilename());
}

int ALZFormat::Extract()
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

bool ALZFormat::IsTargetDir(alz::File* file)
{
    return file->GetAttribute() & 16;
}

int ALZFormat::PreprocessDecrypt(size_t index, alz::File* header)
{
    char password[alz::MAX_PASSWORD_LEN];
    memset(password, 0, sizeof(password));

    decoder_ = new ZipDecoder();

    int res = Result::Success;
    if (decoder_)
    {
        res = Result::Success;
        size_t qc = password_.empty() ? 1 : 0;
        while (qc < 4)
        {
            if (!qc)
            {
                strncpy(password, password_.c_str(), alz::MAX_PASSWORD_LEN);
            }
            else
            {
                res = OnQueryPassword(index, password, alz::MAX_PASSWORD_LEN, qc);
            }

            if (res == Result::Success)
            {
                res = decoder_->PreprocessDecrypt(password, header->GetVerifyData());
                if (res == Result::Success)
                {
                    unsigned char vd = header->GetCRC() >> 24;
                    res = decoder_->VerifyNow(&vd) ? Result::Success : OnError(index, Result::DecryptError);
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

int ALZFormat::PostprocessDecrypt(size_t index, alz::File* header)
{
    return Result::Success;
}

void ALZFormat::GenerateCoder(int method, Coder** coder, size_t* dataHeaderSize)
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
    default:
        *coder = NULL;
        break;
    }
}

int ALZFormat::ExtractIndividualFile(size_t index, OutputStream* stream)
{
    int res = Result::Success;
    alz::File* file = files_[index];

    bool isDir = IsTargetDir(file);

    if (isDir)
    {
        OnProgress(index, 0, PROGRESS_MAX);
        OnProgress(index, PROGRESS_MAX, PROGRESS_MAX);
        res = Result::Success;
    }
    else
    {
        bool isEncrypted = file->IsEncrypted();

        if (isEncrypted)
        {
            res = PreprocessDecrypt(index, file);
        }

        if (res == Result::Success)
        {
            if (file->GetBitFlags())
            {
                BlockExtractInfo info;
                memset(&info, 0, sizeof(info));
                istream_.Seek(file->GetDataOffset(), SEEK_SET);

                GenerateCoder(file->GetCompressMethod(), &info.coder, &info.dataHeaderSize);

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
                        uint64 size = file->GetPackedSize() - info.dataHeaderSize;
                        uint64 uncSize = file->GetUnpackedSize();
                        uint64 wholeSize = size;
                        unsigned char input[alz::EXTRACT_CHUNK] = { 0, };
                        unsigned char output[alz::EXTRACT_CHUNK] = { 0, };

                        while (size)
                        {
                            size_t readsize = static_cast<size_t> (alz::EXTRACT_CHUNK > size ? size : alz::EXTRACT_CHUNK);
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
                                    size_t ucs = alz::EXTRACT_CHUNK;
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
                            if (crc == file->GetCRC())
                            {
                                res = Result::Success;
                            }
                            else
                            {
                                res = Result::InvalidCRC;
                            }

                            if (isEncrypted)
                            {
                                decoder_->PostprocessDecrypt(file);
                                if (!decoder_->VerifyNow(NULL))
                                {
                                    res = OnError(index, Result::DecryptError);
                                }
                            }
                        }
                    }

                    delete info.coder;
                    info.coder = NULL;

                    if (res != Result::Success)
                    {
                        res = OnError(index, res);
                    }
                }
                else
                {
                    // todo
                    res = OnError(index, Result::UnknownAlgorithm);
                }
            }
            else
            {
                // 0 byte file
                OnProgress(index, 0, PROGRESS_MAX);
                OnProgress(index, PROGRESS_MAX, PROGRESS_MAX);
                res = Result::Success;
            }

            if (stream)
            {
                stream->Close();
            }
        }
    }

    return res;
}

InputStream* ALZFormat::GetInputStream()
{
    return &istream_;
}

Info* ALZFormat::GetInfo()
{
    return this;
}

int ALZFormat::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::IS_SOLID, 0);
    VERIFY_KEY(key::archive::IS_SPANNED, spanned_);

    if (!comment_.empty())
    {
        VERIFY_KEY(key::archive::comment::IS_ENCRYPTED, 0);
        VERIFY_KEY(key::archive::comment::IS_UTF8, 0);
        VERIFY_KEY(key::archive::comment::LOCALE, 0);
    }

    bool handle = false;
    int res = global_.AsInt32(key, &handle);

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

int64 ALZFormat::_As8Bytes(unsigned int key, bool* handled)
{
    bool handle = false;
    int64 res = global_.AsInt64(key, &handle);

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

size_t ALZFormat::_AsSize(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::FILE_COUNT, files_.size());
    VERIFY_KEY(key::archive::BLOCK_COUNT, 0);

    bool handle = false;
    size_t res = global_.AsSize(key, &handle);

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

const void* ALZFormat::_AsPtr(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::format::NAME, "alz");

    if (!comment_.empty())
    {
        VERIFY_KEY(key::archive::comment::COMMENT, comment_.c_str());
    }

    bool handle = false;
    const void * res = global_.AsPtr(key, &handle);

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

Info* ALZFormat::_AsInfo(unsigned int key, bool* handled)
{
    bool handle = false;
    Info* res = global_.AsInfo(key, &handle);

    if (handled)
    {
        *handled = handle;
    }

    assert(handled || handle);

    return res;
}

Info* ALZFormat::_AsChildInfo(unsigned int key, size_t index, bool* handled)
{
    VERIFY_KEY(key::file::OBJECT, files_[index]);

    return Info::_AsChildInfo(key, index, handled);
}

int ALZFormat::Initialize(Workspace* sender)
{
    if (event)
    {
        return event->Initialize(NULL);
    }

    return Result::Success;
}

void ALZFormat::Finalize(int res)
{
    if (event)
    {
        event->Finalize(res);
    }
}

int ALZFormat::OnWorkStart(int workCode)
{
    if (event)
    {
        return event->OnWorkStart(workCode);
    }

    return Result::Success;
}

void ALZFormat::OnWorkFinish(int workCode, int res)
{
    if (event)
    {
        event->OnWorkFinish(workCode, res);
    }
}

int ALZFormat::OnFileStart(size_t index)
{
    if (event)
    {
        return event->OnFileStart(index);
    }

    return Result::Success;
}

void ALZFormat::OnFileFinish(size_t index, int res)
{
    if (event)
    {
        event->OnFileFinish(index, res);
    }
}

int ALZFormat::OnError(size_t index, int errorCode)
{
    if (event)
    {
        return event->OnError(index, errorCode);
    }

    return errorCode;
}

int ALZFormat::OnQueryInputStream(size_t index, InputStream** stream, const char* virtualName, size_t queryCount)
{
    if (event)
    {
        return event->OnQueryInputStream(index, stream, virtualName, queryCount);
    }

    return Result::Fail;
}

int ALZFormat::OnQueryOutputStream(size_t index, OutputStream** stream, const char* virtualName)
{
    if (event)
    {
        return event->OnQueryOutputStream(index, stream, virtualName);
    }

    return Result::Success;
}

int ALZFormat::OnQueryPassword(size_t index, char* buffer, size_t size, size_t queryCount)
{
    if (event)
    {
        return event->OnQueryPassword(index, buffer, size, queryCount);
    }

    return Result::Fail;
}

int ALZFormat::OnProgress(size_t index, int current, int max)
{
    if (event)
    {
        return event->OnProgress(index, current, max);
    }

    return Result::Success;
}

}
