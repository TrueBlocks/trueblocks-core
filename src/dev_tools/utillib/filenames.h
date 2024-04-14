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

inline string_q getHomeFolder(void) {
    struct passwd pd;
    struct passwd* pwdptr = &pd;
    struct passwd* tempPwdPtr;
    char pwdbuffer[200];
    size_t pwdlinelen = sizeof(pwdbuffer);
    if (getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr) == 0)
        return string_q(pd.pw_dir) + "/";
    return "./";
}

}  // namespace qblocks

extern bool visitFile(const qblocks::string_q& path, void* data);
