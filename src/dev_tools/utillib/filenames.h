#pragma once
#include "basetypes.h"
#include "sfos.h"

namespace qblocks {

//----------------------------------------------------------------------------------
class CFilename {
  protected:
    string_q path;
    string_q fileName;

  public:
    explicit CFilename(const string_q& fnIn);
    virtual ~CFilename(void) {
    }
    string_q getPath(void) const;
    string_q getFilename(void) const;
};

extern string_q getHomeFolder(void);
typedef bool (*CONSTAPPLYFUNC)(const string_q& path, void* data);
extern bool forEveryFileInFolder(const string_q& mask, CONSTAPPLYFUNC func, void* data);
extern size_t listFilesInFolder(CStringArray& files, const string_q& mask, bool recurse);
extern string_q getLastFileInFolder(const string_q& folder, bool recurse);

}  // namespace qblocks

extern bool visitFile(const qblocks::string_q& path, void* data);
