#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "basetypes.h"
#include "sfos.h"

namespace qblocks {

    //----------------------------------------------------------------------------------
    class CFilename {
    protected:
        SFString path;
        SFString fileName;
        CFilename(void) {}  // cannot create default

    public:
        explicit CFilename(const SFString& fnIn);
        virtual ~CFilename(void) { }
        SFString getPath(void) const;
        SFString getFilename(void) const;
        SFString getFullPath(void) const;
        SFString relativePath(const SFString& relTo="") const;
        virtual bool isValid(void) { return fileExists(getFullPath()); }
    };

    //----------------------------------------------------------------------------------
    class CPath : public CFilename {
    private:
        CPath(void) : CFilename() { }  // cannot create default

    public:
        explicit CPath(const SFString& pathIn) : CFilename(pathIn) { ASSERT(fileName.empty()); }
        virtual bool isValid(void) { return CFilename::isValid() && fileName.empty(); }
    };

    //----------------------------------------------------------------------------------
    typedef bool (*FILEVISITOR)(const SFString& str, void *data);

    //------------------------------------------------------------------
    extern int globErrFunc(const char *epath, int eerrno);
    extern bool forAllFiles(const SFString& mask, FILEVISITOR func, void *data);
    extern bool forEveryFileInFolder(const SFString& mask, FILEVISITOR func, void *data);
    extern SFString getHomeFolder(void);

    //-------------------------------------------------------------------------
    #define quote(a) (SFString("\"") + a + "\"")

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

}  // namespace qblocks

extern bool visitFile(const qblocks::SFString& path, void *data);
