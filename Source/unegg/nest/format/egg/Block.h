/*
 * Block.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_BLOCK_H_
#define NEST_FORMAT_EGG_BLOCK_H_

#include <nest/format/Header.h>

namespace nest
{
namespace egg
{

class Block: public Header
{
public:
    Block();
    virtual ~Block();

    inline unsigned char GetCompressMethod()
    {
        return compMethod_;
    }

    inline unsigned char GetMethodHint()
    {
        return methodHint_;
    }

    inline unsigned int GetUncompressedSize()
    {
        return uncompSize_;
    }

    inline unsigned int GetCompressedSize()
    {
        return compSize_;
    }

    inline unsigned int GetCRC()
    {
        return crc_;
    }

    inline uint64 GetDataOffset()
    {
        return dataOffset_;
    }

protected:
    virtual void OnCleanUp()
    {
    }
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    // Info
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);

    const char* GetComressMethodString();

private:
    unsigned char compMethod_;
    unsigned char methodHint_;
    unsigned int uncompSize_;
    unsigned int compSize_;
    unsigned int crc_;
    uint64 dataOffset_;
};

}
}

#endif /* NEST_FORMAT_EGG_BLOCK_H_ */
