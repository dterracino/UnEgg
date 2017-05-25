/*
 * Header.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_HEADER_H_
#define NEST_FORMAT_HEADER_H_

#include <nest/nest.h>
#include <nest/Info.h>
#include <nest/stream/InputStream.h>
#include <nest/stream/InputStreamRewinder.h>

namespace nest
{

#define IF_READ_STREAM_VAR(stream, var) if (sizeof(var) == stream->Read(&var, sizeof(var)))
#define IF_READ_STREAM_ARY(stream, ary) if (sizeof(ary) == stream->Read(ary, sizeof(ary)))
#define IF_READ_STREAM_PTR(stream, ptr, size) if (size == stream->Read(ptr, size))

class Header: public Info
{
public:
    Header();
    virtual ~Header();

    void Clear();
    int Read(InputStream* stream);

    inline bool IsLoaded()
    {
        return loaded_;
    }

protected:
    virtual void OnCleanUp() = 0;
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream) = 0;

private:
    bool loaded_;
};

}

#endif /* NEST_FORMAT_HEADER_H_ */
