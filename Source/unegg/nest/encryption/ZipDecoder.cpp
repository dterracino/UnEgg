/*
 * ZipDecoder.cpp
 *
 *  Created on: 2011. 3. 29.
 *      Author: revolution
 */

#include "ZipDecoder.h"
#include "../algorithm/crc32.hpp"

namespace nest
{

ZipDecoder::ZipDecoder()
{
    // TODO Auto-generated constructor stub

}

ZipDecoder::~ZipDecoder()
{
    // TODO Auto-generated destructor stub
}

unsigned int ZipDecoder::DecryptByte()
{
    unsigned int res = (keys_[2] | 2) & 0xFFFF;

    return ((res * (res ^ 1)) >> 8) & 0xFF;
}

void ZipDecoder::InitKeys(const char* password)
{
    keys_[0] = 305419896;
    keys_[1] = 591751049;
    keys_[2] = 878082192;

    for (size_t i = 0; i < strlen(password); ++i)
    {
        UpdateKeys(password[i]);
    }
}

void ZipDecoder::UpdateKeys(unsigned int data)
{
    data = data & 0xFF;
    keys_[0] = g_crc32Table[(keys_[0] ^ data) & 0xFF] ^ (keys_[0] >> 8);
    keys_[1] = keys_[1] + (keys_[0] & 0xFF);
    keys_[1] = keys_[1] * 134775813 + 1;
    keys_[2] = g_crc32Table[(keys_[2] ^ (keys_[1] >> 24)) & 0xFF] ^ ((keys_[2] >> 8) & 0x00FFFFFF);
}

int ZipDecoder::OnPreprocess(const char* password, const void* header)
{
    InitKeys(password);
    const unsigned char* p = static_cast<const unsigned char*> (header);

    unsigned int res;
    for (int i = 0; i < 12; ++i)
    {
        res = (p[i] ^ DecryptByte()) & 0xFF;
        UpdateKeys(res);
    }
    verifyData_ = static_cast<unsigned char> (res);

    return Result::Success;
}

int ZipDecoder::OnDecrypt(void* buffer, size_t size)
{
    unsigned char* p = static_cast<unsigned char*> (buffer);

    for (size_t i = 0; i < size; ++i)
    {
        p[i] = p[i] ^ DecryptByte();
        UpdateKeys(p[i]);
    }

    return Result::Success;
}

int ZipDecoder::OnPostprocess(const void* footer)
{
    return Result::Success;
}

bool ZipDecoder::OnVerify(const void* verifyData)
{
    return verifyData ? (verifyData_ == *(static_cast<const unsigned char*> (verifyData))) : true;
}

}
