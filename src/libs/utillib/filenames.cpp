/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

#include "filenames.h"
#include "sfos.h"

namespace qblocks {

    //----------------------------------------------------------------------------------
    int globErrFunc(const char *epath, int eerrno) {
        perror(epath);
        printf("%d:%s\n", eerrno, epath);
        return 0;
    }

    //----------------------------------------------------------------------------------
    bool forAllFiles(const SFString& mask, FILEVISITOR func, void *data ) {
        glob_t globBuf;
        glob((const char *)mask, GLOB_MARK, globErrFunc, &globBuf);
        bool quitEarly = false;
        for (size_t i = 0 ; i < globBuf.gl_pathc && !quitEarly ; i++)
            if (!(func)(globBuf.gl_pathv[i], data))
                quitEarly = true;
        globfree(&globBuf);
        return !quitEarly; // if we quit early, we want to return false, true if we quit naturally
    }

    //----------------------------------------------------------------------------------
    bool forEveryFileInFolder(const SFString& mask, FILEVISITOR func, void *data) {
        // if we quit after visiting all files, return true.
        return forAllFiles(mask, func, data);
    }

    //--------------------------------------------------------------------------------
    SFString getHomeFolder(void) {
        struct passwd pd;
        struct passwd* pwdptr = &pd;
        struct passwd* tempPwdPtr;
        char pwdbuffer[200];
        size_t pwdlinelen = sizeof(pwdbuffer);

        if (getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr) == 0)
            return SFString(pd.pw_dir) + "/";
        return "./";
    }

    //----------------------------------------------------------------------------------
    CFilename::CFilename(const SFString& fnIn) {
        SFString fn = fnIn;
        if (!fn.startsWith('/') && !fn.startsWith('.') && !fn.startsWith('~'))
            fn = "./" + fn;  // assume cwd
        fn.Replace("../", getCWD() + "$%^&#*/");
        fn.Replace("./",  getCWD());
        fn.Replace("~/",  getHomeFolder());
        fn.Replace("$%^&#*/", "../");

        if (fn.endsWith('/')) {
            path = fn;
            fileName = EMPTY;

        } else {
            path = fn.substr(0,fn.ReverseFind('/')+1);
            fileName = fn.Substitute(path, EMPTY);
        }
    }

    //----------------------------------------------------------------------------------
    SFString CFilename::getPath(void) const {
        return path;
    }

    //----------------------------------------------------------------------------------
    SFString CFilename::getFilename(void) const {
        return fileName;
    }

    //----------------------------------------------------------------------------------
    SFString CFilename::getFullPath(void) const {
        return (path + fileName).Substitute("//", "/");
    }

    //----------------------------------------------------------------------------------
    SFString CFilename::relativePath(const SFString& relTo) const {
        SFString rel = (relTo.empty() ? getCWD() : relTo);
        return getFullPath().Substitute(rel, "./");
    }

}  // namespace qblocks
