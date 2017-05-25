/*
 * File.cpp
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "File.h"
#include <nest/format/alz/ALZTypes.h>
#include <nest/Utils.h>

namespace nest
{
namespace alz
{

File::File() :
    crc_(0), compressMethod_(0), packedSize_(0), unpackedSize_(0), offset_(0), verifyData_(NULL)
{
    // TODO Auto-generated constructor stub
    memset(buffer_, 0, sizeof(buffer_));
}

File::~File()
{
    // TODO Auto-generated destructor stub
}

void File::OnCleanUp()
{
    crc_ = 0;
    compressMethod_ = 0;
    packedSize_ = 0;
    unpackedSize_ = 0;
    offset_ = 0;
    comment_.clear();
    if (verifyData_)
    {
        delete[] verifyData_;
        verifyData_ = NULL;
    }
    memset(buffer_, 0, sizeof(buffer_));
    filename_.clear();
}

int File::ReadFileSize(InputStream* stream, uint64* size, size_t byteCount)
{
    int res = Result::ReadError;

    char buffer[8];
    size_t read = stream->Read(buffer, byteCount);
    if (read == byteCount)
    {
        res = Result::Success;
        switch (byteCount)
        {
        case 1:
            *size = *(unsigned char*) (buffer);
            break;
        case 2:
            *size = *(unsigned short*) (buffer);
            break;
        case 4:
            *size = *(unsigned int*) (buffer);
            break;
        case 8:
            *size = *(uint64*) (buffer);
            break;
        default:
            res = Result::Invalid;
        }
    }

    return res;
}

int File::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = Result::ReadError;

    size_t read = stream->Read(buffer_, 4);
    if (read == 4)
    {
        switch (GetSignature())
        {
        case Magic::FILE:
        {
            read = stream->Read(buffer_ + 4, 9);
            if (read == 9)
            {
                packedSize_ = 0;
                unpackedSize_ = 0;

                if (GetBitFlags() != 0)
                {
                    read = stream->Read(&compressMethod_, 2);
                    if (read == 2)
                    {
                        read = stream->Read(&crc_, 4);
                        if (read == 4)
                        {
                            size_t len = GetBaseSize();
                            res = ReadFileSize(stream, &packedSize_, len);
                            if (res == Result::Success)
                            {
                                res = ReadFileSize(stream, &unpackedSize_, len);
                            }
                        }
                    }
                }
                else
                {
                    res = Result::Success;
                }

                if (res == Result::Success)
                {
                    size_t len = GetFilenameLength();
                    char* fnbuf = new char[len + 1];
                    read = stream->Read(fnbuf, len);
                    fnbuf[len] = 0;

                    if (read == len)
                    {
                        utf8::euckr2utf8(fnbuf, &filename_);
                        offset_ = stream->Tell();
                        if (stream->Tell() + GetPackedSize() > stream->Size())
                        {
                            res = Result::NeedMoreStream;
                        }
                        else
                        {
                            if (IsEncrypted())
                            {
                                verifyData_ = new char[12];
                                stream->Read(verifyData_, 12);
                                offset_ += 12;
                            }
                            stream->Seek(GetPackedSize(), SEEK_CUR);
                            res = Result::Success;
                        }
                        rewinder->Clear();
                    }
                }

                if (!rewinder->IsEmpty())
                {
                    filename_.clear();
                }
            }
        }
            break;
        case Magic::COMMENT:
        case Magic::END:
        case Magic::SPLIT:
        case Magic::FILE_END:
            res = Result::End;
            break;
        }
    }

    return res;
}

int File::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::attribute::ATTRIBUTES, GetAttribute());
    VERIFY_KEY(key::file::attribute::MODIFIED, GetDateTime());
    VERIFY_KEY(key::file::IS_ENCRYPTED, IsEncrypted());
    VERIFY_KEY(key::file::method::METHOD, GetCompressMethod());
    VERIFY_KEY(key::file::CRC, GetCRC());

    return Info::AsInt32(key, handled);
}

int64 File::_As8Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::size::PACKED, GetPackedSize());
    VERIFY_KEY(key::file::size::UNPACKED, GetUnpackedSize());
    VERIFY_KEY(key::file::OFFSET, GetDataOffset());

    return Info::AsInt64(key, handled);
}

size_t File::_AsSize(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::BLOCK_COUNT, 0);

    return Info::AsSize(key, handled);
}

const char* File::GetCompressMethodString()
{
    return "DEFLATE";
}

const void* File::_AsPtr(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::name::NAME, GetFilename());
    VERIFY_KEY(key::file::comment::COMMENT, GetComment());
    VERIFY_KEY(key::file::method::NAME, GetCompressMethodString());
    VERIFY_KEY(key::file::encrypt::VERIFY_DATA, GetVerifyData());

    return Info::AsPtr(key, handled);
}

}
}
