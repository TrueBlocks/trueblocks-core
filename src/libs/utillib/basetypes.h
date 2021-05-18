#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <ctype.h>
#include <curl/curl.h>
#include <glob.h>
#include <inttypes.h>
#include <libgen.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <utime.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <iomanip>
#include <bitset>
#include <mutex>  // NOLINT
#include <dlfcn.h>

//-------------------------------------------------------------------------
using namespace std;  // NOLINT - bad practice, but it's way too late now

//--------------------------------------------------------------------
#define NOPOS ((size_t)-1)

//-------------------------------------------------------------------------
// #define DEBUG 1
#ifdef _DEBUG
#define ASSERT(a)                                                                                                      \
    {                                                                                                                  \
        if (!(a)) {                                                                                                    \
            cout << "error at " << __FILE__ << "(" << __LINE__ << ")\n";                                               \
        }                                                                                                              \
    }
#define XX(a)                                                                                                          \
    {                                                                                                                  \
        cout << __FILE__ << " : " << __LINE__ << "\n\t" << (a) << "\n";                                                \
        cout.flush();                                                                                                  \
    }
#else
#define ASSERT(a)
#define XX(a)
#endif

//-------------------------------------------------------------------------
#include "sfstring.h"

namespace qblocks {

//-------------------------------------------------------------------------
typedef pair<string_q, string_q> CNameValue;
typedef vector<CNameValue> CNameValueArray;
typedef map<string_q, string_q> CNameValueMap;
typedef map<string_q, uint64_t> CCounterMap;

//-------------------------------------------------------------------------
typedef bool (*APPLYFUNC)(string_q& line, void* data);
typedef bool (*CHARPTRFUNC)(const char* str, void* data);
typedef int (*SEARCHFUNC)(const void* ob1, const void* ob2);
typedef int (*SORTINGFUNC)(const void* ob1, const void* ob2);

}  // namespace qblocks
