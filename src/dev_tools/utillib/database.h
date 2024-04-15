#pragma once
#include "sfos.h"

namespace qblocks {

#define READ_OK 1
#define READ_EOF 2
#define READ_BAD 3

#define modeReadOnly "r"      // read only - fails if not present
#define modeReadWrite "r+"    // read-write (file must exist)
#define modeWriteCreate "w"   // writing - destroys previous contents or creates
#define modeWriteAppend "a+"  // read/writing - appends

#define LOCK_NOWAIT 1  // read only - do not even check for a lock
#define LOCK_WAIT 2    // Wait for lock to release return true - if wait too long return false
#define LOCK_CREATE 3  // Wait for lock to release - if waiting too long destroy existing lock

//------------------------------------------------------------------------
// A file that must be locked before being accessed (such as a datafile)
//------------------------------------------------------------------------
class CSharedResource {
  private:
    string_q m_mode;
    string_q m_errorMsg;
    size_t m_error;
    bool m_ownsLock;
    string_q m_lockingUser;

  protected:
    string_q m_filename;

  public:
    FILE* m_fp;
    CSharedResource(void) {
        m_fp = NULL;
        m_error = 0;
        m_ownsLock = false;
    }

    virtual ~CSharedResource(void) {
        Release();
    }

    bool Lock(const string_q& fn, const string_q& mode, size_t obeyLock);
    void Release(void);

  private:
    bool waitOnLock(bool deleteOnFail) const;
    bool createLockFile(const string_q& lockfilename);
    bool createLock(bool createOnFail);

    CSharedResource(const CSharedResource& l);
    CSharedResource& operator=(const CSharedResource& l);
};

extern string_q asciiFileToString(const string_q& filename);
extern size_t asciiFileToLines(const string_q& fileName, CStringArray& lines);
extern size_t stringToAsciiFile(const string_q& fileName, const string_q& contents);
extern size_t appendToAsciiFile(const string_q& fileName, const string_q& contents);
extern string_q manageRemoveList(const string_q& filename = "");

}  // namespace qblocks
