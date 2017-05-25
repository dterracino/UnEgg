/*
 * StreamRewinder.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_STREAM_STREAMREWINDER_H_
#define NEST_STREAM_STREAMREWINDER_H_

#include <stdio.h>
#include <vector>
#include <nest/stream/InputStream.h>

namespace nest
{

class InputStreamRewinder
{
public:

    InputStreamRewinder(InputStream& stream) :
        stream_(&stream)
    {
        Push();
    }

    InputStreamRewinder(InputStream* stream) :
        stream_(stream)
    {
        Push();
    }

    virtual ~InputStreamRewinder()
    {
        if (stack_.size())
        {
            stream_->Seek(stack_[0], SEEK_SET);
        }
    }

    inline void Push(void)
    {
        stack_.push_back(stream_->Tell());
    }

    inline void Pop(void)
    {
        stream_->Seek(stack_.back(), SEEK_SET);
        stack_.pop_back();
    }

    inline void Clear(void)
    {
        stack_.clear();
    }

    inline bool IsEmpty()
    {
        return stack_.empty();
    }

private:
    std::vector<uint64> stack_;
    InputStream* stream_;
};

}

#endif /* NEST_STREAM_STREAMREWINDER_H_ */
