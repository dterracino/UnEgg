/*
 * EventHandler.h
 *
 *  Created on: 2010. 9. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iostream>
#include <locale>
#include <math.h>

#ifndef __MINGW32__
#include <sys/ioctl.h>
#else
#include <io.h>
#endif

#include "CommandLine.h"
#include "EventHandler.h"
#include <nest/format/EggFormatFactory.h>
#include <nest/format/ALZFormatFactory.h>
#include <nest/Workspace.h>
#include <nest/Utils.h>

using namespace std;
using namespace nest;

int g_verbose = 1;
int g_disp = 0xF;
char g_delimiter = '|';
const char* g_filename = NULL;
winsize g_console;
EventHandler g_event(&g_console);
char g_lineBuffer[1024] = { 0, };

struct DisplayInfo
{
    const char* title;
    int length;
    const char* type;
};

const DisplayInfo HEADERS[] = { { "Name", 0, "s" }, { "Packed", 11, "llu" }, { "Unpacked", 11, "llu" }, { "Attr", 8, "u" }, { "Blocks",
        8, "zu" }, { "Encrypted", 11, "s" }, { "Algorithm", 11, "s" }, { "CRC", 11, "u" }, { "Comment", 11, "s" } };

void PrintProgramDetails(void)
{
    if (g_verbose > 0)
    {
        printf("\nunegg v0.5\n");
        printf("Copyright(c) 2010 - present ESTsoft Corp. All rights reserved.\n\n");
    }
}

void PrintUsage(void)
{
    printf("Usage : unegg [commands] [archive filename] [destination path].\n\n");
    printf("Available commands.\n");
    printf("-h\tDisplay help.\n");
    printf("-l\tDisplay file list in archive.\n");
    printf("-x\tExtract all files to destination path.\n");
    printf("-vX\tVerbose level X.\n");
    printf("-r\tDo not display progress for redirection.\n");
    printf("-dD\tArchive list display option.\n");
    printf("\tn\tDisplay file name.\n");
    printf("\tp\tDisplay packed size.\n");
    printf("\tu\tDisplay unpacked size.\n");
    printf("\ta\tDisplay file attributes.\n");
    printf("\tb\tDisplay block count.\n");
    printf("\te\tDisplay whether the file is encrypted.\n");
    printf("\tm\tDisplay compression method.\n");
    printf("\tr\tDisplay CRC value.\n");
    printf("\tc\tDisplay comment.\n");
}

void PrintError(int error)
{
    if (g_verbose > 0)
    {
        switch (error)
        {
        case Result::Cancel:
            printf("Canceled by user.\n");
            break;
        case Result::InvalidCRC:
            printf("CRC error.\n");
            break;
        default:
            printf("Error (%d)\n", error);
        }
    }
}

int PrintListOfArchive(const char* filename)
{
    Workspace archive(&g_event);

    int res = archive.Open(filename);
    Info* info = archive.GetInfo();

    if (res == Result::Success)
    {
        size_t headerCount = sizeof(HEADERS) / sizeof(DisplayInfo);
        size_t restOfLine = g_console.ws_col - 2;

        char splitter[g_console.ws_col];
        memset(splitter, 0, sizeof(splitter));
        memset(splitter, '-', g_console.ws_col - 1);

        if (g_verbose > 0)
        {
            printf("Global informations\n");
            printf("[\n");
            printf("\tFormat : %s\n", info->AsString(key::archive::format::NAME));
            printf("\tVersion : %d.%d\n", info->AsInt32(key::archive::version::MAJOR), info->AsInt32(key::archive::version::MINOR));
            bool isSolid = info->AsBool(key::archive::IS_SOLID);
            printf("\tSolid : %s\n", isSolid ? "yes" : "no");
            if (isSolid)
            {
                printf("\t[\n");
                printf("\t\tSolid Block Count : %zu\n", info->AsSize(key::archive::BLOCK_COUNT));
                printf("\t]\n");
            }
            printf("\tSpanned : %s\n", info->AsBool(key::archive::IS_SPANNED) ? "yes" : "no");
            printf("\tFile Count : %zu\n", info->AsSize(key::archive::FILE_COUNT));

            bool handle = false;
            const char* comment = info->AsString(key::archive::comment::COMMENT, &handle);
            if (comment)
            {
                printf("\tGlobal Comment : %s\n", comment);
            }
            else
            {
                printf("\tGlobal Comment : none\n");
            }
            printf("]\n\n");

            if (g_verbose < 2)
            {
                printf("%s\n", splitter);
                bool isFirst = true;
                for (size_t i = 1; i < headerCount; ++i)
                {
                    if (g_disp & ((int) pow(2, i)))
                    {
                        PRINTF2("%c%%%ds", isFirst ? ' ' : g_delimiter, HEADERS[i].length, HEADERS[i].title);
                        restOfLine -= HEADERS[i].length + (isFirst ? 0 : 1);
                        isFirst = false;
                    }
                }
                if (g_disp & 1)
                {
                    PRINTF("%c%%s", isFirst ? ' ' : g_delimiter, HEADERS[0].title);
                    restOfLine -= isFirst ? 0 : 1;
                }
                printf("\n%s\n", splitter);
            }
        }

        for (size_t i = 0; i < info->AsSize(key::archive::FILE_COUNT); ++i)
        {
            Info* finfo = info->AsChildInfo(key::file::OBJECT, i);

            if (g_verbose < 2)
            {
                bool isFirst = true;
                for (size_t j = 1; j < headerCount; ++j)
                {
                    if (g_disp & (int) pow(2, j))
                    {
                        char fmt[20] = { 0, };
                        snprintf(fmt, sizeof(fmt), "%c%%%d%s", isFirst ? ' ' : g_delimiter, HEADERS[j].length, HEADERS[j].type);

                        switch (j)
                        {
                        case 1:
                            printf(fmt, finfo->AsUInt64(key::file::size::PACKED));
                            break;
                        case 2:
                            printf(fmt, finfo->AsUInt64(key::file::size::UNPACKED));
                            break;
                        case 3:
                            printf(fmt, finfo->AsUInt32(key::file::attribute::ATTRIBUTES));
                            break;
                        case 4:
                            printf(fmt, finfo->AsSize(key::file::BLOCK_COUNT));
                            break;
                        case 5:
                            printf(fmt, finfo->AsBool(key::file::IS_ENCRYPTED) ? "yes" : "no");
                            break;
                        case 6:
                            printf(fmt, finfo->AsString(key::file::method::NAME));
                            break;
                        case 7:
                            printf(fmt, finfo->AsUInt32(key::file::CRC));
                            break;
                        case 8:
                        {
                            bool handle = false;
                            const char* comment = finfo->AsString(key::file::comment::COMMENT, &handle);
                            printf(fmt, handle && comment && *comment ? comment : "none");
                        }
                            break;
                        }
                        isFirst = false;
                    }
                }
                if (g_disp & 1)
                {
                    PRINTF("%c%%s", isFirst ? ' ' : g_delimiter, utf8::path_ellipsis(restOfLine, finfo->AsString(key::file::name::NAME)));
                }
            }
            else
            {
                printf("%s\n", utf8::path_ellipsis(restOfLine, finfo->AsString(key::file::name::NAME)));
                printf("[\n");
                for (size_t j = 1; j < headerCount; ++j)
                {
                    char fmt[20];
                    snprintf(fmt, sizeof(fmt), "\t%%11s : %%%s\n", HEADERS[j].type);

                    if (g_disp & (int) pow(2, j))
                    {
                        switch (j)
                        {
                        case 1:
                            printf(fmt, HEADERS[j].title, finfo->AsUInt64(key::file::size::PACKED));
                            break;
                        case 2:
                            printf(fmt, HEADERS[j].title, finfo->AsUInt64(key::file::size::UNPACKED));
                            break;
                        case 3:
                            printf(fmt, HEADERS[j].title, finfo->AsUInt32(key::file::attribute::ATTRIBUTES));
                            break;
                        case 4:
                            printf(fmt, HEADERS[j].title, finfo->AsSize(key::file::BLOCK_COUNT));
                            break;
                        case 5:
                            printf(fmt, HEADERS[j].title, finfo->AsBool(key::file::IS_ENCRYPTED) ? "yes" : "no");
                            break;
                        case 6:
                            printf(fmt, HEADERS[j].title, finfo->AsString(key::file::method::NAME));
                            break;
                        case 7:
                            printf(fmt, HEADERS[j].title, finfo->AsUInt32(key::file::CRC));
                            break;
                        case 8:
                        {
                            bool handle = false;
                            const char* comment = finfo->AsString(key::file::comment::COMMENT, &handle);
                            printf(fmt, HEADERS[j].title, handle && comment && *comment ? comment : "none");
                        }
                            break;
                        }
                    }
                }
                printf("]\n");
            }
            printf("\n");
        }
        printf("%s\n", splitter);
    }
    else
    {
        PrintError(res);
    }

    return res;
}

int ExtractArchive(CommandLine& cmd)
{
    Workspace archive;
    archive.SetEvent(&g_event);

    int res = archive.Open(cmd.GetArgument(0));

    if (res == Result::Success)
    {
        char cwd[1024] = { 0, };
        if (cmd.GetArgumentCount() == 1)
        {
            if (!getcwd(cwd, 1024))
            {
            }
        }
        else
        {
            strcpy(cwd, cmd.GetArgument(1));
        }
        res = archive.Extract(cwd);

        if (res == Result::Success)
        {
            if (g_verbose > 0)
            {
                printf("Total: %u File(s)\n\n", static_cast<unsigned int> (archive.GetInfo()->AsSize(key::archive::FILE_COUNT)));
            }
        }
        else
        {
            PrintError(res);
        }
    }
    else
    {
        PrintError(res);
    }

    return res;
}

int ProcessCommandline(CommandLine& cmd)
{
    int res = -1;

    if (cmd.CommandExists("x"))
    {
        return ExtractArchive(cmd);
    }
    else
    {
        return PrintListOfArchive(cmd.GetArgument(0));
    }

    return res;
}

int main(int argc, char* argv[])
{
    using namespace nest;
    nest::utf8::initialize();
    int res = -1;

    setlocale(LC_ALL, "");

#ifndef __MINGW32__
    ioctl(0, TIOCGWINSZ, &g_console);
#else
    g_console.ws_col = 80;
#endif
    if (g_console.ws_col == 0)
    {
        g_console.ws_col = 132;
    }
    nest::Workspace::AddFactory(new nest::EggFormatFactory());
    nest::Workspace::AddFactory(new nest::ALZFormatFactory());

    CommandLine cmd(argc, argv);
    if (cmd.GetArgumentCount() < 1 || cmd.GetCommandCount() > 1)
    {
        PrintProgramDetails();

        PrintUsage();
    }
    else
    {
        bool redirect = cmd.CommandExists("r");

        g_event.SetDisplayLevel(g_verbose);
        g_event.SetRedirect(redirect);

        std::set<std::string>::iterator it;
        for (it = cmd.GetCommands().begin(); it != cmd.GetCommands().end(); ++it)
        {
            std::string str = *it;
            switch (str[0])
            {
            case 'd':
            {
                g_disp = 0;
                for (size_t i = 1; i < str.length(); ++i)
                {
                    switch (str[i])
                    {
                    case 'n':
                        g_disp |= 1;
                        break;
                    case 'p':
                        g_disp |= 2;
                        break;
                    case 'u':
                        g_disp |= 4;
                        break;
                    case 'a':
                        g_disp |= 8;
                        break;
                    case 'b':
                        g_disp |= 16;
                        break;
                    case 'e':
                        g_disp |= 32;
                        break;
                    case 'm':
                        g_disp |= 64;
                        break;
                    case 'r':
                        g_disp |= 128;
                        break;
                    case 'c':
                        g_disp |= 256;
                    }
                }
            }
                break;
            case 'p':
                if (str.length() > 1)
                {
                    g_delimiter = str[1];
                }
                break;
            case 'v':
                if (str.length() > 1)
                {
                    g_verbose = atoi(str.c_str() + 1);
                }
            }
        }

        PrintProgramDetails();

        res = ProcessCommandline(cmd);
    }

    nest::utf8::finalize();

    return res;
}
