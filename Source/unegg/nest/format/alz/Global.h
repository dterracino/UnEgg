/*
 * Global.h
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_ALZ_GLOBAL_H_
#define NEST_FORMAT_ALZ_GLOBAL_H_

#include <nest/format/Header.h>
#include <nest/format/alz/ALZTypes.h>

namespace nest
{
namespace alz
{

class Global: public Header
{
public:
    Global();
    virtual ~Global();

    inline unsigned short GetVersion()
    {
        return version_;
    }

    inline unsigned short GetDiskID()
    {
        return id_;
    }

    inline unsigned int GetSignature()
    {
        return signature_;
    }

protected:
    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    virtual int _As4Bytes(unsigned int key, bool* handled);

private:
    unsigned int signature_;
    unsigned short version_;
    unsigned short id_;
};

}
}

#endif /* NEST_FORMAT_ALZ_GLOBAL_H_ */
