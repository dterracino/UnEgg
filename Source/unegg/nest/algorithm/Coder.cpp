/*
 * Coder.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Coder.h"

namespace nest
{

Coder::~Coder()
{
    // TODO Auto-generated destructor stub
}

int Coder::Initialize(const unsigned char* dataHeader, size_t size)
{
    return Result::Fail;
}

int Coder::Push(void* inputBuffer, size_t size)
{
    return OnPush(inputBuffer, size);
}

int Coder::Pop(void* outputBuffer, size_t* size)
{
    return OnPop(outputBuffer, size);
}

void Coder::Finalize()
{
}

int Coder::OnPush(void* inputBuffer, size_t size)
{
    return Result::Fail;
}

int Coder::OnPop(void* outputBuffer, size_t* size)
{
    return Result::Fail;
}

}

