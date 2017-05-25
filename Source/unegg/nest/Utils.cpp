/*
 * Utils.cpp
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#include "Utils.h"
#include <sys/stat.h>
#include <errno.h>
#include <iconv.h>
#include <vector>
#include <algorithm>

#ifdef __MINGW32__
#include <io.h>
#endif

namespace nest
{

namespace utf8
{

iconv_t g_iconv = NULL;
char g_utf8buf[65536] = { 0, };
size_t g_utf8bufSize = sizeof(g_utf8buf);

void initialize()
{
    if (!g_iconv)
    {
        g_iconv = iconv_open("UTF-8", "EUC-KR");
    }
}

void finalize()
{
    if (g_iconv)
    {
        iconv_close(g_iconv);
        g_iconv = NULL;
    }
}

size_t strlen(const char* s, size_t size)
{
    size_t i = 0;
    size_t j = 0;

    if (s)
    {
        while (s[i])
        {
            if (size != (size_t) (-1) && i > size)
            {
                break;
            }

            if ((s[i] & 0xc0) != 0x80)
            {
                j++;
            }

            i++;
        }
    }

    return j;
}

size_t strsize(const char* s)
{
    return ::strlen(s);
}

int charlen(char s)
{
    int res = 1;

    if (s & 128)
    {
        s <<= 1;
        for (int i = 0; i < 7; ++i)
        {
            if (s & 128)
            {
                ++res;
            }
            else
            {
                break;
            }
            s <<= 1;
        }
    }

    return res;
}

const char* next(const char* s)
{
    if (s && *s)
    {
        int size = charlen(*s);
        return (s + size);
    }

    return NULL;
}

const char* prev(const char* s)
{
    if (s && *s)
    {
        --s;
        if ((*s & 128) == 0)
        {
            return s;
        }

        while ((*s & 192) == 128)
        {
            --s;
        }

        return s;
    }

    return NULL;
}

const char* offset(const char* s, size_t n)
{
    if (s && n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            s = next(s);
            if (!s)
            {
                break;
            }
        }
    }

    return s;
}

bool nth(const char* s, size_t n, char buffer[6], int* len)
{
    const char* pos = offset(s, n);
    if (pos)
    {
        memset(buffer, 0, 6);
        int l = charlen(*pos);
        if (len)
        {
            *len = l;
        }
        strncpy(buffer, pos, l);

        return true;
    }

    return false;
}

const char* find(const char* s, const char* substr)
{
    return strstr(s, substr);
}

const char* find_r(const char* s, const char* substr)
{
    const char* res = NULL;

    do
    {
        s = find(s, substr);
        if (s)
        {
            res = s++;
        }
    } while (s);

    return res;
}

size_t p2i(const char* s, size_t pos)
{
    return strlen(s, pos) - 1;
}

size_t i2p(const char* s, size_t index)
{
    size_t res = 0;

    for (size_t i = 0; i < index; ++i)
    {
        int len = charlen(*s);
        res += len;
        s += len;
    }

    return res;
}

size_t euckr2utf8(const char* str, std::string* utf8)
{
    size_t len = ::strlen(str);
    if (!len)
    {
        utf8->clear();

        return 0;
    }

    size_t utf8len = 2 * len;
    char* buffer = (char*) malloc(utf8len);
    char* utf8start = buffer;
#ifdef __MINGW32__
    size_t res = iconv(g_iconv, (const char**) &str, &len, &utf8start, &utf8len);
#else
    size_t res = iconv(g_iconv, (char**) &str, &len, &utf8start, &utf8len);
#endif

    if (res == (size_t) (-1))
    {
        utf8->clear();
    }
    else
    {
        buffer[i2p(buffer, utf8len)] = 0;
        *utf8 = buffer;
    }
    free(buffer);

    return res;
}

size_t console_len(const char* str, size_t bytes)
{
    char* p = (char*) str;
    size_t res = 0;
    while (*p)
    {
        if (bytes && bytes < (size_t)(p - str))
        {
            break;
        }
        ++res;
        size_t len = charlen(*p);
        if (len > 1)
        {
            ++res;
        }
        p += len;
    }

    return res;
}

void limit(char* buffer, size_t bufSize, size_t limitWidth, const char* str)
{
    if (limitWidth && bufSize)
    {
        --bufSize;
        char* sp = (char*) str;
        char* op = buffer;
        size_t os = 0;
        size_t cp = 0;
        while (*sp && cp < limitWidth && os < bufSize)
        {
            size_t len = charlen(*sp);
            size_t width = (len > 1 ? 2 : 1);
            if (os + len < bufSize && cp + width <= limitWidth)
            {
                memcpy(op, sp, len);
                op += len;
                os += len;
            }
            sp += len;
            cp += width;
        }
        *op = 0;
    }
    else if (bufSize)
    {
        *buffer = 0;
    }
}

const char* limit(size_t limitWidth, const char* str)
{
    limit(_nest_buffer_, sizeof(_nest_buffer_), limitWidth, str);

    return _nest_buffer_;
}

struct _path_token_info
{
    size_t pos;
    size_t length;
};

void path_ellipsis(char* buffer, size_t bufSize, size_t limitWidth, const char* str)
{
    if (limitWidth && bufSize)
    {
        size_t conwidth = console_len(str);
        bool done = false;

        if (conwidth > limitWidth)
        {
            std::vector<_path_token_info> tokens;
            std::vector<size_t> removed;
            const char* srcptr = str;
            size_t old = 0;
            size_t pos = 0;
            while (*srcptr)
            {
                if (*srcptr == '/')
                {
                    _path_token_info info;
                    info.pos = old;
                    info.length = pos - old + 1;
                    old = pos + 1;
                    tokens.push_back(info);
                }
                size_t len = charlen(*srcptr);
                srcptr += len;
                pos += len;
            }

            if (tokens.size())
            {
                std::vector<size_t> calc;
                calc.resize(tokens.size());
                for (size_t i = 0; i < tokens.size(); ++i)
                {
                    calc[i] = i;
                }

                conwidth += 4;
                while (calc.size() && conwidth > limitWidth)
                {
                    size_t p = (size_t) (calc.size() / 2.0f + 0.49f);
                    _path_token_info& token = tokens[calc[p]];
                    conwidth -= console_len(str + token.pos, token.length);
                    removed.push_back(calc[p]);
                    calc.erase(calc.begin() + p);
                }

                if (conwidth <= limitWidth)
                {
                    std::sort(removed.begin(), removed.end(), std::less<size_t>());
                    size_t min = removed[0];
                    size_t max = removed.back();
                    size_t minp = tokens[min].pos;
                    size_t maxp = tokens[max].pos + tokens[max].length - 1;

                    char* outptr = buffer;
                    size_t srcpos = 0;

                    for (size_t i = 0; i < ::strlen(str) && srcpos < bufSize - 1; ++i)
                    {
                        if (i < minp)
                        {
                            *outptr++ = str[i];
                            ++srcpos;
                        }
                        if (i >= minp && i < minp + 3)
                        {
                            *outptr++ = '.';
                            ++srcpos;
                        }
                        if (i >= maxp)
                        {
                            *outptr++ = str[i];
                            ++srcpos;
                        }
                    }
                    *outptr = 0;

                    done = true;
                }
            }
        }

        if (!done)
        {
            size_t console_width = console_len(str);
            if (console_width > limitWidth)
            {
                const char* src = str;
                while (console_width + 3 > limitWidth && *src)
                {
                    size_t len = charlen(*src);
                    console_width -= len > 1 ? 2 : 1;
                    src += len;
                }

                strncpy(buffer, "...", bufSize < limitWidth ? bufSize : limitWidth);
                if (bufSize > 3)
                {
                    strncpy(buffer + 3, src, bufSize - 3);
                }
            }
            else
            {
                strncpy(buffer, str, bufSize);
            }
        }
    }
    else if (bufSize)
    {
        *buffer = 0;
    }
}

const char* path_ellipsis(size_t limitWidth, const char* str)
{
    path_ellipsis(_nest_buffer_, sizeof(_nest_buffer_), limitWidth, str);
    return _nest_buffer_;
}

}

/*
 Recursively creates the given  path, with the given mode

 Returns: 0 on success and -1 on error
 */

int mkdir_(const char* file, mode_t mode)
{
#ifdef __MINGW32__
    return mkdir(file);
#else
    return mkdir(file, mode);
#endif
}

int mkdir_r(const char *dir, mode_t mode)
{
    const char *p = dir + 1;

    while (*p)
    {
        if (*p == '/')
        {
            *((char*) p) = '\0';

            if (mkdir_(dir, mode) == -1 && errno != EEXIST)
                return -1;

            *((char*) p) = '/';
        }

        p++;
    }

    if (*(p - 1) != '/')
        if (mkdir_(dir, mode) == -1 && errno != EEXIST)
            return -1;

    return 0;
}

char chrlwr(char c)
{
    if ((c <= 'A') && (c >= 'Z'))
    {
        c <<= 5;
    }

    return c;
}

}
