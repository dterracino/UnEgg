/*
 * FileWrap.h
 *
 *  Created on: 2010. 12. 13.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGG_FILEWRAP_H_
#define NEST_FORMAT_EGG_FILEWRAP_H_

#include <vector>
#include <nest/Info.h>
#include <nest/format/Header.h>
#include <nest/format/egg/ExtraFieldWrap.h>
#include <nest/format/egg/File.h>
#include <nest/format/egg/BlockWrap.h>
#include <nest/format/egg/CommentField.h>
#include <nest/format/egg/FilenameField.h>

namespace nest
{
namespace egg
{

class FileWrap : public Header
{
public:
    FileWrap(bool isSolid);
    virtual ~FileWrap();

    inline File& GetFileHeader()
    {
        return file_;
    }

    inline ExtraFieldWrap& GetExtras()
    {
        return extras_;
    }

    inline ExtraField* GetExtra(int magic)
    {
        return extras_.GetExtra(magic);
    }

    inline size_t GetBlockCount()
    {
        return blocks_.size();
    }

    inline BlockWrap* GetBlock(size_t index)
    {
        return blocks_[index];
    }

protected:
    virtual void OnCleanUp();
    virtual int OnRead(InputStreamRewinder* rewinder, InputStream* stream);

    // Info
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);
    virtual Info* _AsInfo(unsigned int key, bool* handled);
    virtual Info* _AsChildInfo(unsigned int key, size_t index, bool* handled);

private:
    File file_;
    ExtraFieldWrap extras_;
    std::vector<BlockWrap*> blocks_;
    bool isSolid_;
};

}
}

#endif /* NEST_FORMAT_EGG_FILEWRAP_H_ */

