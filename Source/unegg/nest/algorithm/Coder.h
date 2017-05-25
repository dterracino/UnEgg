/*
 * Coder.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_ALGORITHM_CODER_H_
#define NEST_ALGORITHM_CODER_H_

#include <nest/nest.h>

namespace nest
{

class Coder
{
public:
    virtual ~Coder();

    virtual int Initialize(const unsigned char* dataHeader, size_t size);
    virtual void Finalize();

    int Push(void* inputBuffer, size_t size);
    int Pop(void* outputBuffer, size_t* size);

protected:
    virtual int OnPush(void* inputBuffer, size_t size);
    virtual int OnPop(void* outputBuffer, size_t* size);
};

}

#endif /* NEST_ALGORITHM_CODER_H_ */
