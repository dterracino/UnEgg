/*
 * Global.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_GLOBAL_H_
#define NEST_FORMAT_EGG_GLOBAL_H_

#include <nest/format/Header.h>

namespace nest
{
namespace egg
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
    inline unsigned int GetID()
    {
        return id_;
    }
    inline unsigned int GetReserved()
    {
        return reserved_;
    }

protected:
    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    virtual int _As4Bytes(unsigned int key, bool* handled);

private:
    unsigned short version_;
    unsigned int id_;
    unsigned int reserved_;
};

}
}

#endif /* NEST_FORMAT_EGG_GLOBAL_H_ */
