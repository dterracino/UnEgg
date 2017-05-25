#ifndef UNEGGMODULE_H
#define UNEGGMODULE_H

#include "unegg.h"

class UnEGGModule
{
public:
    UnEGGModule(void);
    virtual ~UnEGGModule(void);

    operator bool (void);

    int Initialize(LPCTSTR path);
    void Finalize(void);

    struct Functions
    {
        EGGFunction_CreateEgg createEgg;
        EGGFunction_DestroyEgg destroyEgg;
        EGGFunction_OpenArchive openArchive;
        EGGFunction_CloseArchive closeArchive;
        EGGFunction_IsValidArchive isValidArchive;
        EGGFunction_Extract extract;
        EGGFunction_GetFileCount getFileCount;
        EGGFunction_GetFileHeader getFileHeader;
    };

    inline Functions* GetFunc(void)
    {
        return &func_;
    }

    egg_type IsValidArchive(LPCTSTR path);

private:
    HMODULE module_;
    Functions func_;
};

#endif /* UNEGGMODULE_H */
