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

#include <dirent.h>
#include "filenames.h"
#include "sfos.h"
#include "logging.h"

namespace qblocks {

//----------------------------------------------------------------------------------
int globErrFunc(const char* epath, int eerrno) {
    perror(epath);
    printf("%d:%s\n", eerrno, epath);
    return 0;
}

//----------------------------------------------------------------------------------
bool forAllFiles(const string_q& mask, CONSTAPPLYFUNC func, void* data) {
    glob_t globBuf;
    glob(mask.c_str(), GLOB_MARK, globErrFunc, &globBuf);
    bool quitEarly = false;
    for (size_t i = 0; i < globBuf.gl_pathc && !quitEarly; i++)
        if (!(func)(globBuf.gl_pathv[i], data))
            quitEarly = true;
    globfree(&globBuf);
    return !quitEarly;  // if we quit early, we want to return false, true if we quit naturally
}

//----------------------------------------------------------------------------------
bool forEveryFileInFolder(const string_q& mask, CONSTAPPLYFUNC func, void* data) {
    // if we quit after visiting all files, return true.
    return forAllFiles(mask, func, data);
}

//----------------------------------------------------------------------------------
namespace filename_local {
class CFileListState {
  public:
    string_q top;
    CStringArray& list;
    bool recurse;
    CFileListState(const string_q& t, CStringArray& l, bool r) : top(t), list(l), recurse(r) {
    }
};
bool visitFile(const string_q& path, void* data) {
    CFileListState* state = reinterpret_cast<CFileListState*>(data);
    if (endsWith(path, '/')) {
        if (path == state->top || state->recurse) {
            return forEveryFileInFolder(path + "*", visitFile, data);
        } else {
            state->list.push_back(path);
        }
        return true;
    }
    state->list.push_back(path);
    return true;
}
};  // namespace filename_local

//------------------------------------------------------------------
size_t listFilesInFolder(CStringArray& items, const string_q& folder, bool recurse) {
    filename_local::CFileListState state(folder, items, recurse);
    forEveryFileInFolder(folder, filename_local::visitFile, &state);
    return items.size();
}

//------------------------------------------------------------------------------------------------
size_t nFilesInFolder(const string& path, bool recurse) {
    if (recurse) {
        LOG_WARN("recursive counting not implemented.");
        return 0;
    }

    DIR* dp = opendir(path.c_str());
    if (!dp) {
        LOG_WARN("Could not open directory ", path, ".");
        return 0;
    }

    size_t ret = 0;
    struct dirent* ep = NULL;
    while ((ep = readdir(dp)) != NULL)
        ret++;
    closedir(dp);
    return ret;
}

//--------------------------------------------------------------
string_q getFirstFileInFolder(const string_q& folder, bool recurse) {
    CStringArray files;
    listFilesInFolder(files, folder, recurse);
    sort(files.begin(), files.end());
    return (files.size() ? files[0] : "");
}

//--------------------------------------------------------------
string_q getLastFileInFolder(const string_q& folder, bool recurse) {
    CStringArray files;
    listFilesInFolder(files, folder, recurse);
    sort(files.begin(), files.end());
    return (files.size() ? files[files.size() - 1] : "");
}

//--------------------------------------------------------------------------------
string_q getEffectiveUserName(void) {
    struct passwd pd;
    struct passwd* pwdptr = &pd;
    struct passwd* tempPwdPtr;
    char pwdbuffer[200];
    size_t pwdlinelen = sizeof(pwdbuffer);

    if (getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr) == 0)
        return string_q(pd.pw_name);
    return "nobody";
}

//--------------------------------------------------------------------------------
string_q getHomeFolder(void) {
    struct passwd pd;
    struct passwd* pwdptr = &pd;
    struct passwd* tempPwdPtr;
    char pwdbuffer[200];
    size_t pwdlinelen = sizeof(pwdbuffer);

    if (getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr) == 0)
        return string_q(pd.pw_dir) + "/";
    return "./";
}

//----------------------------------------------------------------------------------
CFilename::CFilename(const string_q& fnIn) {
    string_q fn = fnIn;
    if (!startsWith(fn, '/') && !startsWith(fn, '.') && !startsWith(fn, '~'))
        fn = "./" + fn;  // assume cwd
    replace(fn, "../", getCWD() + "$%^&#*/");
    replace(fn, "./", getCWD());
    replace(fn, "~/", getHomeFolder());
    replace(fn, "$%^&#*/", "../");

    if (endsWith(fn, '/')) {
        path = fn;
        fileName = "";

    } else {
        path = extract(fn, 0, fn.rfind('/') + 1);
        fileName = substitute(fn, path, "");
    }
}

//----------------------------------------------------------------------------------
string_q CFilename::getPath(void) const {
    return path;
}

//----------------------------------------------------------------------------------
string_q CFilename::getFilename(void) const {
    return fileName;
}

//----------------------------------------------------------------------------------
string_q CFilename::getFullPath(void) const {
    return substitute((path + fileName), "//", "/");
}

//----------------------------------------------------------------------------------
string_q CFilename::relativePath(const string_q& relTo) const {
    string_q rel = (relTo.empty() ? getCWD() : relTo);
    return substitute(getFullPath(), rel, "./");
}

}  // namespace qblocks
