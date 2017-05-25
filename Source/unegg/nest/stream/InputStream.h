/*
 * InputStream.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_STREAM_INPUTSTREAM_H_
#define NEST_STREAM_INPUTSTREAM_H_

#include <nest/nest.h>
#include <stdio.h>
#include <string>

namespace nest
{

class InputStream
{
public:
    virtual ~InputStream()
    {
    }
    virtual int Initialize(const char* virtualName);
    virtual void Finalize();
    virtual const char* GetVirtualName();
    virtual int Open() = 0;
    virtual void Close() = 0;
    virtual uint64 Seek(uint64 pos, int origin) = 0;
    virtual uint64 Tell() = 0;
    virtual size_t Read(void* buf, size_t size) = 0;
    virtual uint64 Size() = 0;

protected:
    std::string virtualName;
};

}

#endif /* NEST_STREAM_INPUTSTREAM_H_ */
