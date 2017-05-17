#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

//-------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <signal.h>
#include <unistd.h>
#include <glob.h>
#include <libgen.h>
#include <sys/timeb.h>
#include <curl/curl.h>
using namespace std;

namespace qblocks {

    //-------------------------------------------------------------------------
    using SFUint32 = uint64_t;

    //-------------------------------------------------------------------------
    #ifndef MIN
    #define MIN(a, b) (((a)>(b)) ? (b) : (a))
    #define MAX(a, b) (((a)>(b)) ? (a) : (b))
    #endif

    //-------------------------------------------------------------------------
    #define SECS_PER_DAY  (24*60*60)
    #define SECS_PER_HOUR    (60*60)
    #define SECS_PER_MIN        (60)

    //-------------------------------------------------------------------------
    #ifdef _DEBUG
    #define ASSERT(a) { if (!(a)) { cout << "error at " << __FILE__ << "(" << __LINE__ << ")\n"; } }
    #define VERIFY(a) { if (!(a)) { cout << "error at " << __FILE__ << "(" << __LINE__ << ")\n"; } (a); }
    #else
    #define ASSERT(a)
    #define VERIFY(a) (a)
    #endif

    //-------------------------------------------------------------------------
    template<class T>
    inline bool inRange(T val, T min, T max)
    {
        return (val >= min && val <= max);
    }

}  // namespace qblocks

//-------------------------------------------------------------------------
#include "sfstring.h"
#include "string_utils.h"
#include "list.h"

//-------------------------------------------------------------------------
namespace qblocks {

    typedef SFArrayBase<SFString> SFStringArray;
    typedef SFList<SFString> SFStringList;
    typedef SFArrayBase<SFUint32> SFUintArray;

}  // namespace qblocks
