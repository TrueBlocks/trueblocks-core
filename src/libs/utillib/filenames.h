#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
    string_q path;
    string_q fileName;
    CFilename(void) {
    }  // cannot create default

  public:
    explicit CFilename(const string_q& fnIn);
    virtual ~CFilename(void) {
    }
    string_q getPath(void) const;
    string_q getFilename(void) const;
    string_q getFullPath(void) const;
    string_q relativePath(const string_q& relTo = "") const;
    virtual bool isValid(void) {
        return fileExists(getFullPath());
    }
};

//----------------------------------------------------------------------------------
class CPath : public CFilename {
  private:
    CPath(void) : CFilename() {
    }  // cannot create default

  public:
    explicit CPath(const string_q& pathIn) : CFilename(pathIn) {
        ASSERT(fileName.empty());
    }
    virtual bool isValid(void) {
        return CFilename::isValid() && fileName.empty();
    }
};

//----------------------------------------------------------------------------------
typedef bool (*FILEVISITOR)(const string_q& path, void* data);

//------------------------------------------------------------------
extern string_q getEffectiveUserName(void);
extern string_q getHomeFolder(void);
extern bool forEveryFileInFolder(const string_q& mask, FILEVISITOR func, void* data);
extern size_t listFilesInFolder(CStringArray& files, const string_q& mask, bool recurse);
extern string_q getFirstFileInFolder(const string_q& folder, bool recurse);
extern string_q getLastFileInFolder(const string_q& folder, bool recurse);

//-------------------------------------------------------------------------
#define ANY_FILETYPE -1

//-------------------------------------------------------------------------
#define quote(a) (string_q("\"") + a + "\"")

//----------------------------------------------------------------------------------
#define F_INCLUDE_FOLDERS (1 << 1)
#define F_INCLUDE_FILES (1 << 2)
#define F_FULL_PATHS (1 << 3)
#define F_RECURSIVE (1 << 4)
#define F_DEPTHFIRST (1 << 5)
#define F_SORTED (1 << 6)
#define F_INCLUDE_ALL (F_INCLUDE_FOLDERS | F_INCLUDE_FILES)
#define F_DEFAULT (F_INCLUDE_ALL | F_RECURSIVE | F_SORTED)
#define F_FILES_ONLY (F_INCLUDE_FILES | F_RECURSIVE | F_SORTED)
#define F_FOLDERS_ONLY (F_INCLUDE_FOLDERS | F_RECURSIVE | F_SORTED)

}  // namespace qblocks

extern bool visitFile(const qblocks::string_q& path, void* data);
