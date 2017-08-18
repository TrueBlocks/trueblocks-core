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
    void forAllFiles(const SFString& mask, FILEVISITOR func, void *data ) {
        glob_t globBuf;
        glob((const char *)mask, GLOB_MARK, globErrFunc, &globBuf);
        bool done = false;
        for (size_t i = 0 ; i < globBuf.gl_pathc && !done ; i++)
            if (!(func)(globBuf.gl_pathv[i], data))
                done = true;
        globfree(&globBuf);
    }

    //----------------------------------------------------------------------------------
    void forEveryFileInFolder(const SFString& mask, FILEVISITOR func, void *data) {
        forAllFiles(mask, func, data);
    }

    //--------------------------------------------------------------------------------
    SFString getHomeFolder(void) {
        struct passwd pd;
        struct passwd* pwdptr = &pd;
        struct passwd* tempPwdPtr;
        char pwdbuffer[200];
        int pwdlinelen = sizeof(pwdbuffer);

        if (getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr) == 0)
            return SFString(pd.pw_dir) + "/";
        return "./";
    }

    //----------------------------------------------------------------------------------
    CFilename::CFilename(const SFString& fnIn) {
        SFString fn = fnIn;
        if (!fn.startsWith('/') && !fn.startsWith('.') && !fn.startsWith('~'))
            fn = "./" + fn;  // assume cwd
        fn.Replace("../", getCWD() + "xx/");
        fn.Replace("./",  getCWD());
        fn.Replace("~/",  getHomeFolder());
        fn.Replace("xx/", "../");

        if (fn.endsWith('/')) {
            path = fn;
            fileName = EMPTY;

        } else {
            path = fn.Left(fn.ReverseFind('/')+1);
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
