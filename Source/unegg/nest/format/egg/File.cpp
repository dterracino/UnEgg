/*
 * File.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "File.h"
#include <nest/format/egg/EggTypes.h>

namespace nest
{
namespace egg
{

File::File()
{
    // TODO Auto-generated constructor stub

}

File::~File()
{
    // TODO Auto-generated destructor stub
    Clear();
}

int File::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = Result::ReadError;
    int sig = 0;

    IF_READ_STREAM_VAR(stream, sig)
    {
        if (sig == Magic::FILE)
        {

            IF_READ_STREAM_VAR(stream, id_)
            {

                IF_READ_STREAM_VAR(stream, length_)
                {
                    rewinder->Clear();
                    res = Result::Success;
                }
            }
        }
    }

    return res;
}

int File::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::ID, GetID());

    return Header::_As4Bytes(key, handled);
}

int64 File::_As8Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::size::UNPACKED, GetLength());

    return Header::_As8Bytes(key, handled);
}

}
}
