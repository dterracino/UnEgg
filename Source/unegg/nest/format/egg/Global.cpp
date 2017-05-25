/*
 * Global.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Global.h"
#include <nest/format/egg/EggTypes.h>

namespace nest
{
namespace egg
{

Global::Global()
{
    // TODO Auto-generated constructor stub

}

Global::~Global()
{
    // TODO Auto-generated destructor stub
    Clear();
}

void Global::OnCleanUp()
{

}

int Global::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = Result::ReadError;
    int sig = 0;

    IF_READ_STREAM_VAR(stream, sig)
    {
        if (sig == Magic::EGG)
        {

            IF_READ_STREAM_VAR(stream, version_)
            {

                IF_READ_STREAM_VAR(stream, id_)
                {

                    IF_READ_STREAM_VAR(stream, reserved_)
                    {
                        rewinder->Clear();
                        res = Result::Success;
                    }
                }
            }
        }
    }

    return res;
}

int Global::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::archive::version::MAJOR, GetVersion() >> 8);
    VERIFY_KEY(key::archive::version::MINOR, GetVersion() & 0xFF);
    VERIFY_KEY(key::archive::volume::ID, GetID());

    return Header::_As4Bytes(key, handled);
}

}
}
