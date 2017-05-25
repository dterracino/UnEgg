/*
 * FileWrap.cpp
 *
 *  Created on: 2010. 12. 13.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "FileWrap.h"
#include <nest/format/egg/EggTypes.h>

namespace nest
{
namespace egg
{

FileWrap::FileWrap(bool isSolid) :
    isSolid_(isSolid)
{
    // TODO Auto-generated constructor stub

}

FileWrap::~FileWrap()
{
    // TODO Auto-generated destructor stub
    Clear();
}

void FileWrap::OnCleanUp()
{
    file_.Clear();
    extras_.Clear();

    if (blocks_.size())
    {
        for (size_t i = 0; i < blocks_.size(); ++i)
        {
            delete blocks_[i];
        }
        blocks_.clear();
    }
}

int FileWrap::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = file_.Read(stream);

    if (res == Result::Success)
    {
        res = extras_.Read(stream);

        if (res == Result::Success)
        {
            if (isSolid_)
            {
                rewinder->Clear();
            }
            else
            {
                for (size_t i = 0; i < MAX_BLOCKS; ++i)
                {
                    BlockWrap* bh = new BlockWrap();
                    res = bh->Read(stream);

                    switch (res)
                    {
                    case Result::End:
                        rewinder->Clear();
                        delete bh;
                        return Result::Success;
                    case Result::Success:
                        blocks_.push_back(bh);
                        stream->Seek(bh->GetBlock().GetCompressedSize(), SEEK_CUR);
                        break;
                    default:
                        delete bh;
                        return res;
                    }
                }
            }
        }
    }
    else
    {
        res = Result::End;
    }

    return res;
}

int FileWrap::_As4Bytes(unsigned int key, bool* handled)
{
    if (blocks_.size())
    {
        VERIFY_KEY(key::file::CRC, blocks_[0]->AsInt32(key::block::CRC, handled));
    }

    bool handle = false;
    int res = file_.AsInt32(key, &handle);
    if (!handle)
    {
        res = extras_.AsInt32(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

int64 FileWrap::_As8Bytes(unsigned int key, bool* handled)
{
    if (key == key::file::size::PACKED)
    {
        uint64 size = 0;
        for (size_t i = 0; i < blocks_.size(); ++i)
        {
            size += blocks_[i]->AsUInt32(key::block::size::PACKED);
        }
        if (handled)
        {
            *handled = true;
        }

        return size;
    }

    bool handle = false;
    int64 res = file_.AsInt64(key, &handle);
    if (!handle)
    {
        res = extras_.AsInt64(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

size_t FileWrap::_AsSize(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::file::BLOCK_COUNT, blocks_.size());

    bool handle = false;
    int64 res = file_.AsInt64(key, &handle);
    if (!handle)
    {
        res = extras_.AsInt64(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

const void* FileWrap::_AsPtr(unsigned int key, bool* handled)
{
    if (blocks_.size())
    {
        VERIFY_KEY(key::file::method::NAME, blocks_[0]->AsPtr(key::block::method::NAME, handled));
    }

    bool handle = false;
    const void* res = file_.AsPtr(key, &handle);
    if (!handle)
    {
        res = extras_.AsPtr(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

Info* FileWrap::_AsInfo(unsigned int key, bool* handled)
{
    bool handle = false;
    Info* res = file_.AsInfo(key, &handle);
    if (!handle)
    {
        res = extras_.AsInfo(key, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

Info* FileWrap::_AsChildInfo(unsigned int key, size_t index, bool* handled)
{
    VERIFY_KEY(key::block::OBJECT, blocks_[index]);

    bool handle = false;
    Info* res = file_.AsChildInfo(key, index, &handle);
    if (!handle)
    {
        res = extras_.AsChildInfo(key, index, &handle);
    }
    if (handled)
    {
        *handled = handle;
    }
    assert(handled || handle);

    return res;
}

}
}
