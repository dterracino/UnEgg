/*
 * Format.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_FORMAT_H
#define	NEST_FORMAT_FORMAT_H

#include <nest/nest.h>
#include <nest/Events.h>
#include <nest/Info.h>
#include <nest/stream/InputStream.h>

namespace nest
{

class Format {
public:

    Format()
    {
    }

    Format(Events* evnt) :
    event(evnt)
    {
    }

    virtual ~Format()
    {
    }

    void SetEvent(Events* evnt)
    {
        event = evnt;
    }

    Events* GetEvent()
    {
        return event;
    }

    virtual int Open(InputStream* stream) = 0;
    virtual void Close() = 0;
    virtual int CheckHeader(InputStream* stream) = 0;
    virtual int Scan() = 0;
    virtual int Extract() = 0;
    virtual InputStream* GetInputStream() = 0;
    virtual Info* GetInfo() = 0;

protected:
    Events* event;
};

}

#endif	/* NEST_FORMAT_FORMAT_H */

