/*
 * Events.h
 *
 *  Created on: 2011. 5. 26.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_EVENTS_H
#define	NEST_EVENTS_H

#include <nest/nest.h>

namespace nest
{

class InputStream;
class OutputStream;
class Workspace;

class Events
{
public:
    virtual ~Events() {};
    virtual int Initialize(Workspace* sender) = 0;
    virtual void Finalize(int res) = 0;

    virtual int OnWorkStart(int workCode) = 0;
    virtual void OnWorkFinish(int workCode, int res) = 0;

    virtual int OnFileStart(size_t index) = 0;
    virtual void OnFileFinish(size_t index, int res) = 0;

    virtual int OnError(size_t index, int errorCode) = 0;
    virtual int OnQueryInputStream(size_t index, InputStream** stream, const char* virtualName, size_t queryCount) = 0;
    virtual int OnQueryOutputStream(size_t index, OutputStream** stream, const char* virtualName) = 0;
    virtual int OnQueryPassword(size_t index, char* buffer, size_t size, size_t queryCount) = 0;
    virtual int OnProgress(size_t index, int current, int max) = 0;
};

}

#endif	/* NEST_EVENTS_H */

