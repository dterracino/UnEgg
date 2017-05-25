/*
 * ZipDecoder.h
 *
 *  Created on: 2011. 3. 29.
 *      Author: revolution
 */

#ifndef NEST_ENCRYPTION_ZIPDECODER_H_
#define NEST_ENCRYPTION_ZIPDECODER_H_

#include <nest/encryption/Decoder.h>

namespace nest
{

// Do not support multibyte string
class ZipDecoder: public Decoder
{
public:
    ZipDecoder();
    virtual ~ZipDecoder();

protected:
    virtual int OnPreprocess(const char* password, const void* header);
    virtual int OnDecrypt(void* buffer, size_t size);
    virtual int OnPostprocess(const void* footer);
    virtual bool OnVerify(const void* verifyData);

private:
    void InitKeys(const char* password);
    void UpdateKeys(unsigned int data);
    unsigned int DecryptByte();

    unsigned int crc_;
    unsigned int keys_[3];
    unsigned char verifyData_;
};

}

#endif // NEST_ENCRYPTION_ZIPDECODER_H_
