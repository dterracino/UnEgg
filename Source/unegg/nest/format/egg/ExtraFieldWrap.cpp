/*
 * ExtraFieldWrap.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "ExtraFieldWrap.h"
#include <nest/format/egg/SplitField.h>
#include <nest/format/egg/SolidField.h>
#include <nest/format/egg/EncryptField.h>
#include <nest/format/egg/FilenameField.h>
#include <nest/format/egg/CommentField.h>
#include <nest/format/egg/WindowsField.h>
#include <nest/format/egg/PosixField.h>
#include <nest/format/egg/SplitField.h>
#include <nest/format/egg/EggTypes.h>

namespace nest
{
namespace egg
{

ExtraFieldWrap::ExtraFieldWrap()
{
    // TODO Auto-generated constructor stub
    memset(extras_, 0, sizeof(extras_));
}

ExtraFieldWrap::~ExtraFieldWrap()
{
    // TODO Auto-generated destructor stub
    Clear();
}

void ExtraFieldWrap::OnCleanUp()
{
    for (size_t i = 0; i < sizeof(extras_) / sizeof(void*); ++i)
    {
        delete extras_[i];
        extras_[i] = NULL;
    }
}

int ExtraFieldWrap::OnRead(InputStreamRewinder* rewinder, InputStream* stream)
{
    assert(rewinder && stream);

    int res = Result::Invalid;

    for (size_t i = 0; i < MAX_HEADERS; ++i)
    {
        ExtraField* eh = new ExtraField();
        res = eh->Read(stream);
        if (res == Result::Success && eh->GetSignature() != Magic::END)
        {
            switch (eh->GetSignature())
            {
            case Magic::SPLIT:
            case Magic::SOLID:
            case Magic::ENCRYPT:
            case Magic::FILENAME:
            case Magic::COMMENT:
            case Magic::WINDOWS_FILEINFO:
            case Magic::POSIX_FILEINFO:
                extras_[MagicToIndex(eh->GetSignature())] = eh;
                break;
            default:
                delete eh;
            }
        }
        else
        {
            if (eh->GetSignature() == Magic::END)
            {
                rewinder->Clear();
                res = Result::Success;
            }

            delete eh;
            break;
        }
    }

    return res;
}

int ExtraFieldWrap::_As4Bytes(unsigned int key, bool* handled)
{
    CommentField* cfield = (CommentField*)extras_[MagicToIndex(Magic::COMMENT)];
    EncryptField* efield = (EncryptField*)extras_[MagicToIndex(Magic::ENCRYPT)];
    FilenameField* ffield = (FilenameField*)extras_[MagicToIndex(Magic::FILENAME)];
    PosixField* pfield = (PosixField*)extras_[MagicToIndex(Magic::POSIX_FILEINFO)];
    WindowsField* wfield = (WindowsField*)extras_[MagicToIndex(Magic::WINDOWS_FILEINFO)];
    SplitField* sfield = (SplitField*)extras_[MagicToIndex(Magic::SPLIT)];

    VERIFY_KEY(key::file::IS_ENCRYPTED, efield ? 1 : 0);

    if (cfield)
    {
        VERIFY_KEY(key::file::comment::LOCALE, cfield->GetLocale());
        VERIFY_KEY(key::file::comment::IS_UTF8, cfield->IsUTF8());
        VERIFY_KEY(key::file::comment::IS_ENCRYPTED, cfield->IsEncrypted());
        VERIFY_KEY(key::archive::comment::LOCALE, cfield->GetLocale());
        VERIFY_KEY(key::archive::comment::IS_UTF8, cfield->IsUTF8());
        VERIFY_KEY(key::archive::comment::IS_ENCRYPTED, cfield->IsEncrypted());
    }

    if (efield)
    {
        VERIFY_KEY(key::file::encrypt::METHOD, efield->GetEncryptMethod());
        VERIFY_KEY(key::file::encrypt::CRC, efield->GetCRC());
        VERIFY_KEY(key::archive::encrypt::METHOD, efield->GetEncryptMethod());
        VERIFY_KEY(key::archive::encrypt::CRC, efield->GetCRC());
        VERIFY_KEY(key::block::encrypt::METHOD, efield->GetEncryptMethod());
        VERIFY_KEY(key::block::encrypt::CRC, efield->GetCRC());
    }

    if (ffield)
    {
        VERIFY_KEY(key::file::name::IS_ABSOLUTE, ffield->IsAbsolute());
        VERIFY_KEY(key::file::name::IS_ENCRYPTED, ffield->IsEncrypted());
        VERIFY_KEY(key::file::name::IS_UTF8, ffield->IsUTF8());
        VERIFY_KEY(key::file::name::LOCALE, ffield->GetLocale());
        VERIFY_KEY(key::file::name::PARENT_ID, ffield->GetParentPathID());
    }

    if (pfield)
    {
        VERIFY_KEY(key::file::attribute::POSIX_MODE, pfield->GetMode());
        VERIFY_KEY(key::file::attribute::UID, pfield->GetUID());
        VERIFY_KEY(key::file::attribute::GID, pfield->GetGID());
    }

    if (wfield)
    {
        VERIFY_KEY(key::file::attribute::ATTRIBUTES, wfield->GetAttribute());
    }

    if (sfield)
    {
        VERIFY_KEY(key::archive::volume::NEXT, sfield->GetNextFileID());
        VERIFY_KEY(key::archive::volume::PREV, sfield->GetPrevFileID());
    }

    return Header::_As4Bytes(key, handled);
}

int64 ExtraFieldWrap::_As8Bytes(unsigned int key, bool* handled)
{
    //CommentField* cfield = (CommentField*)extras_[MagicToIndex(Magic::COMMENT)];
    //EncryptField* efield = (EncryptField*)extras_[MagicToIndex(Magic::ENCRYPT)];
    //FilenameField* ffield = (FilenameField*)extras_[MagicToIndex(Magic::FILENAME)];
    PosixField* pfield = (PosixField*)extras_[MagicToIndex(Magic::POSIX_FILEINFO)];
    WindowsField* wfield = (WindowsField*)extras_[MagicToIndex(Magic::WINDOWS_FILEINFO)];
    SplitField* sfield = (SplitField*)extras_[MagicToIndex(Magic::SPLIT)];

    if (pfield)
    {
        VERIFY_KEY(key::file::attribute::MODIFIED, pfield->GetLastModifiedDateTime());
    }

    if (wfield)
    {
        VERIFY_KEY(key::file::attribute::MODIFIED, wfield->GetLastModifiedDateTime());
    }

    if (sfield)
    {
        VERIFY_KEY(key::archive::volume::NEXT, sfield->GetNextFileID());
        VERIFY_KEY(key::archive::volume::PREV, sfield->GetPrevFileID());
    }

    return Header::_As8Bytes(key, handled);
}

size_t ExtraFieldWrap::_AsSize(unsigned int key, bool* handled)
{
    //CommentField* cfield = (CommentField*)extras_[MagicToIndex(Magic::COMMENT)];
    //EncryptField* efield = (EncryptField*)extras_[MagicToIndex(Magic::ENCRYPT)];
    //FilenameField* ffield = (FilenameField*)extras_[MagicToIndex(Magic::FILENAME)];
    //PosixField* pfield = (PosixField*)extras_[MagicToIndex(Magic::POSIX_FILEINFO)];
    //WindowsField* wfield = (WindowsField*)extras_[MagicToIndex(Magic::WINDOWS_FILEINFO)];
    //SplitField* sfield = (SplitField*)extras_[MagicToIndex(Magic::SPLIT)];

    return Header::_AsSize(key, handled);
}

const void* ExtraFieldWrap::_AsPtr(unsigned int key, bool* handled)
{
    CommentField* cfield = (CommentField*)extras_[MagicToIndex(Magic::COMMENT)];
    EncryptField* efield = (EncryptField*)extras_[MagicToIndex(Magic::ENCRYPT)];
    FilenameField* ffield = (FilenameField*)extras_[MagicToIndex(Magic::FILENAME)];
    //PosixField* pfield = (PosixField*)extras_[MagicToIndex(Magic::POSIX_FILEINFO)];
    //WindowsField* wfield = (WindowsField*)extras_[MagicToIndex(Magic::WINDOWS_FILEINFO)];
    //SplitField* sfield = (SplitField*)extras_[MagicToIndex(Magic::SPLIT)];

    if (cfield)
    {
        VERIFY_KEY(key::archive::comment::COMMENT, cfield->GetComment());
        VERIFY_KEY(key::file::comment::COMMENT, cfield->GetComment());
    }

    if (efield)
    {
        VERIFY_KEY(key::archive::encrypt::VERIFY_DATA, efield->GetVerifyData());
        VERIFY_KEY(key::archive::encrypt::HEADER, efield->GetAESHeader());
        VERIFY_KEY(key::archive::encrypt::FOOTER, efield->GetAESFooter());
        VERIFY_KEY(key::file::encrypt::VERIFY_DATA, efield->GetVerifyData());
        VERIFY_KEY(key::file::encrypt::HEADER, efield->GetAESHeader());
        VERIFY_KEY(key::file::encrypt::FOOTER, efield->GetAESFooter());
    }

    if (ffield)
    {
        VERIFY_KEY(key::file::name::NAME, ffield->GetFileName());
    }

    return Header::_AsPtr(key, handled);
}

}
}
