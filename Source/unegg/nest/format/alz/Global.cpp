/*
 * Global.cpp
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Global.h"

namespace nest
{
namespace alz
{

Global::Global() :
    signature_(0), version_(0), id_(0)
{
    // TODO Auto-generated constructor stub

}

Global::~Global()
{
    // TODO Auto-generated destructor stub

}

void Global::OnCleanUp()
{
    signature_ = 0;
    version_ = 0;
    id_ = 0;
}

int Global::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = Result::ReadError;

    IF_READ_STREAM_VAR(stream, signature_)
    {
        if (signature_ == Magic::ALZ)
        {

            IF_READ_STREAM_VAR(stream, version_)
            {

                IF_READ_STREAM_VAR(stream, id_)
                {

                    rewinder->Clear();

                    res = Result::Success;
                }
            }
        }
    }

    return res;
}

int Global::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::version::MAJOR, version_ >> 8);
    VERIFY_KEY(key::archive::version::MINOR, version_ & 0xFF);
    VERIFY_KEY(key::archive::volume::ID, id_);

    return Header::_As4Bytes(key, handled);
}

}
}
