/*
 * ExtraField.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_EXTRAFIELD_H_
#define NEST_FORMAT_EGG_EXTRAFIELD_H_

#include <nest/format/Header.h>

namespace nest
{
namespace egg
{

class ExtraField: public Header
{
public:
    ExtraField();
    virtual ~ExtraField();

    int GetSignature();
    unsigned char GetBitFlags();
    bool GetBitFlag(size_t index);
    unsigned int GetSize();
    const char* GetDataPtr(size_t index = 0);

protected:
    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

private:
    int signature_;
    unsigned char gpb_;
    unsigned int size_;
    char* data_;
};

}
}

#endif /* NEST_FORMAT_EGG_EXTRAFIELD_H_ */
