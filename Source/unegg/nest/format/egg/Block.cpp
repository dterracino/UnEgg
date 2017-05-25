/*
 * Block.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Block.h"
#include <nest/format/egg/EggTypes.h>

namespace nest
{
namespace egg
{

Block::Block() :
    compMethod_(0), methodHint_(0), uncompSize_(0), compSize_(0), crc_(0)
{
    // TODO Auto-generated constructor stub

}

Block::~Block()
{
    // TODO Auto-generated destructor stub
    Clear();
}

int Block::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    int res = Result::ReadError;
    int sig = 0;

    IF_READ_STREAM_VAR(stream, sig)
    {
        if (sig == Magic::BLOCK)
        {

            IF_READ_STREAM_VAR(stream, compMethod_)
            {

                IF_READ_STREAM_VAR(stream, methodHint_)
                {

                    IF_READ_STREAM_VAR(stream, uncompSize_)
                    {

                        IF_READ_STREAM_VAR(stream, compSize_)
                        {

                            IF_READ_STREAM_VAR(stream, crc_)
                            {
                                rewinder->Clear();
                                dataOffset_ = stream->Tell();

                                res = Result::Success;
                            }
                        }
                    }
                }
            }
        }
    }

    return res;
}

int Block::_As4Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::block::size::PACKED, GetCompressedSize());
    VERIFY_KEY(key::block::size::UNPACKED, GetUncompressedSize());
    VERIFY_KEY(key::block::method::HINT, GetMethodHint());
    VERIFY_KEY(key::block::method::METHOD, GetCompressMethod());
    VERIFY_KEY(key::block::CRC, GetCRC());

    return Header::_As4Bytes(key, handled);
}

int64 Block::_As8Bytes(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::block::OFFSET, GetDataOffset());

    return Header::_As8Bytes(key, handled);
}

const char* Block::GetComressMethodString()
{
    switch (compMethod_)
    {
    case CompressionMethod::Store:
        return "STORE";
    case CompressionMethod::Deflate:
        return "DEFLATE";
    case CompressionMethod::bzip:
        return "BZIP";
    case CompressionMethod::lzma:
        return "LZMA";
    case CompressionMethod::azo:
        return "AZO";
    }

    return "";
}

const void* Block::_AsPtr(unsigned int key, bool* handled)
{
    VERIFY_KEY(key::block::method::NAME, GetComressMethodString());

    return Header::AsPtr(key, handled);
}

}
}
