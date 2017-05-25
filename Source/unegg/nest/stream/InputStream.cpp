/*
 * InputStream.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "InputStream.h"

namespace nest
{

int InputStream::Initialize(const char* virtualName)
{
    this->virtualName = virtualName;

    return Result::Success;
}

void InputStream::Finalize()
{
    virtualName.clear();
}

const char* InputStream::GetVirtualName()
{
    return virtualName.c_str();
}

}
