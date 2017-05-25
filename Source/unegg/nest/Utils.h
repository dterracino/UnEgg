/*
 * Utils.h
 *
 *  Created on: 2010. 12. 10.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_UTILS_H_
#define NEST_UTILS_H_

#include <nest/nest.h>
#include <string>

namespace nest
{

namespace utf8
{
void initialize();
void finalize();

size_t strlen(const char* s, size_t size = (size_t)(-1));
size_t strsize(const char* s);
int charlen(char s);
const char* next(const char* s);
const char* prev(const char* s);
const char* offset(const char* s, size_t n);
bool nth(const char* s, size_t n, char buffer[6], int* len = NULL);
const char* find(const char* s, const char* substr);
const char* find_r(const char* s, const char* substr);
size_t p2i(const char* s, size_t pos);
size_t i2p(const char* s, size_t index);
size_t euckr2utf8(const char* str, std::string* utf8);
size_t console_len(const char* str, size_t bytes = 0);
void limit(char* buffer, size_t size, size_t limitWidth, const char* str);
const char* limit(size_t limitWidth, const char* str);
void path_ellipsis(char* buffer, size_t size, size_t limitWidth, const char* str);
const char* path_ellipsis(size_t limitWidth, const char* str);
}

static char _nest_buffer_[10240] = { 0, };

#define PRINTF(fmt, arg1, args, ...) \
    snprintf(_nest_buffer_, 1024, fmt, arg1); \
    printf(_nest_buffer_, args);

#define PRINTF2(fmt, arg1, arg2, args, ...) \
    snprintf(_nest_buffer_, 1024, fmt, arg1, arg2); \
    printf(_nest_buffer_, args);

#define PRINTF3(fmt, arg1, arg2, arg3, args, ...) \
    snprintf(_nest_buffer_, 1024, fmt, arg1, arg2, arg3); \
    printf(_nest_buffer_, args);

int mkdir_r(const char* dir, mode_t mode);

}

#endif /* NEST_UTILS_H_ */
