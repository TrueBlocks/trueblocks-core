#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "fielddata.h"
#include "sftime.h"

namespace qblocks {

    #define READ_OK  1
    #define READ_EOF 2
    #define READ_BAD 3

    #define asciiReadOnly     "r"    // ascii read - fails if not present
    #define asciiReadWrite    "r+"   // ascii read-write (file must exist)
    #define asciiWriteCreate  "w"    // ascii writing - destroys previous contents or creates
    #define asciiWriteAppend  "a+"   // ascii read/writing - appends

    #define binaryReadOnly    "rb"   // binary read - fails if not present
    #define binaryReadWrite   "rb+"  // binary read/write - fails if not present
    #define binaryWriteCreate "wb"   // binary write - destroys previous contents or creates
    #define binaryWriteAppend "ab+"  // binary read/writing - appends

    #define LOCK_NOWAIT       1      // read only - do not even check for a lock
    #define LOCK_WAIT         2      // Wait for lock to release return true - if wait too long return false
    #define LOCK_CREATE       3      // Wait for lock to release - if waiting longer than preset time destroy
                                     // existing lock and create a new one

    //------------------------------------------------------------------------
    // A file that must be locked before being accessed (such as a datafile)
    //------------------------------------------------------------------------
    class CSharedResource {
    private:
        SFString m_mode;
        SFString m_errorMsg;
        size_t   m_error;
        bool     m_ownsLock;
        SFString m_lockingUser;
        bool     m_isascii;

    protected:
        SFString m_filename;

    public:
        FILE *m_fp;

        CSharedResource(void) {
            m_fp       = NULL;
            m_error    = 0;
            m_ownsLock = false;
            // m_lockingUser = "";
            // m_mode = "";
            // m_errorMsg = "";
            m_isascii  = false;
        }

        virtual ~CSharedResource(void) {
            Release();
        }

        // forces implementation
        virtual SFString getType(void) const = 0;

        bool Lock(const SFString& fn, const SFString& mode, uint32_t obeyLock);
        bool ReLock(const SFString& mode);
        void Release(void);
        void Close(void);

        bool isOpen(void) const {
            return (m_fp != NULL);
        }

        long Tell(void) const;
        void Seek(long offset, int whence) const;
        bool Eof(void) const;
        char *ReadLine(char *buff, size_t maxBuff);
        void WriteLine(const SFString& str);
        SFString LockFailure(void) const;
        void flush(void) { if (m_fp) fflush(m_fp); }

        virtual bool Upgrade(void) {
            return false;  // did not upgrade anything
        }

        SFString getFilename(void) const { return m_filename; }
        void setFilename(const SFString& fn) { m_filename = fn; }

        // Use only for cases where file deletion does not work
        static bool setLocking(bool val);

        bool isAscii(void) const {
            return m_isascii;
        }

    public:
        size_t Read(bool& val);
        size_t Read(char& val);
        size_t Read(int& val);
        size_t Read(unsigned int& val);
        size_t Read(long& val);
        size_t Read(unsigned long& val);
        size_t Read(long long& val);
        size_t Read(unsigned long long& val);
        size_t Read(float& val);
        size_t Read(double& val);
        size_t Read(SFString& val);
        size_t Read(SFTime& val);

        size_t Write(bool val) const;
        size_t Write(char val) const;
        size_t Write(int val) const;
        size_t Write(unsigned int val) const;
        size_t Write(long val) const;
        size_t Write(unsigned long val) const;
        size_t Write(long long val) const;
        size_t Write(unsigned long long val) const;
        size_t Write(float val) const;
        size_t Write(double val) const;
        size_t Write(const SFString& val) const;
        size_t Write(const SFTime& val) const;

        size_t Read(void *buff, size_t size, size_t cnt);
        size_t Write(const void *buff, size_t size, size_t cnt) const;

    private:
        size_t Read(const char *val) const;
        size_t Write(const char *val) const;

        bool waitOnLock(bool deleteOnFail) const;
        bool createLockFile(const SFString& lockfilename);
        bool createLock(bool createOnFail);

        CSharedResource(const CSharedResource& l);
        CSharedResource& operator=(const CSharedResource& l);

        // Turns on and off file locking for machines that do not allow file delete such as my ISP
        static bool g_locking;  // = true;
    };

    //----------------------------------------------------------------------
    extern size_t stringToAsciiFile(const SFString& fileName, const SFString& contents);
    extern SFString binaryFileToString(const SFString& filename);
    extern bool asciiFileToBuffer(const SFString& filename, size_t& nChars, SFString *buffer,
                                  uint32_t maxLines = INT_MAX);
    extern uint64_t appendToAsciiFile(const SFString& fileName, const SFString& addContents);

    //----------------------------------------------------------------------
    extern SFString docxToString(const SFString& filename);
    extern size_t stringToDocxFile(const SFString& fileName, const SFString& contents);
    extern size_t stringToPDF(const SFString& fileName, const SFString& contents);
    extern SFString excelFileToString(const SFString& filename);

    //----------------------------------------------------------------------
    typedef void (*QUITHANDLER) (int s);
    extern void  defaultQuitHandler (int s);
    extern void  quickQuitHander (int signum);
    extern void  cleanFileLocks (void);

    //----------------------------------------------------------------------
    extern bool shouldQuit(void);
    extern void lockSection(bool lock);

    //----------------------------------------------------------------------
    inline bool asciiFileToBuffer(const SFString& filename, SFString *contents, uint32_t maxLines = INT_MAX) {
        size_t unused = 0;
        return asciiFileToBuffer(filename, unused, contents, maxLines);
    }

    //----------------------------------------------------------------------
    inline SFString asciiFileToString(const SFString& filename) {
        SFString contents;
        asciiFileToBuffer(filename, &contents);
        return contents;
    }

    inline bool isFileLocked(const SFString& fileName) {
        return fileExists(fileName + ".lck");
    }

    // Generic binary file
    class CBinFile : public CSharedResource {
    public:
        SFString getType(void) const override { return "CBinFile"; }
    };

    // Generic ascii file
    class CAsciiFile : public CSharedResource {
    public:
        SFString getType(void) const override { return "CAsciiFile"; }
    };
}  // namespace qblocks
