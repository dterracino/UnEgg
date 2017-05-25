/*
 * StoreCoder.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_ALGORITHM_STORECODER_H_
#define NEST_ALGORITHM_STORECODER_H_

#include <nest/algorithm/Coder.h>

namespace nest
{

class StoreCoder: public Coder
{
public:
    StoreCoder();
    virtual ~StoreCoder();

    virtual int Initialize(const unsigned char* dataHeader, size_t size);
    virtual void Finalize();

protected:
    virtual int OnPush(void* inputBuffer, size_t size);
    virtual int OnPop(void* outputBuffer, size_t* size);

private:
    void* buffer_;
    size_t size_;
};

}

#endif /* NEST_ALGORITHM_STORECODER_H_ */
