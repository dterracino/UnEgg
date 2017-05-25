/*
 * FormatFactory.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_FORMATFACTORY_H
#define	NEST_FORMAT_FORMATFACTORY_H

#include <nest/nest.h>
#include <nest/format/Format.h>
#include <nest/stream/InputStream.h>

namespace nest
{

class FormatFactory
{
public:
    virtual Format* CreateFormat() = 0;
    virtual int CheckHeader(InputStream* stream) = 0;
};

}

#endif	/* NEST_FORMAT_FORMATFACTORY_H */

