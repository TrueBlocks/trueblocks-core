/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

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
