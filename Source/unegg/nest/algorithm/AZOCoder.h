/*
 * AZOCoder.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_ALGORITHM_AZOCODER_H_
#define NEST_ALGORITHM_AZOCODER_H_

#include <nest/algorithm/Coder.h>
#include <nest/algorithm/azo/AZOWrapper.h>

namespace nest
{

class AZOCoder: public Coder
{
public:
    AZOCoder();
    virtual ~AZOCoder();

    virtual int Initialize(const unsigned char* dataHeader, size_t size);
    virtual void Finalize();

protected:
    virtual int OnPush(void* inputBuffer, size_t size);
    virtual int OnPop(void* outputBuffer, size_t* size);

private:
    AZO_Stream stream_;
    AZODecoder* decoder_;
};

}

#endif /* NEST_ALGORITHM_AZOCODER_H_ */
