#include "StdAfx.h"
#include "UnEGGObject.h"
#include <strsafe.h>
#include "UnEGGSampleDlg.h"

UnEGGModule* UnEGGObject::module = NULL;

UnEGGObject::UnEGGObject(void)
: handle_(NULL), opened_(false)
{
}

UnEGGObject::~UnEGGObject(void)
{
    Destroy();
}

UINT __cdecl _EGG_EventProc(void* user, UINT code, void* param)
{
    switch (code)
    {
    case eggevent::Extract::CODE:
        {
            UnEGGObject* obj = static_cast<UnEGGObject*>(user);
            eggevent::Extract* p = static_cast<eggevent::Extract*>(param);

            return obj->OnExtract(p->index, p->nameInArchive, p->fileName);
        }
        break;
    case eggevent::Progress::CODE:
        {
            UnEGGObject* obj = static_cast<UnEGGObject*>(user);
            eggevent::Progress* p = static_cast<eggevent::Progress*>(param);

            return obj->OnProgress(p->total, p->current);
        }
        break;
    case eggevent::Error::CODE:
        {
            UnEGGObject* obj = static_cast<UnEGGObject*>(user);
            eggevent::Error* p = static_cast<eggevent::Error*>(param);

            return obj->OnError(p->code);
        }
        break;
    case eggevent::QueryPassword::CODE:
        {
            UnEGGObject* obj = static_cast<UnEGGObject*>(user);
            eggevent::QueryPassword* p = static_cast<eggevent::QueryPassword*>(param);
            
            PasswordInfo info;
            info.index = p->index;

            UINT res = obj->OnQueryPassword(&info);
            _tcscpy_s(p->password, 1024, info.password);

            return res;
        }
        break;
    }

    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::Create(void)
{
    if (!handle_)
    {
        assert(module && *module);
        return module->GetFunc()->createEgg(&handle_, _EGG_EventProc, this);
    }
    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::Destroy(void)
{
    if (handle_)
    {
        assert(module && *module);
        return module->GetFunc()->destroyEgg(&handle_);
    }
    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::OpenArchive(LPCTSTR path)
{
    canceled_ = false;
    assert(module && *module && handle_);
    UINT r = module->GetFunc()->openArchive(handle_, path);
    if (r == EGG_ERROR_SUCCESS)
    {
        opened_ = true;
    }

    return r;
}

UINT UnEGGObject::CloseArchive(void)
{
    opened_ = false;
    if (handle_)
    {
        assert(module && *module);
        return module->GetFunc()->closeArchive(handle_);
    }
    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::Extract(LPCTSTR path)
{
    assert(module && *module && handle_);
    path_ = path;
    if (path_[path_.GetLength() - 1] != _T('\\'))
    {
        path_.Append(_T("\\"));
    }

    UINT res = module->GetFunc()->extract(handle_);
    extractingPart_.clear();

    return res;
}

size_t UnEGGObject::GetFileCount(void)
{
    assert(module && *module && handle_);
    size_t count = 0;
    module->GetFunc()->getFileCount(handle_, &count);

    return count;
}

UINT UnEGGObject::GetFileHeader(size_t index, EGGFileHeader* header)
{
    assert(module && *module && handle_);
    return module->GetFunc()->getFileHeader(handle_, index, header);
}

UINT UnEGGObject::OnProgress(short total, short current)
{
    if (canceled_)
    {
        return EGG_ERROR_CANCEL;
    }

    AfxGetMainWnd()->SendMessage(WM_SETPROG, total, current);

    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::OnExtract(size_t index, LPCTSTR nameInArchive, TCHAR filename[MAX_PATH])
{
    if (extractingPart_.size())
    {
        if (!extractingPart_[index])
        {
            return EGG_ERROR_SKIP;
        }
    }

    CString tmp = path_;
    tmp += nameInArchive;
    StringCchCopy(filename, MAX_PATH, tmp);

    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::OnError(int code)
{
    UNREFERENCED_PARAMETER(code);

    TRACE(_T("Error : %d\n"), code);

    return EGG_ERROR_SUCCESS;
}

UINT UnEGGObject::OnQueryPassword(PasswordInfo* info)
{
    LRESULT res = AfxGetMainWnd()->SendMessage(WM_QUERYPASSWORD, reinterpret_cast<WPARAM>(info));

    UINT result = EGG_ERROR_SUCCESS; // success

    switch (res)
    {
    case 1:
        result = EGG_ERROR_WRONGPASSWORD; // skip
        break;
    case 2:
        canceled_ = true;
        result = EGG_ERROR_CANCEL; // cancel
        break;
    default:
        break;
    }

    return result;
}
