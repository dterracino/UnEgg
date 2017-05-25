/*
 * LZMACoder.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "LZMACoder.h"
#include <nest/third-party/algorithm/lzma/Alloc.h>

namespace nest
{

static void *SzAlloc(void *p, size_t size)
{
    p = p;
    return MyAlloc(size);
}

static void SzFree(void *p, void *address)
{
    p = p;
    MyFree(address);
}

static ISzAlloc g_Alloc = { SzAlloc, SzFree };

LZMACoder::LZMACoder()
{
    // TODO Auto-generated constructor stub

}

LZMACoder::~LZMACoder()
{
    // TODO Auto-generated destructor stub
}

int LZMACoder::Initialize(const unsigned char* dataHeader, size_t size)
{
    Coder::Initialize(dataHeader, size);

    int res = Result::Success;
    LzmaDec_Construct(&state_);
    res = LzmaDec_Allocate(&state_, dataHeader + 4, LZMA_PROPS_SIZE, &g_Alloc);
    if (res == 0)
    {
        LzmaDec_Init(&state_);
    }

    return res;
}

void LZMACoder::Finalize()
{
    Coder::Finalize();

    LzmaDec_Free(&state_, &g_Alloc);
}

int LZMACoder::OnPush(void* inputBuffer, size_t size)
{
    input_ = static_cast<unsigned char*> (inputBuffer);
    inputSize_ = size;
    inputPos_ = 0;

    return Result::Success;
}

int LZMACoder::OnPop(void* outputBuffer, size_t* size)
{
    unsigned char* obuf = static_cast<unsigned char*> (outputBuffer);

    ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
    SRes res;
    size_t inProcessed = inputSize_ - inputPos_;
    ELzmaStatus status;

    res = LzmaDec_DecodeToBuf(&state_, obuf, size, input_ + inputPos_, &inProcessed, finishMode, &status);

    inputPos_ += inProcessed;

    int r = Result::Fail;
    switch (status)
    {
    case LZMA_STATUS_NOT_FINISHED:
        r = Result::Success;
        break;
    case LZMA_STATUS_NOT_SPECIFIED:
        r = Result::Fail;
        break;
    case LZMA_STATUS_NEEDS_MORE_INPUT:
    case LZMA_STATUS_FINISHED_WITH_MARK:
    case LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK:
        r = Result::End;
        break;
    }
    return r;
}

}

