/*
 * StoreCoder.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "StoreCoder.h"

namespace nest
{

StoreCoder::StoreCoder()
{
    // TODO Auto-generated constructor stub
}

StoreCoder::~StoreCoder()
{
    // TODO Auto-generated destructor stub
}

int StoreCoder::Initialize(const unsigned char* dataHeader, size_t size)
{
    Coder::Initialize(dataHeader, size);
    size_ = 0;
    buffer_ = NULL;

    return Result::Success;
}

void StoreCoder::Finalize()
{
    Coder::Finalize();
}

int StoreCoder::OnPush(void* inputBuffer, size_t size)
{
    buffer_ = inputBuffer;
    size_ = size;

    return Result::Success;
}

int StoreCoder::OnPop(void* outputBuffer, size_t* size)
{
    *size = size_;

    int res = Result::End;

    if (size_)
    {
        memcpy(outputBuffer, buffer_, size_);
        size_ = 0;
        res = Result::Success;
    }

    return res;
}

}
