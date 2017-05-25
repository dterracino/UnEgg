/*
 * FileOutputStream.h
 *
 *  Created on: 2011. 3. 25.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_STREAM_FILEOUTPUTSTREAM_H_
#define NEST_STREAM_FILEOUTPUTSTREAM_H_

#include <string>
#include <nest/stream/OutputStream.h>

namespace nest
{

class FileOutputStream: public OutputStream
{
public:
    FileOutputStream();
    virtual ~FileOutputStream();

    virtual int Create(const char* virtualName);
    virtual int CreateFolder(const char* virtualName);
    virtual void Close();

    bool IsOpened()
    {
        return file_ != NULL;
    }

    virtual size_t Write(void* buf, size_t size);

protected:
    int EnsureFilePath(const char* path);
    int ForceDirectory(const char* path);

private:
    std::string filename_;
    FILE* file_;
};

}

#endif /* NEST_STREAM_FILEOUTPUTSTREAM_H_ */
