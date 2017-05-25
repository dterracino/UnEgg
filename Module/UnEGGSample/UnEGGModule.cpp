#include "StdAfx.h"
#include "UnEGGModule.h"

UnEGGModule::UnEGGModule(void)
: module_(NULL)
{
    memset(&func_, 0, sizeof(func_));
}

UnEGGModule::~UnEGGModule(void)
{
    Finalize();
}

UnEGGModule::operator bool (void)
{
    return module_ != 0;
}

int UnEGGModule::Initialize(LPCTSTR path)
{
    int res = 0;

    Finalize();
    module_ = LoadLibrary(path);
    if (module_)
    {
        func_.createEgg = (EGGFunction_CreateEgg)GetProcAddress(module_, "EGG_CreateEgg");
        func_.destroyEgg = (EGGFunction_DestroyEgg)GetProcAddress(module_, "EGG_DestroyEgg");
        func_.openArchive = (EGGFunction_OpenArchive)GetProcAddress(module_, "EGG_OpenArchive");
        func_.closeArchive = (EGGFunction_CloseArchive)GetProcAddress(module_, "EGG_CloseArchive");
        func_.isValidArchive = (EGGFunction_IsValidArchive)GetProcAddress(module_, "EGG_IsValidArchive");
        func_.extract = (EGGFunction_Extract)GetProcAddress(module_, "EGG_Extract");
        func_.getFileCount = (EGGFunction_GetFileCount)GetProcAddress(module_, "EGG_GetFileCount");
        func_.getFileHeader = (EGGFunction_GetFileHeader)GetProcAddress(module_, "EGG_GetFileHeader");

        res = 1;
    }

    return res;
}

void UnEGGModule::Finalize(void)
{
    if (module_)
    {
        memset(&func_, 0, sizeof(func_));
        FreeLibrary(module_);
        module_ = NULL;
    }
}

int UnEGGModule::IsValidArchive(LPCTSTR path)
{
    egg_type res = EGG_TYPE_INVALID;
    if (module_ && func_.isValidArchive)
    {
        func_.isValidArchive(path, &res);
    }

    return res;
}
