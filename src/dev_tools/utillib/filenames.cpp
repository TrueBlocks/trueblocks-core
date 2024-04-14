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

namespace qblocks {

int globErrFunc(const char* epath, int eerrno) {
    perror(epath);
    printf("%d:%s\n", eerrno, epath);
    return 0;
}

typedef bool (*CONSTAPPLYFUNC)(const string_q& path, void* data);
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

string_q CFilename::getPath(void) const {
    return path;
}

string_q CFilename::getFilename(void) const {
    return fileName;
}

}  // namespace qblocks
