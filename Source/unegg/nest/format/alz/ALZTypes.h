/*
 * ALZTypes.h
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_ALZ_ALZTYPES_H_
#define NEST_FORMAT_ALZ_ALZTYPES_H_

#include <nest/nest.h>

namespace nest
{
namespace alz
{

const size_t MAX_PASSWORD_LEN = 65536;
const size_t EXTRACT_CHUNK = 1024 * 4; //1024;

namespace Magic
{
    const unsigned int ALZ = 0x015A4C41;
    const unsigned int FILE = 0x015A4C42;
    const unsigned int END = 0x015A4C43;
    const unsigned int COMMENT = 0x015A4C45;
    const unsigned int SPLIT = 0x035A4C43;
    const unsigned int FILE_END = 0x025A4C43;
}

}
}

#endif /* NEST_FORMAT_ALZ_ALZTYPES_H_ */
