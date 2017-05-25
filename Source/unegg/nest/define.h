/*
 * Define.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: Jehoon Shin(Revolution)
 *
 *  Copyright(c) 2010-Present ESTsoft Corp. All rights reserved.
 *
 *  For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef NEST_DEFINE_H_
#define NEST_DEFINE_H_

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

typedef unsigned long long uint64;
typedef long long int64;

namespace Result
{

enum
{
    Success,
    Skip,
    End,
    Fail,
    Cancel,
    OpenError,
    ReadError,
    WriteError,
    Invalid,
    Unsupported,
    UnknownAlgorithm,
    NotReady,
    InvalidCRC,
    InvalidRequest,
    SolidError,
    InsufficientBlock,
    DecryptError,
    UnknownDecryptMethod,
    NeedMoreStream
};
}

const int PROGRESS_MAX = 10000;
const int FILENAME_LENGTH_MAX = FILENAME_MAX;

#ifndef SIZE_T_MAX
#define SIZE_T_MAX (size_t)(-1)
#endif

#endif /* NEST_DEFINE_H_ */
