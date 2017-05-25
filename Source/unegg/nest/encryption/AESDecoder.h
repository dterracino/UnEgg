/*
 * AESDecoder.h
 *
 *  Created on: 2011. 3. 29.
 *      Author: revolution
 */

#ifndef NEST_ENCRYPTION_AESDECODER_H_
#define NEST_ENCRYPTION_AESDECODER_H_

#include <nest/encryption/Decoder.h>
#include <nest/third-party/encryption/aes/fileenc.h>

namespace nest
{

class AESDecoder: public Decoder
{
public:
    enum AESLevel
    {
        AES128, AES256
    };

    AESDecoder(AESLevel level);
    virtual ~AESDecoder();

protected:
    virtual int OnPreprocess(const char* password, const void* header);
    virtual int OnDecrypt(void* buffer, size_t size);
    virtual int OnPostprocess(const void* footer);
    virtual bool OnVerify(const void* verifyData);

private:
    AESLevel level_;
    unsigned char salt_[16];
    fcrypt_ctx zcx_;
    bool verified_;
};

}

#endif /* NEST_ENCRYPTION_AESDECODER_H_ */
