#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "fielddata.h"
#include "sfos.h"

// NOTE: This file has a lot of NOLINT's in it. Because it's someone else's code, I wanted
// to be conservitive in changing it. It's easier to hide the lint than modify the code

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
        // m_lockingUser = "";
        // m_mode = "";
        // m_errorMsg = "";
    }

    virtual ~CSharedResource(void) {
        Release();
    }

    // forces implementation
    virtual string_q getType(void) const = 0;

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

    virtual bool Upgrade(void) {
        return false;  // did not upgrade anything
    }

    string_q getFilename(void) const {
        return m_filename;
    }
    void setFilename(const string_q& fn) {
        m_filename = fn;
    }

  public:
    size_t Read(bool& val);
    size_t Read(char& val);
    size_t Read(int& val);
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

//----------------------------------------------------------------------
extern string_q asciiFileToString(const string_q& filename);
extern size_t asciiFileToString(const string_q& filename, string& contents);  // non-copy
extern size_t asciiFileToLines(const string_q& fileName, CStringArray& lines);
extern size_t asciiFileToLines(const string_q& fileName, CUintArray& lines);
extern size_t stringToAsciiFile(const string_q& fileName, const string_q& contents);
extern size_t linesToAsciiFile(const string_q& fileName, const CStringArray& lines, char sep = '\n');
extern string_q linesToString(const CStringArray& lines, char sep = '\n');
extern size_t appendToAsciiFile(const string_q& fileName, const string_q& addContents);
extern bool forEveryLineInAsciiFile(const string_q& filename, CHARPTRFUNC func, void* data);

//----------------------------------------------------------------------
extern string_q docxToString(const string_q& filename);
extern size_t stringToDocxFile(const string_q& fileName, const string_q& contents);
extern size_t stringToPDF(const string_q& fileName, const string_q& contents);
extern string_q excelFileToString(const string_q& filename);

//----------------------------------------------------------------------
typedef void (*QUITHANDLER)(int s);
extern void defaultQuitHandler(int s);
extern void quickQuitHandler(int signum);
extern void registerQuitHandler(QUITHANDLER qh);
extern void cleanFileLocks(void);

//----------------------------------------------------------------------
class codewrite_t {
  public:
    string_q fileName, codeOutIn, namespc;
    uint32_t nSpaces;
    bool testing, stripEOFNL, force;
    codewrite_t(const string_q& fn, const string_q& c, const string_q& n, uint32_t ns, bool t, bool s = true,
                bool f = false)
        : fileName(fn), codeOutIn(c), namespc(n), nSpaces(ns), testing(t), stripEOFNL(s), force(f) {
    }
};
extern bool shouldQuit(void);
extern void lockSection(void);
extern void unlockSection(void);
extern bool writeTheCode(const codewrite_t& cw);

//----------------------------------------------------------------------
inline bool isFileLocked(const string_q& fileName) {
    return fileExists(fileName + ".lck");
}

// Generic binary file
class CBinFile : public CSharedResource {
  public:
    string_q getType(void) const override {
        return "CBinFile";
    }
};

// Generic ascii file
class CAsciiFile : public CSharedResource {
  public:
    string_q getType(void) const override {
        return "CAsciiFile";
    }
};

extern string_q manageRemoveList(const string_q& filename = "");
}  // namespace qblocks
