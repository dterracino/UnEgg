#ifndef UNEGGOBJECT_H
#define UNEGGOBJECT_H

#include <atlstr.h>
#include <vector>
#include "unegg.h"
#include "UnEGGModule.h"

struct PasswordInfo
{
    PasswordInfo()
    {
        index = size_t(-1);
        memset(password, 0, MAX_PATH * sizeof(TCHAR));
    }

    size_t index;
    TCHAR password[MAX_PATH];
};

class UnEGGObject
{
public:
    UnEGGObject(void);
    virtual ~UnEGGObject(void);

    UINT Create(void);
    UINT Destroy(void);

    UINT Cancel(void);
    UINT OpenArchive(LPCTSTR path);
    UINT CloseArchive(void);
    UINT Extract(LPCTSTR path);
    size_t GetFileCount(void);
    UINT GetFileHeader(size_t index, EGGFileHeader* header);
    inline bool IsOpened(void)
    {
        return opened_;
    }
    inline std::vector<bool>& GetExtractingPart(void)
    {
        return extractingPart_;
    }

    static UnEGGModule* module;

protected:
    virtual UINT OnProgress(short total, short current);
    virtual UINT OnExtract(size_t index, LPCTSTR nameInArchive, TCHAR filename[MAX_PATH]);
    virtual UINT OnError(int code);
    virtual UINT OnQueryPassword(PasswordInfo* info);

private:
    friend UINT __cdecl _EGG_EventProc(void* user, UINT code, void* param);

    CString path_;
    egg_handle handle_;
    bool canceled_;
    bool opened_;
    std::vector<bool> extractingPart_;
};

#endif /* UNEGGOBJECT_H */
