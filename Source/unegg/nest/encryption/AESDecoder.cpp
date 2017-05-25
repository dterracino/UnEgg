/*
 * AESDecoder.cpp
 *
 *  Created on: 2011. 3. 29.
 *      Author: revolution
 */

#include "AESDecoder.h"

namespace nest
{

AESDecoder::AESDecoder(AESLevel level) :
    level_(level)
{
    // TODO Auto-generated constructor stub

}

AESDecoder::~AESDecoder()
{
    // TODO Auto-generated destructor stub
}

int AESDecoder::OnPreprocess(const char* password, const void* header)
{
    int mode = level_ == AES128 ? 1 : 3;
    unsigned int saltSize = SALT_LENGTH(mode);

    memset(salt_, 0, sizeof(salt_));
    memcpy(salt_, header, saltSize);

    unsigned char tmp_buf[16] = { 0 };
    fcrypt_init(mode, (const unsigned char*) password, static_cast<int> (strlen(password)), salt_, tmp_buf, &zcx_);

    verified_ = memcmp(static_cast<const unsigned char*> (header) + saltSize, tmp_buf, PWD_VER_LENGTH) == 0;

    return Result::Success;
}

int AESDecoder::OnDecrypt(void* buffer, size_t size)
{
    fcrypt_decrypt(static_cast<unsigned char*> (buffer), static_cast<int> (size), &zcx_);

    return Result::Success;
}

int AESDecoder::OnPostprocess(const void* footer)
{
    unsigned char tmp_buf[16] = { 0 };
    fcrypt_end(tmp_buf, &zcx_);

    verified_ = memcmp(footer, tmp_buf, 10) == 0;

    return Result::Success;
}

bool AESDecoder::OnVerify(const void* verifyData)
{
    return verified_;
}

}
