/*
 * ExtraFieldWrap.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_EXTRAFIELDWRAP_H_
#define NEST_FORMAT_EGG_EXTRAFIELDWRAP_H_

#include <nest/format/Header.h>
#include <nest/format/egg/ExtraField.h>

namespace nest
{
namespace egg
{

class ExtraFieldWrap: public Header
{
public:
    ExtraFieldWrap();
    virtual ~ExtraFieldWrap();

    inline bool FieldExists(int magic)
    {
        return GetExtra(magic) != NULL;
    }

    inline ExtraField* GetExtra(int magic)
    {
        return extras_[MagicToIndex(magic)];
    }

protected:
    inline int MagicToIndex(int magic)
    {
        return magic / 67 % 7;
    }

    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    // Info
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);

private:
    ExtraField* extras_[7];
};

}
}

#endif /* NEST_FORMAT_EGG_EXTRAFIELDWRAP_H_ */
