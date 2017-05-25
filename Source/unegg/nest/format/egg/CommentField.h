/*
 * CommentField.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_COMMENTFIELD_H_
#define NEST_FORMAT_EGG_COMMENTFIELD_H_

#include <nest/format/egg/ExtraField.h>
#include <string>

namespace nest
{
namespace egg
{

class CommentField: public ExtraField
{
public:
    inline bool IsEncrypted()
    {
        return GetBitFlag(3);
    }

    inline bool IsUTF8()
    {
        return GetBitFlag(4);
    }

    inline unsigned short GetLocale()
    {
        return IsUTF8() ? -1 : *(unsigned short*) GetDataPtr();
    }

    inline const char* GetComment()
    {
        return GetDataPtr(2 - (IsUTF8() ? 2 : 0));
    }
};

}
}

#endif /* NEST_FORMAT_EGG_COMMENTFIELD_H_ */
