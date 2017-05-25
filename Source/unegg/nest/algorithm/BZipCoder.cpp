/*
 * BZipCoder.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "BZipCoder.h"

namespace nest
{

BZipCoder::BZipCoder()
{
	// TODO Auto-generated constructor stub

}

BZipCoder::~BZipCoder()
{
	// TODO Auto-generated destructor stub
}

int BZipCoder::Initialize(const unsigned char* dataHeader, size_t size)
{
	Coder::Initialize(dataHeader, size);

	memset(&stream_, 0, sizeof(stream_));

	int res = BZ2_bzDecompressInit(&stream_, 0, 0);

	return res == BZ_OK ? Result::Success : Result::Fail;
}

void BZipCoder::Finalize()
{
	Coder::Finalize();

	BZ2_bzDecompressEnd(&stream_);
}

int BZipCoder::OnPush(void* inputBuffer, size_t size)
{
	stream_.next_in = static_cast<char*> (inputBuffer);
	stream_.avail_in = static_cast<unsigned int>(size);

	return Result::Success;
}

int BZipCoder::OnPop(void* outputBuffer, size_t* size)
{
	stream_.avail_out = static_cast<unsigned int>(*size);
	stream_.next_out = static_cast<char*> (outputBuffer);

	int res = BZ2_bzDecompress(&stream_);
	int result = Result::Invalid;

	switch (res)
	{
	case BZ_OK:
	case BZ_STREAM_END:
		*size -= stream_.avail_out;

		if (res == BZ_STREAM_END)
		{
			result = Result::End;
		}
		else
		{
			result = stream_.avail_out == 0 ? Result::Success : Result::End;
		}
		break;
	default:
		*size = 0;
	}

	return result;
}

}

