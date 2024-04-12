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

int cleanFolder(const string_q& path, bool recurse, bool interactive) {
    CStringArray files;
    listFilesInFolder(files, path, true);
    for (auto file : files)
        ::remove(file.c_str());
    return static_cast<int>(files.size());
}

static string_q escapePath(const string_q& nameIn) {
    string_q name = nameIn;
    replaceAll(name, "&", "\\&");
    replaceAll(name, "(", "\\(");
    replaceAll(name, ")", "\\)");
    replaceAll(name, "'", "\\'");
    return name;
}

int copyFile(const string_q& fromIn, const string_q& toIn) {
    ifstream src(escapePath(fromIn), ios::binary);
    ofstream dst(escapePath(toIn), ios::binary);
    dst << src.rdbuf();
    return static_cast<int>(fileExists(toIn));
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

static const char* CHR_VALID_NAME =
    "\t\n\r()<>[]{}`\\|; "
    "'!$^*~@"
    "?&#+%"
    ",:/=\"";

string_q makeValidName(const string_q& inOut) {
    string_q ret = inOut;
    replaceAny(ret, CHR_VALID_NAME, "_");
    if (!ret.empty() && isdigit(ret[0]))
        ret = "_" + ret;
    return ret;
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

bool folderExists(const string_q& folderName) {
    if (folderName.empty())
        return false;

    string_q folder = folderName;
    if (!endsWith(folder, '/'))
        folder += '/';

    size_t nFiles = 0;
    string_q mask = folder + "*.*";
    doGlob(nFiles, NULL, mask, true);

    if (!nFiles)
        doGlob(nFiles, NULL, mask, false);
    if (!nFiles) {
        mask = folder + ".";
        doGlob(nFiles, NULL, mask, false);
    }

    return (nFiles > 0);
}

uint64_t fileSize(const string_q& filename) {
    if (!fileExists(filename))
        return 0;
    struct stat statBuf;
    stat(filename.c_str(), &statBuf);
    return (uint64_t)statBuf.st_size;
}

}  // namespace qblocks
