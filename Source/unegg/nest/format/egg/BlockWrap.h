/*
 * BlockWrap.h
 *
 *  Created on: 2010. 12. 13.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_BLOCKWRAP_H_
#define NEST_FORMAT_EGG_BLOCKWRAP_H_

#include <nest/format/Header.h>
#include <nest/format/egg/Block.h>
#include <nest/format/egg/ExtraFieldWrap.h>

namespace nest
{
namespace egg
{

class BlockWrap : public Header
{
public:
    BlockWrap();
    virtual ~BlockWrap();

    inline Block& GetBlock()
    {
        return block_;
    }

    inline ExtraFieldWrap& GetExtras()
    {
        return extras_;
    }

    inline uint64 GetDataOffset()
    {
        return dataOffset_;
    }

protected:
    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    // Info
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);
    virtual Info* _AsInfo(unsigned int key, bool* handled);
    virtual Info* _AsChildInfo(unsigned int key, size_t index, bool* handled);

private:
    Block block_;
    ExtraFieldWrap extras_;
    uint64 dataOffset_;
};

}
}

#endif /* NEST_FORMAT_EGG_BLOCKWRAP_H_ */
