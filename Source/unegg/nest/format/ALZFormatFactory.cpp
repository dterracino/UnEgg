/*
 * ALZFormatFactory.cpp
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "ALZFormatFactory.h"
#include <nest/format/ALZFormat.h>

namespace nest
{

ALZFormat ALZFormatFactory::staticFormat_;

ALZFormatFactory::ALZFormatFactory()
{
    // TODO Auto-generated constructor stub

}

ALZFormatFactory::~ALZFormatFactory()
{
    // TODO Auto-generated destructor stub
}

Format* ALZFormatFactory::CreateFormat()
{
    return new ALZFormat();
}

int ALZFormatFactory::CheckHeader(InputStream* stream)
{
    return staticFormat_.CheckHeader(stream);
}

}
