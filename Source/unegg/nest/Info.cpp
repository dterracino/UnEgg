/*
 * Info.cpp
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Info.h"

namespace nest
{

Info::~Info()
{

}

int Info::AsInt32(unsigned int key, bool* handled)
{
    return _As4Bytes(key, handled);
}

unsigned int Info::AsUInt32(unsigned int key, bool* handled)
{
    return static_cast<unsigned int> (_As4Bytes(key, handled));
}

int64 Info::AsInt64(unsigned int key, bool* handled)
{
    return _As8Bytes(key, handled);
}

uint64 Info::AsUInt64(unsigned int key, bool* handled)
{
    return static_cast<unsigned int> (_As8Bytes(key, handled));
}

bool Info::AsBool(unsigned int key, bool* handled)
{
    return _As4Bytes(key, handled) != 0;
}

size_t Info::AsSize(unsigned int key, bool* handled)
{
    return _AsSize(key, handled);
}

const char* Info::AsString(unsigned int key, bool* handled)
{
    return static_cast<const char*> (_AsPtr(key, handled));
}

const void* Info::AsPtr(unsigned int key, bool* handled)
{
    return _AsPtr(key, handled);
}

Info* Info::AsInfo(unsigned int key, bool* handled)
{
    return _AsInfo(key, handled);
}

Info* Info::AsChildInfo(unsigned int key, size_t index, bool* handled)
{
    return _AsChildInfo(key, index, handled);
}

void Info::InfoAssert(bool value, unsigned int key)
{
#ifdef _DEBUG
    if (!value)
    {
        printf("Info assertion failed with key(%u)\n", key);
        assert(value);
    }
#endif
}


int Info::_As4Bytes(unsigned int key, bool* handled)
{
    InfoAssert(handled, key);

    return 0;
}

int64 Info::_As8Bytes(unsigned int key, bool* handled)
{
    InfoAssert(handled, key);

    return 0;
}

size_t Info::_AsSize(unsigned int key, bool* handled)
{
    InfoAssert(handled, key);

    return 0;
}

const void* Info::_AsPtr(unsigned int key, bool* handled)
{
    InfoAssert(handled, key);

    return NULL;
}

Info* Info::_AsInfo(unsigned int key, bool* handled)
{
    InfoAssert(handled, key);

    return NULL;
}

Info* Info::_AsChildInfo(unsigned int key, size_t index, bool* handled)
{
    InfoAssert(handled, key);

    return NULL;
}

}
