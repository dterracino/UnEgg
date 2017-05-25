/*
 * ExtraField.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "ExtraField.h"
#include <nest/format/egg/EggTypes.h>

namespace nest
{
namespace egg
{

ExtraField::ExtraField() :
    gpb_(0), size_(0), data_(NULL)
{
    // TODO Auto-generated constructor stub

}

ExtraField::~ExtraField()
{
    // TODO Auto-generated destructor stub
    Clear();
}

void ExtraField::OnCleanUp()
{
    delete[] data_;
    data_ = NULL;
}

int ExtraField::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = Result::ReadError;

    IF_READ_STREAM_VAR(stream, signature_)
    {
        if (signature_ == Magic::END)
        {
            res = Result::End;
            rewinder->Clear();
        }
        else
        {

            IF_READ_STREAM_VAR(stream, gpb_)
            {
                unsigned int ls = 0;
                unsigned short ss = 0;

                if ((gpb_ & 1) == 1)
                {

                    IF_READ_STREAM_VAR(stream, ls)
                    {
                        size_ = ls;
                    }
                    else
                    {
                        return res;
                    }
                }
                else
                {

                    IF_READ_STREAM_VAR(stream, ss)
                    {
                        size_ = ss;
                    }
                    else
                    {
                        return res;
                    }
                }

                if (size_)
                {
                    // for filename and comment..
                    data_ = new char[size_ + 1];
                    data_[size_] = 0;

                    IF_READ_STREAM_PTR(stream, data_, size_)
                    {
                        res = Result::Success;
                        rewinder->Clear();
                    }
                    else
                    {
                        delete[] data_;
                    }
                }
                else
                {
                    res = Result::Success;
                    rewinder->Clear();
                }
            }
        }
    }

    return res;
}

int ExtraField::GetSignature()
{
    return signature_;
}

unsigned char ExtraField::GetBitFlags()
{
    return gpb_;
}

bool ExtraField::GetBitFlag(size_t index)
{
    return gpb_ & (1 << (index - 1));
}

unsigned int ExtraField::GetSize()
{
    return size_;
}

const char* ExtraField::GetDataPtr(size_t index)
{
    return data_ + index;
}

}
}
