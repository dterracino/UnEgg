/*
 * Decoder.cpp
 *
 *  Created on: 2011. 3. 29.
 *      Author: revolution
 */

#include "Decoder.h"

namespace nest
{

Decoder::~Decoder()
{
    // TODO Auto-generated destructor stub
}

int Decoder::PreprocessDecrypt(const char* password, const void* header)
{
    return OnPreprocess(password, header);
}

int Decoder::DecryptBuffer(void* buffer, size_t size)
{
    return OnDecrypt(buffer, size);
}

int Decoder::PostprocessDecrypt(const void* footer)
{
    return OnPostprocess(footer);
}

bool Decoder::VerifyNow(const void* verifyData)
{
    return OnVerify(verifyData);
}

}
