/*
 * EggFormatFactory.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGGFORMATFACTORY_H
#define	NEST_FORMAT_EGGFORMATFACTORY_H

#include <nest/format/FormatFactory.h>
#include <nest/format/EggFormat.h>

namespace nest
{

class EggFormatFactory: public FormatFactory
{
public:
    EggFormatFactory();
    virtual ~EggFormatFactory();

    virtual Format* CreateFormat();
    virtual int CheckHeader(InputStream* stream);

private:
    static EggFormat staticFormat_;
};

}

#endif	/* NEST_FORMAT_EGGFORMATFACTORY_H */

