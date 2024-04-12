#include <glob.h>
#include <libgen.h>
#include <algorithm>
#include <unistd.h>
#ifdef __APPLE__
#include <libproc.h>
#else
#include <iostream>
#include <filesystem>
#endif
#include "basetypes.h"
#include "sfos.h"
#include "sfstring.h"
#include "database.h"
#include "filenames.h"

namespace qblocks {

#define remove unlink

static int globErrFunc(const char* epath, int eerrno) {
    //  perror(epath);
    return 0;
}

void doGlob(size_t& nStrs, string_q* strs, const string_q& maskIn, int wantFiles) {
    glob_t globBuf;

    string_q mask = maskIn;

    // should check return code
    glob(mask.c_str(), GLOB_MARK, globErrFunc, &globBuf);

    size_t n = globBuf.gl_pathc;
    size_t mx = nStrs;
    nStrs = 0;
    char c;

    for (size_t i = 0; i < n; i++) {
        // get path
        char* tmp = globBuf.gl_pathv[i];
        // last char
        c = tmp[strlen(tmp) - 1];

        // if path ends in '/' then this is directory, filter accordingly

        bool isDir = ('/' == c);
        if (NULL != strs) {
            string_q path = globBuf.gl_pathv[i];

            // filter specified directories and remove trailing '/'
            if (endsWith(path, '/'))
                path = extract(path, 0, path.length() - 1);

            // trim path to last directory / file
            path = CFilename(path).getFilename();
            if (startsWith(path, '/'))
                path = extract(path, 1);

            if (isDir)
                path = "d-" + path;
            else
                path = "f-" + path;

            strs[nStrs] = path;
        }

        nStrs++;
        if (NULL != strs && nStrs >= mx) {
            break;
        }
    }

    globfree(&globBuf);
}

string_q getCWD(const string_q& filename) {
    string_q folder;
    size_t kMaxPathSize = _POSIX_PATH_MAX;
    char buffer[kMaxPathSize];
    if (::getcwd(buffer, kMaxPathSize))
        folder = buffer;
    if (!endsWith(folder, '/'))
        folder += "/";
    return folder + filename;  // may be empty
}

bool fileExists(const string_q& file) {
    struct stat statBuf;
    return !file.empty() && stat(file.c_str(), &statBuf) == 0;
}

uint64_t fileSize(const string_q& filename) {
    if (!fileExists(filename))
        return 0;
    struct stat statBuf;
    stat(filename.c_str(), &statBuf);
    return (uint64_t)statBuf.st_size;
}

}  // namespace qblocks
