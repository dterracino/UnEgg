//------------------------------------------------------------------------------
// File: unegg.h
//
// Desc: EGG & ALZ
//
// Author: Revolution
//
// ChangeLog
//     12/15/2008: Created
//
// Copyright (c) ESTsoft Corporation. All rights reserved.
//------------------------------------------------------------------------------

#ifndef UNEGG_H
#define UNEGG_H

typedef size_t egg_handle;
typedef int egg_type;

const egg_type EGG_TYPE_INVALID = 0;
const egg_type EGG_TYPE_ALZ = 1;
const egg_type EGG_TYPE_EGG = 2;

const UINT EGG_ERROR_SUCCESS = 1;
const UINT EGG_ERROR_FAIL = 0;
const UINT EGG_ERROR_SKIP = 2;
const UINT EGG_ERROR_CANCEL = 3;
const UINT EGG_ERROR_IO = 4;
const UINT EGG_ERROR_TYPEMISMATCH = 5;
const UINT EGG_ERROR_FILE = 6;
const UINT EGG_ERROR_FORMAT = 7;
const UINT EGG_ERROR_ALGORITHM = 8;
const UINT EGG_ERROR_WRONGPASSWORD = 9;

const UINT EGG_WORKCODE_NONE = 0;
const UINT EGG_WORKCODE_SCAN = 1;
const UINT EGG_WORKCODE_EXTRACT = 2;
const UINT EGG_WORKCODE_TEST = 3;

namespace eggevent
{
    struct Extract
    {
        static const UINT CODE = 1;

        size_t index;
        LPCTSTR nameInArchive;
        TCHAR fileName[MAX_PATH];
    };

    struct Progress
    {
        static const UINT CODE = 2;
        short total;
        short current;
    };

    struct Start
    {
        static const UINT CODE = 3;
        UINT code;
    };

    struct Finish
    {
        static const UINT CODE = 4;
        UINT code;
        UINT result;
    };

    struct ActivityStart
    {
        static const UINT CODE = 5;
        size_t index;
        UINT code;
    };

    struct ActivityFinish
    {
        static const UINT CODE = 6;
        size_t index;
        UINT code;
        UINT result;
    };

    struct QueryPassword
    {
        static const UINT CODE = 7;
        size_t index;
        size_t queryCount;
        TCHAR password[1024];
    };

    struct Error
    {
        static const UINT CODE = 8;
        size_t index;
        UINT code;
    };

    struct Test
    {
        static const UINT CODE = 9;
        size_t index;
        LPCTSTR nameInArchive;
        BOOL skipThisFile;
    };

    typedef UINT (__cdecl *Proc)(void* user, UINT code, void* param);
};

struct EGGFileHeader
{
    size_t size;
    LPCTSTR fileName;
    LPCTSTR comment;
    unsigned __int64 packedSize;
    unsigned __int64 unpackedSize;
    unsigned int attribute;
    unsigned int crc;
    unsigned int method;
    time_t lastModified;
    BOOL encrypted;
};

typedef UINT (__cdecl *EGGFunction_CreateEgg)(egg_handle* handle, eggevent::Proc eventProc, void* user);
typedef UINT (__cdecl *EGGFunction_DestroyEgg)(egg_handle* handle);
typedef UINT (__cdecl *EGGFunction_IsValidArchive)(const TCHAR* fileName, egg_type* result);
typedef UINT (__cdecl *EGGFunction_OpenArchive)(egg_handle handle, const TCHAR* fileName);
typedef UINT (__cdecl *EGGFunction_CloseArchive)(egg_handle handle);
typedef UINT (__cdecl *EGGFunction_Extract)(egg_handle handle);
typedef UINT (__cdecl *EGGFunction_Test)(egg_handle handle);
typedef UINT (__cdecl *EGGFunction_GetFileCount)(egg_handle handle, size_t* count);
typedef UINT (__cdecl *EGGFunction_GetFileHeader)(egg_handle handle, size_t index, EGGFileHeader* header);

#endif /* UNEGG_H */
