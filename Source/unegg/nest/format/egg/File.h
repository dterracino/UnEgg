/*
 * File.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_FILE_H_
#define NEST_FORMAT_EGG_FILE_H_

#include <nest/format/Header.h>

namespace nest
{
namespace egg
{

class File: public Header
{
public:
    File();
    virtual ~File();

    inline unsigned int GetID()
    {
        return id_;
    }

    inline uint64 GetLength()
    {
        return length_;
    }

protected:

    virtual void OnCleanUp()
    {
    }
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    // Info
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);

private:
    unsigned int id_;
    uint64 length_;
};

}
}

#endif /* NEST_FORMAT_EGG_FILE_H_ */
