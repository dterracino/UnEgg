/*
 * AZOCoder.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "AZOCoder.h"

namespace nest
{

AZOCoder::AZOCoder()
{
	// TODO Auto-generated constructor stub

}

AZOCoder::~AZOCoder()
{
	// TODO Auto-generated destructor stub
}

int AZOCoder::Initialize(const unsigned char* dataHeader, size_t size)
{
	Coder::Initialize(dataHeader, size);

	decoder_ = new AZODecoder();
	memset(&stream_, 0, sizeof(stream_));

	return Result::Success;
}

void AZOCoder::Finalize()
{
	Coder::Finalize();

	delete decoder_;
	decoder_ = NULL;
}

int AZOCoder::OnPush(void* inputBuffer, size_t size)
{
	stream_.next_in = static_cast<char*> (inputBuffer);
	stream_.avail_in = static_cast<unsigned int> (size);

	return Result::Success;
}

int AZOCoder::OnPop(void* outputBuffer, size_t* size)
{
	stream_.avail_out = static_cast<unsigned int>(*size);
	stream_.next_out = static_cast<char*> (outputBuffer);

	int res = decoder_->Decompress(stream_);

	bool cont = stream_.avail_out == 0;

	*size -= stream_.avail_out;

	if (res == AZO_STREAM_END)
	{
		return Result::End;
	}

	if (cont)
	{
		return Result::Success;
	}

	return Result::End;
}

}
