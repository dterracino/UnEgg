/*
 * EventHandler.cpp
 *
 *  Created on: 2011. 3. 25.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EventHandler.h"
#include <nest/nest.h>
#include <nest/format/EggFormat.h>
#include <nest/Workspace.h>
#include <nest/Utils.h>
#include <unistd.h>

using namespace nest;

EventHandler::EventHandler(winsize* console) :
    vLevel_(0), redirect_(false), console_(console)
{
    // TODO Auto-generated constructor stub

}

EventHandler::~EventHandler()
{
    // TODO Auto-generated destructor stub
}

void EventHandler::SetDisplayLevel(int vLevel)
{
    vLevel_ = vLevel;
}

void EventHandler::SetRedirect(bool redirect)
{
    redirect_ = redirect;
}

int EventHandler::Initialize(Workspace* sender)
{
    overwrite_ = false;
    sender_ = sender;
    archiveName_ = sender->GetFilename();
    info_ = sender->GetInfo();

    return Result::Success;
}

void EventHandler::Finalize(int res)
{
    printf("\n");

    if (info_->AsBool(key::archive::IS_SOLID) && res == Result::InvalidCRC)
    {
        printf("\nSolid CRC is broken.\n");
    }

    sender_ = NULL;
    info_ = NULL;
    filename_.clear();
    archiveName_.clear();
}

int EventHandler::OnWorkStart(int workCode)
{
    if (vLevel_ > 1)
    {
        switch (workCode)
        {
        case WorkCode::Open:
            PrintIndent(sender_->GetWorkStack().size());
            printf("Opening archive \"%s\".\n", sender_->GetFilename());
            break;
        case WorkCode::Scan:
            PrintIndent(sender_->GetWorkStack().size());
            printf("Scanning files in \"%s\".\n", sender_->GetFilename());
            break;
        case WorkCode::Extract:
            PrintIndent(sender_->GetWorkStack().size());
            printf("Extracting files in \"%s\".\n", sender_->GetFilename());
            break;
        }
    }

    return Result::Success;
}

void EventHandler::OnWorkFinish(int workCode, int res)
{
    if (vLevel_ > 1)
    {
        switch (workCode)
        {
        case WorkCode::Open:
            PrintIndent(sender_->GetWorkStack().size());
            printf("Finish opening.\n");
            break;
        case WorkCode::Scan:
            PrintIndent(sender_->GetWorkStack().size());
            printf("Finish scanning.\n");
            break;
        case WorkCode::Extract:
            PrintIndent(sender_->GetWorkStack().size());
            printf("Finish extracting.\n");
            break;
        }
    }
}

int EventHandler::OnFileStart(size_t index)
{
    Info* info = info_->AsChildInfo(key::file::OBJECT, index);

    filename_ = info->AsString(key::file::name::NAME);

    if (sender_->GetLastWork() != WorkCode::Extract)
    {
        if (vLevel_ > 1)
        {
            printf("Found - %s\n", utf8::path_ellipsis(console_->ws_col - 9, filename_.c_str()));
        }
    }
    else
    {
        if (redirect_)
        {
            printf("%s ", filename_.c_str());
        }
        else
        {
            printf("  0 %% - %s", utf8::path_ellipsis(console_->ws_col - 10, filename_.c_str()));
        }
    }

    return Result::Success;
}

void EventHandler::OnFileFinish(size_t index, int res)
{
    if (sender_->GetLastWork() == WorkCode::Extract)
    {
        switch (res)
        {
        case Result::Success:
            if (!info_->AsBool(key::archive::IS_SOLID))
            {
                printf(" - CRC ok.\n");
            }
            else
            {
                printf("\n");
            }
            break;
        case Result::Skip:
            printf(" - Skipped by user.\n");
            break;
        case Result::Cancel:
            printf(" - Canceled by user.\n");
            break;
        case Result::InvalidCRC:
            printf(" - CRC error.\n");
            break;
        default:
            printf(" - error.\n");
        }
    }
    else
    {
//        if (vLevel_ > 1)
//        {
//            printf("\n");
//        }
    }
}

int EventHandler::OnError(size_t index, int errorCode)
{
    int res = errorCode;

    std::string msg;
    switch (errorCode)
    {
    case Result::Skip:
        msg = "Skipping by user.";
        break;
    case Result::Cancel:
        msg = "Canceled by user.";
        break;
    case Result::OpenError:
        msg = "Failed to open file.";
        break;
    case Result::ReadError:
        msg = "Reading I/O error.";
        break;
    case Result::WriteError:
        msg = "Writing I/O error.";
        break;
    case Result::Unsupported:
        msg = "Not supported.";
        break;
    case Result::NotReady:
        msg = "Not ready.";
        break;
    case Result::InvalidCRC:
        msg = "CRC error.";
        break;
    case Result::InvalidRequest:
        msg = "Invalid request.";
        break;
    case Result::SolidError:
        msg = "Solid stream error";
        break;
    case Result::InsufficientBlock:
        msg = "Insufficient blocks.";
        break;
        //    case Invalid:
    default:
        msg = "Undefined error.";
        break;

    }
    //printf("%s\n", msg.c_str());

    return res;
}

int EventHandler::OnQueryOutputStream(size_t index, OutputStream** stream, const char* virtualName)
{
    int res = Result::Success;
    bool exists = 0 == access(virtualName, F_OK);

    if (exists)
    {
        switch (overwrite_)
        {
        case 0:
        {
            retry: printf("\n");
            printf("\"%s\" File already exists. Overwrite?\n(Yes[Y]/No[N]/Yes All[A]/No All[O]/Cancel[C])", filename_.c_str());

            char answer[1024];
            if (fgets(answer, 1024, stdin))
            {
                switch (answer[0])
                {
                case 'Y':
                case 'y':
                    break;
                case 'N':
                case 'n':
                    res = Result::Skip;
                    break;
                case 'A':
                case 'a':
                    overwrite_ = 1;
                    res = Result::Success;
                    break;
                case 'O':
                case 'o':
                    overwrite_ = 2;
                    res = Result::Skip;
                    break;
                case 'C':
                case 'c':
                    res = Result::Cancel;
                    break;
                default:
                    goto retry;
                }
            }
            else
            {
                goto retry;
            }
            printf("\n");
        }
            break;
        case 1:
            res = Result::Success;
            break;
        case 2:
            res = Result::Skip;
            break;
        }
    }

    return res;
}

int EventHandler::OnQueryPassword(size_t index, char* buffer, size_t size, size_t queryCount)
{
    printf("\nEnter password: ");
    if (fgets(buffer, static_cast<unsigned int> (size), stdin))
    {
        size_t len = strlen(buffer);
        while (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = 0;
            --len;
        }

        return Result::Success;
    }
    return Result::Fail;
}

int EventHandler::OnQueryInputStream(size_t index, InputStream** stream, const char* virtualName, size_t queryCount)
{
    return queryCount > 3 ? Result::Fail : Result::Success;
}

int EventHandler::OnProgress(size_t index, int current, int max)
{
    if (!redirect_)
    {
        printf("\r");
        printf("%3d %% - %s", static_cast<int> (current * 100.f / max), utf8::path_ellipsis(console_->ws_col - 10, filename_.c_str()));
        fflush(stdout);
    }

    return Result::Success;
}

void EventHandler::PrintIndent(size_t size)
{
    if (vLevel_ > 1)
    {
        for (size_t i = 0; i < size - 1; ++i)
        {
            printf("\t");
        }
    }
}

