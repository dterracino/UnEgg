/*
 * Info.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_INFO_H
#define	NEST_INFO_H

#include <nest/nest.h>

namespace nest
{

class Info
{
public:
    virtual ~Info();
    int AsInt32(unsigned int key, bool* handled = NULL);
    unsigned int AsUInt32(unsigned int key, bool* handled = NULL);
    int64 AsInt64(unsigned int key, bool* handled = NULL);
    uint64 AsUInt64(unsigned int key, bool* handled = NULL);
    bool AsBool(unsigned int key, bool* handled = NULL);
    size_t AsSize(unsigned int key, bool* handled = NULL);
    const char* AsString(unsigned int key, bool* handled = NULL);
    const void* AsPtr(unsigned int key, bool* handled = NULL);
    Info* AsInfo(unsigned int key, bool* handled = NULL);
    Info* AsChildInfo(unsigned int key, size_t index, bool* handled = NULL);

protected:
    void InfoAssert(bool value, unsigned int key);

    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);
    virtual Info* _AsInfo(unsigned int key, bool* handled);
    virtual Info* _AsChildInfo(unsigned int key, size_t index, bool* handled);
};

}

#endif	/* NEST_INFO_H */

