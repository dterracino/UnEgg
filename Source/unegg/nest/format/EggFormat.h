/*
 * EggFormat.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_FORMAT_EGGFORMAT_H
#define	NEST_FORMAT_EGGFORMAT_H

#include <string>
#include <vector>
#include <nest/nest.h>
#include <nest/format/Format.h>
#include <nest/Events.h>
#include <nest/Info.h>
#include <nest/format/egg/ExtraFieldWrap.h>
#include <nest/format/egg/Global.h>
#include <nest/format/egg/FileWrap.h>
#include <nest/format/egg/BlockWrap.h>
#include <nest/format/egg/EncryptField.h>
#include <nest/algorithm/Coder.h>
#include <nest/encryption/Decoder.h>
#include <nest/stream/InputStreamLinker.h>

namespace nest
{

class EggFormat: public Format, Events, Info
{
public:
    EggFormat();
    EggFormat(Events* evnt);
    virtual ~EggFormat();

    // Format Implementation
    virtual int Open(InputStream* stream);
    virtual void Close();
    virtual int CheckHeader(InputStream* stream);
    virtual int Scan();
    virtual int Extract();

    virtual InputStream* GetInputStream();
    virtual Info* GetInfo();

protected:
    // Info Implementation
    virtual int _As4Bytes(unsigned int key, bool* handled);
    virtual int64 _As8Bytes(unsigned int key, bool* handled);
    virtual size_t _AsSize(unsigned int key, bool* handled);
    virtual const void* _AsPtr(unsigned int key, bool* handled);
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

    int CheckHeaderInternal(InputStream* stream, egg::Global* header);
    int ScanHeaders(InputStream* stream = NULL, egg::ExtraFieldWrap* header = NULL);
    int ScanFiles();
    int ScanFooters();

    int PrepareSpannedStreams(void);
    int ReadSolidBlocks();
    unsigned int CalculateDummySize(InputStream* stream);
    int GenerateVolumeName(size_t index, char* buffer, size_t size);

    int QueryExtractStream(size_t index, OutputStream** stream);
    int PrepareSolidBlock(egg::BlockWrap* block, BlockExtractInfo* info);
    bool IsTargetDir(egg::FileWrap* file);
    void GenerateCoder(int method, Coder** coder, size_t* dataHeaderSize);

    int ExtractNormalArchive();
    int ExtractSolidArchive();
    int ExtractIndividualFile(size_t index, OutputStream* stream);

    int PreprocessDecrypt(size_t index, egg::EncryptField* header);
    int PostprocessDecrypt(size_t index, egg::EncryptField* header);

    bool opened_;
    std::string password_;
    egg::ExtraFieldWrap headerExtras_;
    egg::ExtraFieldWrap footerExtras_;
    egg::Global globalHeader_;
    std::vector<egg::FileWrap*> files_;
    std::vector<egg::BlockWrap*> solidBlocks_;
    bool isSpanned_;
    bool isSolid_;
    Decoder* decoder_;
    InputStreamLinker istream_;
};

}

#endif	/* NEST_FORMAT_EGGFORMAT_H */

