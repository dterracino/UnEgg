/*
 * ALZFormat.h
 *
 *  Created on: 2011. 6. 3.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_ALZFORMAT_H_
#define NEST_FORMAT_ALZFORMAT_H_

#include <vector>
#include <nest/format/alz/ALZTypes.h>
#include <nest/stream/InputStreamLinker.h>
#include <nest/format/Format.h>
#include <nest/format/alz/Global.h>
#include <nest/format/alz/File.h>
#include <nest/algorithm/Coder.h>
#include <nest/encryption/Decoder.h>
#include <nest/Info.h>

namespace nest
{

class ALZFormat: public Format, public Info
{
public:
    ALZFormat();
    ALZFormat(Events* evnt);
    virtual ~ALZFormat();

    virtual int Open(InputStream* stream);
    virtual void Close();
    virtual int CheckHeader(InputStream* stream);
    virtual int Scan();
    virtual int Extract();
    virtual InputStream* GetInputStream();
    virtual Info* GetInfo();

protected:
    int CheckHeaderInternal(InputStream* stream, alz::Global* global);
    int ScanFiles();
    int ScanFooters();
    int GenerateVolumeName(size_t index, char* buffer, size_t size);
    int PrepareSpannedStreams();
    int QueryExtractStream(size_t index, OutputStream** stream);
    int ExtractIndividualFile(size_t index, OutputStream* stream);
    bool IsTargetDir(alz::File* file);
    int PreprocessDecrypt(size_t index, alz::File* header);
    int PostprocessDecrypt(size_t index, alz::File* header);
    void GenerateCoder(int method, Coder** coder, size_t* dataHeaderSize);

    // Info Implementation
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);
    virtual Info* _AsInfo(unsigned int key, bool* handled);
    virtual Info* _AsChildInfo(unsigned int key, size_t index, bool* handled);

    // Events Implementation
    virtual int Initialize(Workspace* sender);
    virtual void Finalize(int res);

    virtual int OnWorkStart(int workCode);
    virtual void OnWorkFinish(int workCode, int res);

    virtual int OnFileStart(size_t index);
    virtual void OnFileFinish(size_t index, int res);

    virtual int OnError(size_t index, int errorCode);
    virtual int OnQueryInputStream(size_t index, InputStream** stream, const char* virtualName, size_t queryCount);
    virtual int OnQueryOutputStream(size_t index, OutputStream** stream, const char* virtualName);
    virtual int OnQueryPassword(size_t index, char* buffer, size_t size, size_t queryCount);
    virtual int OnProgress(size_t index, int current, int max);

private:
    struct BlockExtractInfo
    {
        Coder* coder;
        size_t dataHeaderSize;
        unsigned int crc;
        uint64 rawDataRemained;
        uint64 dataRemained;
    };
    struct SpannedVolume
    {
        InputStream* stream;
        uint64 offset;
        uint64 size;
    };

    InputStreamLinker istream_;
    bool opened_;
    alz::Global global_;
    std::vector<alz::File*> files_;
    bool spanned_;
    std::string comment_;
    unsigned int crc_;
    Decoder* decoder_;
    std::string password_;
};

}

#endif /* NEST_FORMAT_ALZFORMAT_H_ */
