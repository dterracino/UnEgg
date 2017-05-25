/*
 * ALZFormatFactory.h
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_ALZFORMATFACTORY_H_
#define NEST_FORMAT_ALZFORMATFACTORY_H_

#include <nest/format/FormatFactory.h>
#include <nest/format/ALZFormat.h>

namespace nest
{

class ALZFormatFactory: public FormatFactory
{
public:
    ALZFormatFactory();
    virtual ~ALZFormatFactory();

    virtual Format* CreateFormat();
    virtual int CheckHeader(InputStream* stream);

private:
    static ALZFormat staticFormat_;
};

}

#endif /* NEST_FORMAT_ALZFORMATFACTORY_H_ */
