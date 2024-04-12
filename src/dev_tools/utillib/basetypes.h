#pragma once

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
#include <memory>

using namespace std;

#define NOPOS ((size_t)-1)

#include "sfstring.h"

namespace qblocks {

using string_q = std::string;
using CStringArray = vector<string_q>;

}  // namespace qblocks
