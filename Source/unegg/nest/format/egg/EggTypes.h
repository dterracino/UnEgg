/*
 * EggTypes.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_EGGTYPES_H_
#define NEST_FORMAT_EGG_EGGTYPES_H_

#include <nest/nest.h>

namespace nest
{
namespace egg
{

const size_t MAX_HEADERS = 100;
const size_t MAX_BLOCKS = 65536;
const size_t MAX_PASSWORD_LEN = 65536;
const size_t EXTRACT_CHUNK = 1024 * 4; //1024;

namespace Magic
{
const int EGG = 0x41474745;
const int FILE = 0x0a8590e3;
const int BLOCK = 0x02b50c13;
const int ENCRYPT = 0x08d1470f;
const int WINDOWS_FILEINFO = 0x2c86950b;
const int POSIX_FILEINFO = 0x1ee922e5;
const int FILENAME = 0x0a8591ac;
const int COMMENT = 0x04c63672;
const int SPLIT = 0x24f5a262;
const int SOLID = 0x24e5a060;
const int DUMMY = 0x07463307;
const int END = 0x08e28222;
}

}
}

#endif /* NEST_FORMAT_EGG_EGGTYPES_H_ */
