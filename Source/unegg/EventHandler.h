/*
 * EventHandler.h
 *
 *  Created on: 2011. 3. 25.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <string>
#include <nest/Events.h>
#include <nest/Info.h>

#ifndef __MINGW32__
#include <sys/ioctl.h>
#else
struct winsize
{
    unsigned short ws_row; /* rows, in characters */
    unsigned short ws_col; /* columns, in characters */
    unsigned short ws_xpixel; /* horizontal size, pixels */
    unsigned short ws_ypixel; /* vertical size, pixels */
};
#endif

class EventHandler: public nest::Events
{
public:
    EventHandler(winsize* console);
    virtual ~EventHandler();

    void SetDisplayLevel(int vLevel);
    void SetRedirect(bool redirect);

    virtual int Initialize(nest::Workspace* sender);
    virtual void Finalize(int res);

    virtual int OnWorkStart(int workCode);
    virtual void OnWorkFinish(int workCode, int res);

    virtual int OnFileStart(size_t index);
    virtual void OnFileFinish(size_t index, int res);

    virtual int OnError(size_t index, int errorCode);
    virtual int OnQueryInputStream(size_t index, nest::InputStream** stream,
            const char* virtualName, size_t queryCount);
    virtual int OnQueryOutputStream(size_t index, nest::OutputStream** stream,
            const char* virtualName);
    virtual int OnQueryPassword(size_t index, char* buffer, size_t size,
            size_t queryCount);
    virtual int OnProgress(size_t index, int current, int max);

private:
    void PrintIndent(size_t size);

    int vLevel_;
    bool redirect_;
    nest::Workspace* sender_;
    int overwrite_;
    std::string filename_;
    std::string archiveName_;
    nest::Info* info_;
    winsize* console_;
};

#endif /* EVENTHANDLER_H_ */
