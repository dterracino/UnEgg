/*
 * InputStreamLinker.h
 *
 *  Created on: 2011. 5. 27.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_STREAM_INPUTSTREAMLINKER_H
#define	NEST_STREAM_INPUTSTREAMLINKER_H

#include <vector>
#include <string>
#include <nest/nest.h>
#include <nest/stream/InputStream.h>

namespace nest
{

class InputStreamLinker: public InputStream
{
public:
    InputStreamLinker();
    virtual ~InputStreamLinker();

    virtual int Open();
    virtual void Close();
    virtual void Finalize();

    virtual uint64 Seek(uint64 pos, int origin);
    virtual uint64 Tell();
    virtual size_t Read(void* buf, size_t size);
    virtual uint64 Size();

    int PushStream(InputStream* stream, uint64 offset = 0, uint64 size = NOT_SET64);
    int PopStream();
    int PopAll();

    struct StreamBlock
    {
        InputStream* stream;
        uint64 offset;
        uint64 size;
        uint64 spos;
        uint64 epos;
    };

    StreamBlock* GetBlock(size_t index);
    size_t GetBlockCount();
    int UpdateStreams();

    static const size_t NOT_SET = (size_t) (-1);
    static const uint64 NOT_SET64 = (uint64) (-1);

protected:
    size_t PositionToIndex(uint64 pos);
    int OpenInternal(size_t index);

private:
    uint64 position_;
    size_t curBlock_;
    std::vector<StreamBlock> blocks_;
};

}

#endif	/* NEST_STREAM_INPUTSTREAMLINKER_H */

