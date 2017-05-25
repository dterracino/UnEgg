/*
 * DeflateCoder.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "DeflateCoder.h"
#include <string.h>

namespace nest
{

DeflateCoder::DeflateCoder()
{
	// TODO Auto-generated constructor stub

}

DeflateCoder::~DeflateCoder()
{
	// TODO Auto-generated destructor stub
}

int DeflateCoder::Initialize(const unsigned char* dataHeader, size_t size)
{
	Coder::Initialize(dataHeader, size);

	memset(&stream_, 0, sizeof(stream_));
	stream_.zalloc = Z_NULL;
	stream_.zfree = Z_NULL;
	stream_.opaque = Z_NULL;

	int res = inflateInit2(&stream_, -MAX_WBITS);

	return res == Z_OK ? Result::Success : Result::Fail;
}

void DeflateCoder::Finalize()
{
	Coder::Finalize();

	inflateEnd(&stream_);
}

int DeflateCoder::OnPush(void* inputBuffer, size_t size)
{
	stream_.next_in = static_cast<Bytef*> (inputBuffer);
	stream_.avail_in = static_cast<unsigned int>(size);

	return Result::Success;
}

int DeflateCoder::OnPop(void* outputBuffer, size_t* size)
{
	stream_.avail_out = static_cast<unsigned int>(*size);
	stream_.next_out = static_cast<Bytef*> (outputBuffer);

	int res = inflate(&stream_, Z_NO_FLUSH);

	bool finished = true;
	switch (res)
	{
	case Z_OK:
	case Z_STREAM_END:
	case Z_BUF_ERROR:
	{
		finished = stream_.avail_out != 0 || res == Z_STREAM_END;

		*size -= stream_.avail_out;
		break;
	}
	}

	return finished ? Result::End : Result::Success;
}

}

