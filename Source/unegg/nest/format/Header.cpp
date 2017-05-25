/*
 * Header.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Header.h"

namespace nest
{

Header::Header() :
loaded_(false)
{
    // TODO Auto-generated constructor stub

}

Header::~Header()
{
    // TODO Auto-generated destructor stub
}

void Header::Clear()
{
    OnCleanUp();
    loaded_ = false;
}

int Header::Read(InputStream* stream)
{
    Clear();

    InputStreamRewinder rewinder(stream);

    int res = OnRead(&rewinder, stream);
    loaded_ = (res == Result::Success) || (res == Result::End);

    return res;
}

}

