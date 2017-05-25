/*
 * OutputStream.h
 *
 *  Created on: 2011. 3. 25.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_STREAM_OUTPUTSTREAM_H_
#define NEST_STREAM_OUTPUTSTREAM_H_

#include <stdio.h>
#include <nest/nest.h>

namespace nest
{

class OutputStream
{
public:
    virtual ~OutputStream()
    {
    }
    virtual int Create(const char* virtualName) = 0;
    virtual int CreateFolder(const char* virtualName) = 0;
    virtual void Close() = 0;
    virtual size_t Write(void* buf, size_t size) = 0;
};

}

#endif /* NEST_STREAM_OUTPUTSTREAM_H_ */
