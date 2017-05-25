/*
 * Workspace.cpp
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Workspace.h"
#include <nest/stream/FileOutputStream.h>
#include <algorithm>

namespace nest
{

std::vector<FormatFactory*> Workspace::factoryList_;

int Workspace::AddFactory(FormatFactory* factory)
{
    factoryList_.push_back(factory);

    return Result::Success;
}

Workspace::Workspace()
: format_(NULL), event_(NULL)
{

}

Workspace::Workspace(Events* evnt)
: format_(NULL), event_(evnt)
{
}

Workspace::~Workspace()
{
    Close();
}

Events* Workspace::GetEvent()
{
    return event_;

}

void Workspace::SetEvent(Events* evnt)
{
    event_ = evnt;
}

int Workspace::Open(const char* filename)
{
    if (IsOpened())
    {
        Close();
    }

    filename_ = filename;
    InputStream* stream = new FileInputStream();
    streams_.push_back(stream);
    stream->Initialize(filename);
    stream->Open();

    int res = Result::Unsupported;
    for (size_t i = 0; i < factoryList_.size(); ++i)
    {
        FormatFactory* factory = factoryList_[i];
        if (factory)
        {
            stream->Seek(0, SEEK_SET);
            res = factory->CheckHeader(stream);

            if (res == Result::Success)
            {
                format_ = factory->CreateFormat();
                format_->SetEvent(this);
                stream->Seek(0, SEEK_SET);
                res = format_->Open(stream);

                if (res == Result::Success)
                {
                    res = format_->Scan();
                }
                break;
            }
        }
    }

    if (res != Result::Success)
    {
        Close();
    }

    return res == Result::Success ? res : Result::Unsupported;
}

void Workspace::Close()
{
    filename_.clear();

    if (format_)
    {
        format_->Close();
        delete format_;
        format_ = NULL;
    }

    for (size_t i = 0; i < streams_.size(); ++i)
    {
        InputStream* stream = streams_[i];
        stream->Close();
        stream->Finalize();
        delete stream;
    }
    streams_.clear();
}

int Workspace::Extract(const char* destination)
{
    int res = Result::NotReady;

    if (format_)
    {
        destination_ = destination;
        if (destination_[destination_.size() - 1] != '/')
        {
            destination_.push_back('/');
        }

        res = format_->Extract();
    }

    return res;
}

const char* Workspace::GetFilename()
{
    return filename_.c_str();
}

bool Workspace::IsOpened()
{
    return format_ != NULL;
}

uint64 Workspace::GetStreamPos()
{
    return format_ ? format_->GetInputStream()->Tell() : 0;
}

uint64 Workspace::GetStreamSize()
{
    return format_ ? format_->GetInputStream()->Size() : 0;
}

Info* Workspace::GetInfo()
{
    return format_ ? format_->GetInfo() : NULL;
}

int Workspace::GetLastWork()
{
    return work_.top();
}

std::stack<int>& Workspace::GetWorkStack()
{
    return work_;
}

int Workspace::Initialize(Workspace* sender)
{
    if (event_)
    {
        return event_->Initialize(this);
    }

    return Result::Success;
}

void Workspace::Finalize(int res)
{
    if (event_)
    {
        event_->Finalize(res);
    }
}

int Workspace::OnWorkStart(int workCode)
{
    int res = Result::Success;

    if (work_.empty())
    {
        res = Initialize(this);
    }

    if (res == Result::Success)
    {
        work_.push(workCode);

        if (event_)
        {
            res = event_->OnWorkStart(workCode);
        }
    }

    return res;
}

void Workspace::OnWorkFinish(int workCode, int res)
{
    if (event_)
    {
        event_->OnWorkFinish(workCode, res);
    }

    work_.pop();

    if (work_.empty())
    {
        Finalize(res);
    }
}

int Workspace::OnFileStart(size_t index)
{
    if (event_)
    {
        return event_->OnFileStart(index);
    }

    return Result::Success;
}

void Workspace::OnFileFinish(size_t index, int res)
{
    if (event_)
    {
        event_->OnFileFinish(index, res);
    }
}

int Workspace::OnError(size_t index, int errorCode)
{
    if (event_)
    {
        return event_->OnError(index, errorCode);
    }

    return errorCode;
}

int Workspace::OnQueryInputStream(size_t index, InputStream** stream, const char* virtualName, size_t queryCount)
{
    int res = Result::Fail;

    if (event_)
    {
        res = event_->OnQueryInputStream(index, stream, virtualName, queryCount);
    }

    if (res == Result::Success && !*stream)
    {
        *stream = new FileInputStream();
        (*stream)->Initialize(virtualName);
    }

    if (*stream)
    {
        streams_.push_back(*stream);
    }

    return res;
}

int Workspace::OnQueryOutputStream(size_t index, OutputStream** stream, const char* virtualName)
{
    int res = Result::Fail;
    std::string filename = destination_;
    filename.append("../../");
    filename.append(virtualName);

    size_t pos = filename.find("/../");
    while (pos != std::string::npos)
    {
        filename.replace(pos, 4, "/");
        pos = filename.find("/../");
    }
    pos = filename.find("../");
    while (pos != std::string::npos)
    {
        filename.replace(pos, 3, "/");
        pos = filename.find("../");
    }
    pos = filename.find("/..");
    while (pos != std::string::npos)
    {
        filename.replace(pos, 3, "/");
        pos = filename.find("/..");
    }

    if (event_)
    {
        res = event_->OnQueryOutputStream(index, stream, filename.c_str());
    }

    if (res == Result::Success && !*stream)
    {
        *stream = new FileOutputStream();
    }
    if (*stream)
    {
        (*stream)->Create(filename.c_str());
    }

    return res;
}

int Workspace::OnQueryPassword(size_t index, char* buffer, size_t size, size_t queryCount)
{
    if (event_)
    {
        return event_->OnQueryPassword(index, buffer, size, queryCount);
    }

    return Result::Fail;
}

int Workspace::OnProgress(size_t index, int current, int max)
{
    if (event_)
    {
        return event_->OnProgress(index, current, max);
    }

    return Result::Success;
}

}
