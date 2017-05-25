/*
 * LZMACoder.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_ALGORITHM_LZMACODER_H_
#define NEST_ALGORITHM_LZMACODER_H_

#include <nest/algorithm/Coder.h>
#include <nest/third-party/algorithm/lzma/LzmaDec.h>

namespace nest
{

class LZMACoder: public Coder
{
public:
	LZMACoder();
	virtual ~LZMACoder();

	virtual int Initialize(const unsigned char* dataHeader, size_t size);
	virtual void Finalize();

protected:
	virtual int OnPush(void* inputBuffer, size_t size);
	virtual int OnPop(void* outputBuffer, size_t* size);

private:
	static const int IN_BUF_SIZE = 1 << 16;
	static const int OUT_BUF_SIZE = 1 << 16;

	CLzmaDec state_;
	unsigned char* input_;
	size_t inputSize_;
	size_t inputPos_;
	size_t outputPos_;
};

}

#endif /* NEST_ALGORITHM_LZMACODER_H_ */
