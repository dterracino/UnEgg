/*
 * Dummy.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_DUMMY_H_
#define NEST_FORMAT_EGG_DUMMY_H_

#include <nest/format/egg/ExtraField.h>

namespace nest
{
namespace egg
{

class Dummy: public ExtraField
{
public:
    Dummy();
    virtual ~Dummy();
};

}
}

#endif /* NEST_FORMAT_EGG_DUMMY_H_ */
