/*
 * CommandLine.h
 *
 *  Created on: 2010. 12. 9.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include <set>
#include <vector>
#include <string>

class CommandLine
{
public:
    CommandLine(int argc, char* argv[]);
    virtual ~CommandLine();

    bool CommandExists(const char* c);
    size_t GetCommandCount();
    std::set<std::string>& GetCommands();
    size_t GetArgumentCount();
    const char* GetArgument(size_t index);

protected:
    void ParseCommandline(int argc, char* argv[]);

private:
    std::set<std::string> commands_;
    std::vector<std::string> args_;
};

#endif /* COMMANDLINE_H_ */
