/*
 * FileInputStream.h
 *
 *  Created on: 2010. 12. 13.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_STREAM_FILEINPUTSTREAM_H_
#define NEST_STREAM_FILEINPUTSTREAM_H_

#include <string>
#include <nest/stream/InputStream.h>

namespace nest
{

class FileInputStream: public InputStream
{
public:
    FileInputStream();
    virtual ~FileInputStream();

    virtual int Open();
    virtual void Close();

    virtual uint64 Seek(uint64 pos, int origin);
    virtual uint64 Tell();
    virtual size_t Read(void* buf, size_t size);
    virtual uint64 Size();

private:
    FILE* file_;
    uint64 size_;
};

}

#endif /* NEST_STREAM_FILEINPUTSTREAM_H_ */
