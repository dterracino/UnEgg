/*
 * SplitField.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_SPLITFIELD_H_
#define NEST_FORMAT_EGG_SPLITFIELD_H_

#include <nest/format/egg/ExtraField.h>

namespace nest
{
namespace egg
{

class SplitField: public ExtraField
{
public:
    inline unsigned int GetPrevFileID()
    {
        return *(unsigned int*) GetDataPtr(0);
    }

    inline unsigned int GetNextFileID()
    {
        return *(unsigned int*) GetDataPtr(4);
    }
};

}
}

#endif /* NEST_FORMAT_EGG_SPLITFIELD_H_ */
