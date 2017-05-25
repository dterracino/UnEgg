/*
 * nest.h
 *
 *  Created on: 2011. 5. 23.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_NEST_H_
#define NEST_NEST_H_

#include <nest/define.h>

namespace nest
{

namespace key
{
static unsigned int _CHILD = 0;
}

#define DEFINE_CATEGORY(category) \
    namespace key \
    {\
    namespace category \
    {\
        static unsigned int OBJECT = _CHILD++ << 4; \
        static unsigned int _NEXT = 0; \
    }\
    }

#define DEFINE_CATEGORY2(category, category2) \
    namespace key \
    {\
    namespace category \
    {\
    namespace category2 \
    {\
        static unsigned int OBJECT = _CHILD++ << 4; \
        static unsigned int _NEXT = 0; \
    }\
    }\
    }

#define DEFINE_KEY(category, name) \
    namespace key\
    {\
    namespace category \
    {\
        static const unsigned int name = OBJECT | ++_NEXT; \
    }\
    }

#define DEFINE_KEY2(category, category2, name) \
    namespace key \
    {\
    namespace category \
    {\
    namespace category2 \
    {\
        static const unsigned int name = OBJECT | ++_NEXT; \
    }\
    }\
    }

#define VERIFY_KEY(value, result) \
    if (key == value) \
    { \
        if (handled) \
        { \
            *handled = true; \
        } \
        return result; \
    }

DEFINE_CATEGORY(archive)
DEFINE_KEY(archive, IS_SOLID)
DEFINE_KEY(archive, IS_SPANNED)
DEFINE_KEY(archive, FILE_COUNT)
DEFINE_KEY(archive, BLOCK_COUNT)
DEFINE_CATEGORY2(archive, version)
DEFINE_KEY2(archive, version, MAJOR)
DEFINE_KEY2(archive, version, MINOR)
DEFINE_CATEGORY2(archive, volume)
DEFINE_KEY2(archive, volume, ID)
DEFINE_KEY2(archive, volume, NEXT)
DEFINE_KEY2(archive, volume, PREV)
DEFINE_CATEGORY2(archive, format)
DEFINE_KEY2(archive, format, NAME)
DEFINE_CATEGORY2(archive, comment)
DEFINE_KEY2(archive, comment, COMMENT)
DEFINE_KEY2(archive, comment, IS_ENCRYPTED)
DEFINE_KEY2(archive, comment, IS_UTF8)
DEFINE_KEY2(archive, comment, LOCALE)
DEFINE_CATEGORY2(archive, encrypt)
DEFINE_KEY2(archive, encrypt, METHOD)
DEFINE_KEY2(archive, encrypt, CRC)
DEFINE_KEY2(archive, encrypt, VERIFY_DATA)
DEFINE_KEY2(archive, encrypt, HEADER)
DEFINE_KEY2(archive, encrypt, FOOTER)

DEFINE_CATEGORY(file)
DEFINE_KEY(file, BLOCK_COUNT)
DEFINE_KEY(file, IS_ENCRYPTED)
DEFINE_KEY(file, ID)
DEFINE_KEY(file, CRC)
DEFINE_KEY(file, OFFSET)
DEFINE_CATEGORY2(file, method)
DEFINE_KEY2(file, method, METHOD)
DEFINE_KEY2(file, method, HINT)
DEFINE_KEY2(file, method, NAME)
DEFINE_CATEGORY2(file, name)
DEFINE_KEY2(file, name, NAME)
DEFINE_KEY2(file, name, IS_ENCRYPTED)
DEFINE_KEY2(file, name, IS_UTF8)
DEFINE_KEY2(file, name, IS_ABSOLUTE)
DEFINE_KEY2(file, name, LOCALE)
DEFINE_KEY2(file, name, PARENT_ID)
DEFINE_CATEGORY2(file, size)
DEFINE_KEY2(file, size, PACKED)
DEFINE_KEY2(file, size, UNPACKED)
DEFINE_CATEGORY2(file, comment)
DEFINE_KEY2(file, comment, COMMENT)
DEFINE_KEY2(file, comment, IS_ENCRYPTED)
DEFINE_KEY2(file, comment, IS_UTF8)
DEFINE_KEY2(file, comment, LOCALE)
DEFINE_CATEGORY2(file, attribute)
DEFINE_KEY2(file, attribute, POSIX_MODE)
DEFINE_KEY2(file, attribute, UID)
DEFINE_KEY2(file, attribute, GID)
DEFINE_KEY2(file, attribute, ATTRIBUTES)
DEFINE_KEY2(file, attribute, MODIFIED)
DEFINE_CATEGORY2(file, encrypt)
DEFINE_KEY2(file, encrypt, METHOD)
DEFINE_KEY2(file, encrypt, CRC)
DEFINE_KEY2(file, encrypt, VERIFY_DATA)
DEFINE_KEY2(file, encrypt, HEADER)
DEFINE_KEY2(file, encrypt, FOOTER)

DEFINE_CATEGORY(block)
DEFINE_KEY(block, CRC)
DEFINE_KEY(block, OFFSET)
DEFINE_CATEGORY2(block, method)
DEFINE_KEY2(block, method, METHOD)
DEFINE_KEY2(block, method, HINT)
DEFINE_KEY2(block, method, NAME)
DEFINE_CATEGORY2(block, size)
DEFINE_KEY2(block, size, PACKED)
DEFINE_KEY2(block, size, UNPACKED)
DEFINE_CATEGORY2(block, encrypt)
DEFINE_KEY2(block, encrypt, METHOD)
DEFINE_KEY2(block, encrypt, CRC)
DEFINE_KEY2(block, encrypt, VERIFY_DATA)
DEFINE_KEY2(block, encrypt, HEADER)
DEFINE_KEY2(block, encrypt, FOOTER)

namespace CompressionMethod
{
enum
{
    Store, Deflate, bzip, azo, lzma
};
}

namespace WorkCode
{
enum
{
    Check, Open, Scan, Extract
};
}

namespace EncryptMethod
{
const int ZIP_COMPATIBLE = 0;
const int AES_128 = 1;
const int AES_256 = 2;
}
}

#endif /* NEST_NEST_H_ */
