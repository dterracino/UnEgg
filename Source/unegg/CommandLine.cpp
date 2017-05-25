/*
 * CommandLine.cpp
 *
 *  Created on: 2010. 12. 9.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "CommandLine.h"
#include <nest/Utils.h>

CommandLine::CommandLine(int argc, char* argv[])
{
    ParseCommandline(argc, argv);
}

CommandLine::~CommandLine()
{
    // TODO Auto-generated destructor stub
}

void CommandLine::ParseCommandline(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            commands_.insert(std::string(&argv[i][1]));
        }
        else
        {
            std::string arg = argv[i];
            args_.push_back(arg);
        }
    }
}

bool CommandLine::CommandExists(const char* c)
{
    return commands_.find(std::string(c)) != commands_.end();
}

size_t CommandLine::GetCommandCount()
{
    return commands_.size();
}

std::set<std::string>& CommandLine::GetCommands()
{
    return commands_;
}

size_t CommandLine::GetArgumentCount()
{
    return args_.size();
}

const char* CommandLine::GetArgument(size_t index)
{
    return args_[index].c_str();
}
