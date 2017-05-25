/*
 * BlockWrap.cpp
 *
 *  Created on: 2010. 12. 13.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "BlockWrap.h"

namespace nest
{
namespace egg
{

BlockWrap::BlockWrap()
{
    // TODO Auto-generated constructor stub

}

BlockWrap::~BlockWrap()
{
    // TODO Auto-generated destructor stub
    Clear();
}

void BlockWrap::OnCleanUp()
{
    block_.Clear();
    extras_.Clear();
    dataOffset_ = 0;
}

int BlockWrap::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = block_.Read(stream);

    if (res == Result::Success)
    {
        res = extras_.Read(stream);

        if (res == Result::Success)
        {
            dataOffset_ = stream->Tell();
            rewinder->Clear();
        }
    }
    else
    {
        res = Result::End;
    }

    return res;
}

int BlockWrap::_As4Bytes(unsigned int key, bool* handled)
{
    bool handle = false;
    int res = block_.AsInt32(key, &handle);
    if (!handle)
    {
        res = extras_.AsInt32(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

int64 BlockWrap::_As8Bytes(unsigned int key, bool* handled)
{
    bool handle = false;
    int64 res = block_.AsInt64(key, &handle);
    if (!handle)
    {
        res = extras_.AsInt64(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

size_t BlockWrap::_AsSize(unsigned int key, bool* handled)
{
    bool handle = false;
    size_t res = block_.AsSize(key, &handle);
    if (!handle)
    {
        res = extras_.AsSize(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

const void* BlockWrap::_AsPtr(unsigned int key, bool* handled)
{
    bool handle = false;
    const void* res = block_.AsPtr(key, &handle);
    if (!handle)
    {
        res = extras_.AsPtr(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

Info* BlockWrap::_AsInfo(unsigned int key, bool* handled)
{
    bool handle = false;
    Info* res = block_.AsInfo(key, &handle);
    if (!handle)
    {
        res = extras_.AsInfo(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

Info* BlockWrap::_AsChildInfo(unsigned int key, size_t index, bool* handled)
{
    bool handle = false;
    Info* res = block_.AsChildInfo(key, index, &handle);
    if (!handle)
    {
        res = extras_.AsChildInfo(key, index, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}


}
}
