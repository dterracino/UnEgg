/*
 * FilenameField.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_FILENAMEFIELD_H_
#define NEST_FORMAT_EGG_FILENAMEFIELD_H_

#include <nest/format/egg/ExtraField.h>

namespace nest
{
namespace egg
{

class FilenameField: public ExtraField
{
public:
    inline bool IsEncrypted()
    {
        return GetBitFlag(3);
    }

    inline bool IsUTF8()
    {
        return !GetBitFlag(4);
    }

    inline bool IsAbsolute()
    {
        return GetBitFlag(5);
    }

    inline unsigned short GetLocale()
    {
        return IsUTF8() ? -1 : *(unsigned short*) GetDataPtr();
    }

    inline unsigned int GetParentPathID()
    {
        return IsAbsolute() ? *(unsigned int*) GetDataPtr(2 - (IsUTF8() ? 2 : 0)) : -1;
    }

    inline const char* GetFileName()
    {
        return GetDataPtr((IsAbsolute() ? 6 : 2) - (IsUTF8() ? 2 : 0));
    }
};

}
}

#endif /* NEST_FORMAT_EGG_FILENAMEFIELD_H_ */
