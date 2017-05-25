/*
 * EncryptField.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_ENCRYPTFIELD_H_
#define NEST_FORMAT_EGG_ENCRYPTFIELD_H_

#include <nest/format/egg/ExtraField.h>

namespace nest
{
namespace egg
{

class EncryptField: public ExtraField
{
public:
    inline unsigned char GetEncryptMethod()
    {
        return *GetDataPtr();
    }

    inline const char* GetVerifyData()
    {
        return GetDataPtr(1);
    }

    inline int GetCRC()
    {
        return *(int*) GetDataPtr(13);
    }

    inline const char* GetAESHeader()
    {
        switch (GetEncryptMethod())
        {
        default:
            return NULL;
        case 1:
        case 2:
            return GetDataPtr(1);
        }
    }

    inline const char* GetAESFooter()
    {
        switch (GetEncryptMethod())
        {
        default:
            return NULL;
        case 1:
            return GetDataPtr(11);
        case 2:
            return GetDataPtr(19);
        }
    }
};

}
}

#endif /* NEST_FORMAT_EGG_ENCRYPTFIELD_H_ */
