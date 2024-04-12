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
    bool ReLock(const string_q& mode);
    void Release(void);
    void Close(void);

    bool isOpen(void) const {
        return (m_fp != NULL);
    }

    long Tell(void) const;                     // NOLINT
    void Seek(long offset, int whence) const;  // NOLINT
    bool Eof(void) const;
    char* ReadLine(char* buff, size_t maxBuff);
    void WriteLine(const string_q& str);
    string_q LockFailure(void) const;
    void flush(void) {
        if (m_fp)
            fflush(m_fp);
    }
    void setFilename(const string_q& fn) {
        m_filename = fn;
    }

  public:
    size_t Read(bool& val);
    size_t Read(char& val);
    size_t Read(int& val);
    size_t Read(int8_t& val);
    size_t Read(uint8_t& val);
    size_t Read(int16_t& val);
    size_t Read(uint16_t& val);
    size_t Read(unsigned int& val);
    size_t Read(long& val);                // NOLINT
    size_t Read(unsigned long& val);       // NOLINT
    size_t Read(long long& val);           // NOLINT
    size_t Read(unsigned long long& val);  // NOLINT
    size_t Read(float& val);
    size_t Read(double& val);
    size_t Read(string_q& val);

    size_t Write(bool val) const;
    size_t Write(char val) const;
    size_t Write(int val) const;
    size_t Write(int8_t val) const;
    size_t Write(uint8_t val) const;
    size_t Write(int16_t val) const;
    size_t Write(uint16_t val) const;
    size_t Write(unsigned int val) const;
    size_t Write(long val) const;                // NOLINT
    size_t Write(unsigned long val) const;       // NOLINT
    size_t Write(long long val) const;           // NOLINT
    size_t Write(unsigned long long val) const;  // NOLINT
    size_t Write(float val) const;
    size_t Write(double val) const;
    size_t Write(const string_q& val) const;

    size_t Read(void* buff, size_t size, size_t cnt);
    size_t Write(const void* buff, size_t size, size_t cnt) const;

  private:
    size_t Read(const char* val) const;
    size_t Write(const char* val) const;

    bool waitOnLock(bool deleteOnFail) const;
    bool createLockFile(const string_q& lockfilename);
    bool createLock(bool createOnFail);

    CSharedResource(const CSharedResource& l);
    CSharedResource& operator=(const CSharedResource& l);
};

extern string_q asciiFileToString(const string_q& filename);
extern size_t asciiFileToLines(const string_q& fileName, CStringArray& lines);
extern size_t stringToAsciiFile(const string_q& fileName, const string_q& contents);
extern string_q manageRemoveList(const string_q& filename = "");

}  // namespace qblocks
