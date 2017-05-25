/*
 * FileInputStream.cpp
 *
 *  Created on: 2010. 12. 13.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "FileInputStream.h"

namespace nest
{

FileInputStream::FileInputStream() :
    file_(NULL), size_(-1)
{
    // TODO Auto-generated constructor stub
}

FileInputStream::~FileInputStream()
{
    // TODO Auto-generated destructor stub
    Close();
}

int FileInputStream::Open()
{
    Close();
    size_ = -1;

    file_ = fopen(virtualName.c_str(), "rb");
    if (file_ != NULL)
    {
        Size();
    }

    return file_ != NULL ? Result::Success : Result::ReadError;
}

void FileInputStream::Close()
{
    if (file_)
    {
        fclose(file_);
        file_ = NULL;
    }
}

uint64 FileInputStream::Seek(uint64 pos, int origin)
{
    return fseeko64(file_, pos, origin);
}

uint64 FileInputStream::Tell()
{
    return ftello(file_);
}

size_t FileInputStream::Read(void* buf, size_t size)
{
    return fread(buf, 1, size, file_);
}

uint64 FileInputStream::Size()
{
    if (size_ == static_cast<uint64> (-1))
    {
        uint64 offset = Tell();
        Seek(0, SEEK_END);
        size_ = Tell();
        Seek(offset, SEEK_SET);
    }

    return size_;
}

}
