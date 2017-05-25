/*
 * Decoder.h
 *
 *  Created on: 2011. 3. 29.
 *      Author: revolution
 */

#ifndef NEST_ENCRYPTION_DECODER_H_
#define NEST_ENCRYPTION_DECODER_H_

#include <nest/nest.h>

namespace nest
{

class Decoder
{
public:
    virtual ~Decoder();

    int PreprocessDecrypt(const char* password, const void* header);
    int DecryptBuffer(void* buffer, size_t size);
    int PostprocessDecrypt(const void* footer);

    bool VerifyNow(const void* verifyData);

protected:
    virtual int OnPreprocess(const char* password, const void* header) = 0;
    virtual int OnDecrypt(void* buffer, size_t size) = 0;
    virtual int OnPostprocess(const void* footer) = 0;
    virtual bool OnVerify(const void* verifyData) = 0;
};

}

#endif /* NEST_ENCRYPTION_DECODER_H_ */
