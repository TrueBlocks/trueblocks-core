#pragma once
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
    }
    virtual bool isValid(void) {
        return CFilename::isValid() && fileName.empty();
    }
};

extern string_q getHomeFolder(void);
extern bool forEveryFileInFolder(const string_q& mask, CONSTAPPLYFUNC func, void* data);
extern size_t listFilesInFolder(CStringArray& files, const string_q& mask, bool recurse);
extern size_t nFilesInFolder(const string& path, bool recurse = false);
extern string_q getLastFileInFolder(const string_q& folder, bool recurse);

#define ANY_FILETYPE -1

}  // namespace qblocks

extern bool visitFile(const qblocks::string_q& path, void* data);
