/*
 * PosixField.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_POSIXFIELD_H_
#define NEST_FORMAT_EGG_POSIXFIELD_H_

#include <nest/format/egg/ExtraField.h>

namespace nest
{
namespace egg
{

class PosixField: public ExtraField
{
public:
    inline int GetMode()
    {
        return *(int*) GetDataPtr();
    }

    inline int GetUID()
    {
        return *(int*) GetDataPtr(4);
    }

    inline int GetGID()
    {
        return *(int*) GetDataPtr(8);
    }

    inline int64 GetLastModifiedDateTime()
    {
        return *(int64*) GetDataPtr(12);
    }
};

}
}

#endif /* NEST_FORMAT_EGG_POSIXFIELD_H_ */
