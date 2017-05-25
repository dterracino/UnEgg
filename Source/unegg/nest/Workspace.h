/*
 * Workspace.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_WORKSPACE_H
#define	NEST_WORKSPACE_H

#include <vector>
#include <string>
#include <stack>
#include <nest/nest.h>
#include <nest/Events.h>
#include <nest/stream/FileInputStream.h>
#include <nest/format/Format.h>
#include <nest/format/FormatFactory.h>

namespace nest
{

class Workspace: public Events
{
public:
    Workspace();
    Workspace(Events* evnt);
    virtual ~Workspace();

    void SetEvent(Events* evnt);
    Events* GetEvent();

    int Open(const char* filename);
    void Close();
    int Extract(const char* destination);

    const char* GetFilename();
    bool IsOpened();

    uint64 GetStreamPos();
    uint64 GetStreamSize();

    Info* GetInfo();

    int GetLastWork();
    std::stack<int>& GetWorkStack();

    static int AddFactory(FormatFactory* factory);

protected:
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
    static std::vector<FormatFactory*> factoryList_;

    std::vector<InputStream*> streams_;
    std::string filename_;
    std::string destination_;
    Format* format_;
    std::stack<int> work_;
    Events* event_;
};

}

#endif	/* NEST_WORKSPACE_H */

