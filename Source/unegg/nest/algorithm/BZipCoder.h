/*
 * BZipCoder.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_ALGORITHM_BZIPCODER_H_
#define NEST_ALGORITHM_BZIPCODER_H_

#include <nest/algorithm/Coder.h>
#include <nest/third-party/algorithm/bzip2/bzlib.h>

namespace nest
{

class BZipCoder: public Coder
{
public:
    BZipCoder();
    virtual ~BZipCoder();

    virtual int Initialize(const unsigned char* dataHeader, size_t size);
    virtual void Finalize();

protected:
    virtual int OnPush(void* inputBuffer, size_t size);
    virtual int OnPop(void* outputBuffer, size_t* size);

private:
    bz_stream stream_;
};

}

#endif /* NEST_ALGORITHM_BZIPCODER_H_ */
