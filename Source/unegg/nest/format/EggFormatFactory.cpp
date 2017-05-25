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

#include "EggFormatFactory.h"

namespace nest
{

EggFormat EggFormatFactory::staticFormat_;

EggFormatFactory::EggFormatFactory()
{
}

EggFormatFactory::~EggFormatFactory()
{
}

Format* EggFormatFactory::CreateFormat()
{
    return new EggFormat();
}

int EggFormatFactory::CheckHeader(InputStream* stream)
{
    return staticFormat_.CheckHeader(stream);
}

}
