/*
 * File.h
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_ALZ_FILE_H_
#define NEST_FORMAT_ALZ_FILE_H_

#include <nest/format/Header.h>

namespace nest
{
namespace alz
{

class File: public Header
{
public:
    File();
    virtual ~File();

    inline unsigned int GetSignature()
    {
        return *(unsigned int*) buffer_;
    }

    inline unsigned short GetFilenameLength()
    {
        return *(unsigned short*) &buffer_[4];
    }

    inline unsigned char GetAttribute()
    {
        return *(unsigned char*) &buffer_[6];
    }

    inline unsigned int GetDateTime()
    {
        return *(unsigned int*) &buffer_[7];
    }

    inline unsigned short GetBitFlags()
    {
        return *((unsigned short*) &buffer_[11]);
    }

    inline bool IsEncrypted()
    {
        return (GetBitFlags() & 1) == 1;
    }

    inline bool IsCommentExists()
    {
        return (GetBitFlags() & 8) == 1;
    }

    inline unsigned char GetBaseSize()
    {
        return (GetBitFlags() & 0xF0) >> 4;
    }

    inline unsigned short GetCompressMethod()
    {
        return compressMethod_ == 2 ? CompressionMethod::Deflate : CompressionMethod::Store;
    }

    inline unsigned int GetCRC()
    {
        return crc_;
    }

    inline uint64 GetPackedSize()
    {
        return packedSize_;
    }

    inline uint64 GetUnpackedSize()
    {
        return unpackedSize_;
    }

    inline uint64 GetDataOffset()
    {
        return offset_;
    }

    inline const char* GetFilename()
    {
        return filename_.c_str();
    }

    inline const char* GetComment()
    {
        return comment_.c_str();
    }

    inline void SetComment(const char* comment)
    {
        comment_ = comment;
    }

    inline const char* GetVerifyData()
    {
        return verifyData_;
    }

protected:
    int ReadFileSize(InputStream* stream, uint64* size, size_t byteCount);

    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);

    const char* GetCompressMethodString();

private:
    unsigned char buffer_[13];
    unsigned int crc_;
    unsigned short compressMethod_;
    uint64 packedSize_;
    uint64 unpackedSize_;
    uint64 offset_;
    char* verifyData_;
    std::string filename_;
    std::string comment_;
};

}
}

#endif /* NEST_FORMAT_ALZ_FILE_H_ */
