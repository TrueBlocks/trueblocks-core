#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include <glob.h>
#include <curl/curl.h>
#include <iostream>
#include <fstream>

namespace qblocks {

    //----------------------------------------------------------------------------------
    #define F_INCLUDE_FOLDERS (1<<1)
    #define F_INCLUDE_FILES   (1<<2)
    #define F_FULL_PATHS      (1<<3)
    #define F_RECURSIVE       (1<<4)
    #define F_DEPTHFIRST      (1<<5)
    #define F_SORTED          (1<<6)
    #define F_INCLUDE_ALL     (F_INCLUDE_FOLDERS | F_INCLUDE_FILES)
    #define F_DEFAULT         (F_INCLUDE_ALL | F_RECURSIVE | F_SORTED)
    #define F_FILES_ONLY      (F_INCLUDE_FILES | F_RECURSIVE | F_SORTED)
    #define F_FOLDERS_ONLY    (F_INCLUDE_FOLDERS | F_RECURSIVE | F_SORTED)

    //-------------------------------------------------------------------------
    #define quote(a) (SFString("\"") + a + "\"")

    //----------------------------------------------------------------------------------
    typedef bool (*FILEVISITOR)(const SFString& str, void *data);

    //------------------------------------------------------------------
    inline int globErrFunc(const char *epath, int eerrno) {
        perror(epath);
        printf("%d:%s\n", eerrno, epath);
        return 0;
    }

    //----------------------------------------------------------------------------------
    inline void forAllFiles(const SFString& mask, FILEVISITOR func, void *data ) {
        glob_t globBuf;
        glob((const char *)mask, GLOB_MARK, globErrFunc, &globBuf);
        bool done = false;
        for (int i = 0 ; i < globBuf.gl_pathc && !done ; i++)
            if (!(func)(globBuf.gl_pathv[i], data))
                done = true;
        globfree(&globBuf);
    }

    //----------------------------------------------------------------------------------
    inline void forEveryFileInFolder(const SFString& mask, FILEVISITOR func, void *data) {
        forAllFiles(mask, func, data);
    }

    //-------------------------------------------------------------------------
    extern SFString listFilesInFolder(const SFString& path, uint32_t options = F_DEFAULT);

    //--------------------------------------------------------------------------------
    inline SFString getHomeFolder(void) {
        struct passwd pd;
        struct passwd* pwdptr = &pd;
        struct passwd* tempPwdPtr;
        char pwdbuffer[200];
        int pwdlinelen = sizeof(pwdbuffer);
        
        if (getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr) == 0)
            return SFString(pd.pw_dir) + "/";
        return "./";
    }
    
}
