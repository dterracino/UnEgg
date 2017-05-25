/*
 * InputStreamLinker.cpp
 *
 *  Created on: 2011. 5. 27.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "InputStreamLinker.h"
#include <assert.h>

namespace nest
{

InputStreamLinker::InputStreamLinker() :
    position_(0), curBlock_(NOT_SET)
{
}

InputStreamLinker::~InputStreamLinker()
{
    Finalize();
}

int InputStreamLinker::Open()
{
    int res = Result::Success;

    if (curBlock_ == NOT_SET)
    {
        position_ = 0;
        curBlock_ = NOT_SET;

        res = OpenInternal(0);
    }

    return res;
}

uint64 InputStreamLinker::Seek(uint64 pos, int origin)
{
    assert(curBlock_ != NOT_SET);

    uint64 newpos = 0;
    switch (origin)
    {
    case SEEK_SET:
        newpos = pos;
        break;
    case SEEK_CUR:
        newpos = position_ + pos;
        break;
    case SEEK_END:
        newpos = Size() - pos;
        break;
    }
    size_t newBlock = PositionToIndex(newpos);

    if (curBlock_ != newBlock)
    {
        OpenInternal(newBlock);
    }

    StreamBlock& block = blocks_[curBlock_];
    block.stream->Seek(newpos - block.spos + block.offset, SEEK_SET);
    position_ = newpos;

    return position_;
}

uint64 InputStreamLinker::Tell()
{
    assert(curBlock_ != NOT_SET);

    return position_;
}

size_t InputStreamLinker::Read(void* buf, size_t size)
{
    assert(curBlock_ != NOT_SET);

    unsigned char* p = static_cast<unsigned char*> (buf);
    size_t arb = 0;

    for (size_t i = curBlock_; i < blocks_.size() && size > 0; ++i)
    {
        OpenInternal(i);

        StreamBlock& block = blocks_[i];
        uint64 avail = block.epos - position_;

        if (avail > 0)
        {
            size_t rb = block.stream->Read(p, static_cast<unsigned int> (avail > size ? size : avail));
            size -= rb;
            p += rb;
            position_ += rb;
            arb += rb;
        }
    }

    return arb;
}

uint64 InputStreamLinker::Size()
{
    assert(curBlock_ != NOT_SET);

    return blocks_.size() ? blocks_.back().epos : 0;
}

int InputStreamLinker::PushStream(InputStream* stream, uint64 offset, uint64 size)
{
    if (stream)
    {
        StreamBlock block;
        block.stream = stream;
        block.offset = offset;

        if (size == NOT_SET64)
        {
            stream->Open();
            size = stream->Size();
        }
        block.size = size;
        stream->Close();

        block.spos = 0;
        if (blocks_.size())
        {
            block.spos += blocks_.back().epos;
        }
        block.epos = block.spos + size;

        blocks_.push_back(block);
    }

    return Result::Success;
}

int InputStreamLinker::PopStream()
{
    InputStream* stream = blocks_.back().stream;
    blocks_.pop_back();
    stream->Close();

    return UpdateStreams();
}

int InputStreamLinker::PopAll()
{
    for (size_t i = 0; i < blocks_.size(); ++i)
    {
        InputStream* stream = blocks_[i].stream;
        blocks_.pop_back();
        stream->Close();
    }
    position_ = 0;
    curBlock_ = NOT_SET;

    return Result::Success;
}

int InputStreamLinker::UpdateStreams()
{
    for (size_t i = 0; i < blocks_.size(); ++i)
    {
        blocks_[i].spos = i > 0 ? blocks_[i - 1].epos : 0;
        blocks_[i].epos = blocks_[i].spos + blocks_[i].size;
    }

    if (blocks_.size())
    {
        if (curBlock_ != NOT_SET)
        {
            if (curBlock_ >= blocks_.size())
            {
                position_ = 0;
                OpenInternal(0);
            }
            else
            {
                Seek(position_, SEEK_SET);
            }
        }
    }
    else
    {
        Close();
    }

    return Result::Success;
}

void InputStreamLinker::Close()
{
    position_ = 0;
    if (curBlock_ != NOT_SET)
    {
        blocks_[curBlock_].stream->Close();
    }
    curBlock_ = NOT_SET;
}

void InputStreamLinker::Finalize()
{
    Close();
    for (size_t i = 0; i < blocks_.size(); ++i)
    {
        InputStream* stream = blocks_[i].stream;
        stream->Close();
    }
    blocks_.clear();
    position_ = 0;
    curBlock_ = NOT_SET;

    InputStream::Finalize();
}

size_t InputStreamLinker::PositionToIndex(uint64 pos)
{
    StreamBlock& block = blocks_[curBlock_];
    if (block.spos > pos)
    {
        for (size_t i = 0; i < curBlock_; ++i)
        {
            StreamBlock& block = blocks_[i];
            if (block.spos <= pos && block.epos > pos)
            {
                return i;
            }
        }
    }
    else if (block.epos <= pos)
    {
        for (size_t i = curBlock_ + 1; i < blocks_.size(); ++i)
        {
            StreamBlock& block = blocks_[i];
            if (block.spos <= pos && block.epos > pos)
            {
                return i;
            }
        }
    }

    return curBlock_;
}

int InputStreamLinker::OpenInternal(size_t index)
{
    int res = Result::Success;

    if (curBlock_ != index)
    {
        if (curBlock_ != NOT_SET)
        {
            blocks_[curBlock_].stream->Close();
        }
        curBlock_ = index;
        StreamBlock& block = blocks_[index];
        block.stream->Open();
        block.stream->Seek(block.offset, SEEK_SET);
    }

    return res;
}

InputStreamLinker::StreamBlock* InputStreamLinker::GetBlock(size_t index)
{
    return &blocks_[index];
}

size_t InputStreamLinker::GetBlockCount()
{
    return blocks_.size();
}

}
