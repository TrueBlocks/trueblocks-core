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
#include "basetypes.h"

#include "database.h"
#include "dates.h"
#include "sfos.h"
#include "filenames.h"
#include "conversions.h"

namespace qblocks {

    #define maxSecondsLock 3

    #define LK_NO_CREATE_LOCK_FILE 1
    #define LK_FILE_NOT_EXIST      2
    #define LK_BAD_OPEN_MODE       3
    #define LK_NO_REMOVE_LOCK      4

    //----------------------------------------------------------------------
    extern SFString manageRemoveList(const SFString& filename="");
    extern uint32_t quitCount(uint32_t s=0);
    bool CSharedResource::g_locking = true;

    //----------------------------------------------------------------------
    bool CSharedResource::setLocking(bool val) {
        bool ret = g_locking;
        g_locking = val;
        return ret;
    }

    //----------------------------------------------------------------------
    bool CSharedResource::createLockFile(const SFString& lockfilename) {
        if (!g_locking)
            return true;

        m_ownsLock = false;
        FILE *fp = fopen((const char *)lockfilename, asciiWriteCreate);
        if (fp) {
            fprintf(fp, "%s\n", (const char *)m_lockingUser);
            fclose(fp);
            m_ownsLock = true;
            manageRemoveList(lockfilename);
            return m_ownsLock;
        }
        m_error = LK_NO_CREATE_LOCK_FILE;
        m_errorMsg = "Could not create lock file: " + lockfilename;
        return false;  // why could I not create the lock file?
    }

    //----------------------------------------------------------------------
    bool CSharedResource::createLock(bool createOnFail) {
        if (!g_locking)
            return true;

        SFString lockFilename = m_filename + ".lck";

        int i = 0;
        while (i < maxSecondsLock) {
            if (!fileExists(lockFilename))
                return createLockFile(lockFilename);
            usleep(1000000);
            i++;
        }

        // Someone has had the lock for maxSecondsLock seconds -- if told to blow that lock away
        if (createOnFail) {
            // Release the old lock and create a new one
            m_ownsLock = true;
            Release();
            return createLockFile(lockFilename);
        }

        return false;
    }

    //----------------------------------------------------------------------
    bool CSharedResource::waitOnLock(bool deleteOnFail) const {
        if (!g_locking)
            return true;

        SFString lockFilename = m_filename + ".lck";

        if (fileExists(lockFilename) && !isTestMode())
            cerr << "Waiting for lock to clear\n";

        int i = 0;
        while (i < maxSecondsLock) {
            if (!fileExists(lockFilename))
                return true;
            usleep(1000000);
            i++;
        }

        // Someone has had the lock for maxSecondsLock seconds -- if told to blow that lock away
        if (deleteOnFail) {
            // Release the old lock and create a new one
            bool owns = m_ownsLock;
            ((CSharedResource*)this)->m_ownsLock = true;  // NOLINT
            ((CSharedResource*)this)->Release();          // NOLINT
            ((CSharedResource*)this)->m_ownsLock = owns;  // NOLINT
            cerr << "Lock cleared...\n";
            return true;
        }

        return false;
    }

    //----------------------------------------------------------------------
    static bool isAscii(const SFString& mode) {
        return (mode % asciiReadOnly ||
                mode % asciiReadWrite ||
                mode % asciiWriteCreate ||
                mode % asciiWriteAppend);
    }

    //----------------------------------------------------------------------
    bool CSharedResource::ReLock(const SFString& mode) {
        ASSERT(isOpen());
        ASSERT(m_ownsLock);

        // Close and re-open the file without relinqishing the lock
        Close();
        m_fp = fopen((const char *)m_filename, mode);
        m_isascii = qblocks::isAscii(mode);

        return isOpen();
    }

    //----------------------------------------------------------------------
    bool CSharedResource::Lock(const SFString& fn, const SFString& mode, uint32_t lockType) {
        ASSERT(!isOpen());

        m_filename = fn;
        m_mode = mode;
        m_fp = NULL;

        // If the file we are trying to lock does not exist but we are not trying to open
        // it under one of the create modes then do not create a lock, do not open the file,
        // and let the user know.
        if (!fileExists(m_filename) &&
            (m_mode != asciiWriteCreate && m_mode != asciiWriteAppend && m_mode != binaryWriteCreate)
            ) {
            m_error = LK_FILE_NOT_EXIST;
            m_errorMsg = "File does not exist: " + m_filename;
            return false;
        }

        bool openIt = true;
        if (m_mode == binaryReadOnly || m_mode == asciiReadOnly) {

            // Wait for lock to clear...
            if (lockType == LOCK_WAIT)
                waitOnLock(true);

            // ... proceed even if it doesn't....
            openIt = true;

        } else if (m_mode == binaryReadWrite || m_mode == binaryWriteCreate ||
                   m_mode == asciiReadWrite  || m_mode == asciiWriteAppend  ||
                   m_mode == asciiWriteCreate) {

            ASSERT(lockType == LOCK_CREATE || lockType == LOCK_WAIT);
            openIt = createLock(lockType != LOCK_WAIT);
            if (!openIt) {
                m_error = LK_NO_CREATE_LOCK_FILE;
                m_errorMsg = "Could not create lock file: " + m_filename + ".lck";
            }

        } else {
            m_error = LK_BAD_OPEN_MODE;
            m_errorMsg = "Bad file open mode: " + m_mode;
            return false;
        }

        if (openIt) {
            m_fp = fopen((const char *)m_filename, (const char *)m_mode);  // operator on event database
            m_isascii = qblocks::isAscii(m_mode);
        }

        return isOpen();
    }

    //----------------------------------------------------------------------
    void CSharedResource::Release(void) {
        Close();

        if (g_locking && m_ownsLock) {
            SFString lockFilename = m_filename + ".lck";
            bool ret = remove(lockFilename.c_str());
            manageRemoveList("r:"+lockFilename);
            if (ret != 0) {
//              fprintf(stdout, "What happened?: %d: %s\n", ret, (const char *)(m_filename + ".lck"));
                m_error = LK_NO_REMOVE_LOCK;
                m_errorMsg = "Could not remove lock";
            } else {
//              fprintf(stdout, "Removed it: %s\n", (const char *)(m_filename + ".lck"));
            }
        }

        m_ownsLock = false;
    }

    //----------------------------------------------------------------------
    void CSharedResource::Close(void) {
        if (m_fp) {
            fflush(m_fp);
            fclose(m_fp);
        }
        m_fp = NULL;
        m_isascii = false;
    }

    //----------------------------------------------------------------------
    SFString CSharedResource::LockFailure(void) const {
        // In some cases (for example when can't open event file because it has not yet
        // been created) this may not be an error -- Lock should set an error flag
        // which this guy should read and do right
        return asStringU(m_error) + ": " + m_errorMsg;
    }

    //----------------------------------------------------------------------
    static bool binaryFileToBuffer(const SFString& filename, uint32_t& nChars, char *buffer) {
        if (!fileExists(filename)) {
            nChars = 0;
            if (buffer)
                buffer[0] = '\0';
            return false;
        }

        nChars = (uint32_t)fileSize(filename);
        if (buffer) {
            CBinFile lock;
            if (lock.Lock(filename, binaryReadOnly, LOCK_NOWAIT)) {  // do not wait for lock - read only file
                ASSERT(lock.isOpen());
                lock.Seek(0, SEEK_SET);
                lock.Read(buffer, nChars, sizeof(char));
                buffer[nChars] = '\0';
                lock.Release();
            } else {
                return false;
            }
        }
        return true;
    }

    //----------------------------------------------------------------------
    SFString binaryFileToString(const SFString& filename) {
        SFString ret; uint32_t nChars = 0;
        if (binaryFileToBuffer(filename, nChars, NULL)) {
            char *buffer = new char[nChars + 100];  // safty factor
            if (binaryFileToBuffer(filename, nChars, buffer))
                ret = buffer;
            if (buffer)
                delete [] buffer;
        }
        return ret;
    }
//    extern bool binaryFileToBuffer(const SFString& filename, uint32_t& nChars, char *buffer);
    //----------------------------------------------------------------------
    bool CSharedResource::Eof(void) const {
        ASSERT(isOpen());
        return feof(m_fp);
    }

    //----------------------------------------------------------------------
    void CSharedResource::Seek(long offset, int whence) const {
        ASSERT(isOpen());
        fseek(m_fp, offset, whence);
    }

    //----------------------------------------------------------------------
    long CSharedResource::Tell(void) const {
        ASSERT(isOpen());
        return ftell(m_fp);
    }

    //----------------------------------------------------------------------
    size_t CSharedResource::Read(void *buff, size_t size, size_t cnt) {
        ASSERT(isOpen());
        return fread(buff, cnt, size, m_fp);
    }

    //----------------------------------------------------------------------
    size_t CSharedResource::Read(bool& val) { return Read(&val, sizeof(bool), 1); }
    size_t CSharedResource::Read(char& val) { return Read(&val, sizeof(char), 1); }
    size_t CSharedResource::Read(int& val) { return Read(&val, sizeof(int), 1); }
    size_t CSharedResource::Read(unsigned int& val) { return Read(&val, sizeof(int), 1); }
    size_t CSharedResource::Read(long& val) { return Read(&val, sizeof(long), 1); }
    size_t CSharedResource::Read(unsigned long& val) { return Read(&val, sizeof(long), 1); }
    size_t CSharedResource::Read(long long& val) { return Read(&val, sizeof(long long), 1); }
    size_t CSharedResource::Read(unsigned long long& val) { return Read(&val, sizeof(long long), 1); }
    size_t CSharedResource::Read(float& val) { return Read(&val, sizeof(float), 1); }
    size_t CSharedResource::Read(double& val) { return Read(&val, sizeof(double), 1); }
    size_t CSharedResource::Read(SFTime& val) { return Read(&val.m_nSeconds, sizeof(long), 1); }

    //----------------------------------------------------------------------
    size_t CSharedResource::Read(SFString& str) {
        ASSERT(isOpen());
        ASSERT(!isAscii());

        unsigned long len;
        Read(&len, sizeof(unsigned long), 1);

        if (len < 8192) {
            char buff[8192];
            Read(buff, sizeof(char), len);
            buff[len] = '\0';
            str = buff;
        } else {
            char *buff = new char[len+1];
            Read(buff, sizeof(char), len);
            buff[len] = '\0';
            str = buff;
            delete [] buff;
        }

        return len;
    }

    //----------------------------------------------------------------------
    char *CSharedResource::ReadLine(char *buff, size_t maxBuff) {
        ASSERT(isOpen());
        ASSERT(isAscii());
        return fgets(buff, static_cast<int>(maxBuff), m_fp);
    }

    //----------------------------------------------------------------------
    size_t CSharedResource::Write(const void *buff, size_t size, size_t cnt) const {
        ASSERT(isOpen());
        return fwrite(buff, size, cnt, m_fp);
    }

    //----------------------------------------------------------------------
    size_t CSharedResource::Write(bool val) const { return Write(&val, sizeof(bool), 1); }
    size_t CSharedResource::Write(char val) const { return Write(&val, sizeof(char), 1); }
    size_t CSharedResource::Write(int val) const { return Write(&val, sizeof(int), 1); }
    size_t CSharedResource::Write(unsigned int val) const { return Write(&val, sizeof(int), 1); }
    size_t CSharedResource::Write(long val) const { return Write(&val, sizeof(long), 1); }
    size_t CSharedResource::Write(unsigned long val) const { return Write(&val, sizeof(long), 1); }
    size_t CSharedResource::Write(long long val) const { return Write(&val, sizeof(long long), 1); }
    size_t CSharedResource::Write(unsigned long long     val) const { return Write(&val, sizeof(long long), 1); }
    size_t CSharedResource::Write(float val) const { return Write(&val, sizeof(float), 1); }
    size_t CSharedResource::Write(double val) const { return Write(&val, sizeof(double), 1); }
    size_t CSharedResource::Write(const SFTime& val) const { return Write(&val.m_nSeconds, sizeof(long), 1); }

    //----------------------------------------------------------------------
    size_t CSharedResource::Write(const SFString& val) const {
        ASSERT(isOpen());
        ASSERT(!isAscii());

        unsigned long len = val.length();
        size_t ret = Write(&len, sizeof(unsigned long), 1);
        return Write((const char *)val, sizeof(char), len) + ret;
    }

    //----------------------------------------------------------------------
    void CSharedResource::WriteLine(const SFString& str) {
        ASSERT(isOpen());
        ASSERT(isAscii());
        fprintf(m_fp, "%s", (const char *)str);
    }

    //----------------------------------------------------------------------
    bool asciiFileToBuffer(const SFString& filename, size_t& nChars, SFString *buffer, uint32_t maxLines) {
        if (!fileExists(filename) || folderExists(filename)) {
            nChars = 0;
            if (buffer)
                *buffer = EMPTY;
            return false;
        }

        size_t nBytes = fileSize(filename);

        // Allocate a enough space to store the file so its not realloced each
        // time we do +=
        char *val = new char[nBytes+100];
        memset(val, 0, nBytes+100);
        char *s = val;

        CAsciiFile lock;
        if (lock.Lock(filename, asciiReadOnly, LOCK_NOWAIT)) {  // do not wait for lock - read only file
            uint64_t nLines = 0;

            ASSERT(lock.isOpen());
            char buff[4096];
            while (nLines < maxLines && lock.ReadLine(buff, 4096)) {
                nChars += strlen(buff);
                if (buffer) {
                    size_t len = strlen(buff);
                    strncpy(s, buff, len);
                    s += len;
                }

                nLines++;
            }

            lock.Release();
        } else {
            // ...and not report any errors since errors will have already been reported by InstallCheck
            // lock.LockFailure();
            if (val)
                delete [] val;
            return false;
        }

        ASSERT(s);
        *s = '\0';

        if (buffer && val)
            *buffer = val;

        if (val)
            delete [] val;

        return true;
    }

    //----------------------------------------------------------------------
    SFString excelFileToString(const SFString& excelFilename) {
        if (excelFilename.Contains(".xlsx"))
            return "Only .xls Excel files are supported";
        return doCommand("exportExcel " + excelFilename);
    }

    //----------------------------------------------------------------------
    SFString docxToString(const SFString& filename) {
        if (!filename.Contains(".docx"))
            return "Only .docx files are supported";
        return doCommand(getHomeFolder() + "source/docx2txt.pl " +
                               filename.Substitute(" ", "\\ ").Substitute("'", "\\'") + " -");
    }

    //----------------------------------------------------------------------
    size_t stringToDocxFile(const SFString& fileName, const SFString& contents) {
        SFString cmd = getHomeFolder() + "source/createDocx \"" +
                        fileName + "\" \"" + contents.Substitute("\"", "''") + "\"";
        SFString ret = doCommand(cmd);
        fprintf(stderr, "ret: %s\n", (const char*)ret);
        return true;
    }

    //----------------------------------------------------------------------
    size_t stringToPDF(const SFString& fileName, const SFString& contents) {
        SFString tmpName = "/tmp/toPDF.txt";
        SFString pdfName = "/tmp/toPDF.pdf";
        stringToAsciiFile(tmpName, contents);

        SFString cmd = getHomeFolder() + "source/toPDF \"" + tmpName + "\" \"" + pdfName + "\" 2>/dev/null";
        doCommand(cmd);
        copyFile(pdfName, fileName);

        remove(tmpName.c_str());
        remove(pdfName.c_str());

        return true;
    }

    //----------------------------------------------------------------------
    size_t stringToAsciiFile(const SFString& fileName, const SFString& contents) {
        CAsciiFile lock;
        if (lock.Lock(fileName, asciiWriteCreate, LOCK_WAIT)) {
            lock.WriteLine((const char*)contents);
            lock.Release();
        } else {
            fprintf(stderr, "%s\n", (const char*)SFString("Could not open file: " + fileName));
            return false;
        }
        return true;
    }

    //------------------------------------------------------------------------------------------------------------
    void writeTheCode(const SFString& fileName, const SFString& codeOutIn, const SFString& ns, bool spaces) {
        SFString codeOut = codeOutIn;
        SFString orig = asciiFileToString(fileName);
        SFString existingCode = orig.Substitute("//EXISTING_CODE","// EXISTING_CODE");
        if (spaces) {
            existingCode.ReplaceAll("    ", "\t");
            codeOut     .ReplaceAll("    ", "\t");
        }

        SFString tabs;
        int nTabs = 4;
        while (nTabs >= 0) {
            tabs = SFString('\t', (size_t)nTabs);
            nTabs--;
            //--------------------------------------------------------------------------------------
            while (existingCode.Contains(tabs + "// EXISTING_CODE")) {
                existingCode.Replace(tabs + "// EXISTING_CODE", "<code>");
                existingCode.Replace(tabs + "// EXISTING_CODE", "</code>");
            }
            while (existingCode.Contains("</code>")) {
                SFString snipit = Strip(snagFieldClear(existingCode, "code"), '\n');
                codeOut.Replace(tabs + "// EXISTING_CODE\n" + tabs + "// EXISTING_CODE",
                                tabs + "// EXISTING_CODE\n" + snipit + "\n" + tabs + "// EXISTING_CODE");
            }
            codeOut.ReplaceAll("// EXISTING_CODE\n\n" + tabs + "// EXISTING_CODE", "// EXISTING_CODE\n" +
                               tabs + "// EXISTING_CODE");
            //--------------------------------------------------------------------------------------
        }

        // One final cleanup
        codeOut.ReplaceAll("\n\n}", "\n}");
        codeOut.ReplaceAll("\n\n\n", "\n\n");

        if (spaces)
            codeOut.ReplaceAll("\t", "    ");

        if (codeOut.Contains("virtual") || codeOut.Contains("override"))
            codeOut.Replace("~", "virtual ~");

        // If we don't write it because it's identical, it won't force a rebuild
        if (orig != codeOut)
            stringToAsciiFile(fileName, codeOut);
    }

    //-----------------------------------------------------------------------
    static bool sectionLocked = false;
    void lockSection(bool lock) {
        sectionLocked = lock;
    }

    //-----------------------------------------------------------------------
    uint32_t quitCount(uint32_t s) {
        static uint32_t cnt = 0;
        if (cnt && sectionLocked) // ignore if we're locked
            return false;
        cnt += s;
        return cnt;
    }

    //-----------------------------------------------------------------------
    bool shouldQuit(void) {
        bool ret = quitCount() > 1;
        if (ret) {
            cout << "\nFinishing work...Hit Cntl+C again to quit...\n";
            cleanFileLocks();
            cout.flush();
        }
        return ret;
    }

    //-----------------------------------------------------------------------
    void cleanFileLocks(void) {
        SFString list = manageRemoveList();
        while (!list.empty()) {
            SFString file = nextTokenClear(list, '|');
            remove(file.c_str());
            cerr << "Removing file: " << file << "\n";
            cerr.flush();
        }
        manageRemoveList("clear");
    }

    //-----------------------------------------------------------------------
    void defaultQuitHandler(int signum) {
        if (quitCount(1) > 2) {
            cleanFileLocks();
            if (signum != -1)
                exit(1);
        }
    }

    //-----------------------------------------------------------------------
    void quickQuitHander(int signum) {
        cleanFileLocks();
        if (signum != -1)
            exit(1);
    }

    //-----------------------------------------------------------------------
    void registerQuitHandler(QUITHANDLER qh) {

        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = qh;
        sigemptyset (&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction (SIGINT,  &sigIntHandler, NULL);
        sigaction (SIGTERM, &sigIntHandler, NULL);
    }

    //-----------------------------------------------------------------------
    SFString manageRemoveList(const SFString& filename) {
        static SFString theList;
        if (filename == "clear") {
            theList = "";
            return "";
        }

        SFString fn = filename.Substitute("r:", "");;
        if (!fn.empty()) {
            if (filename.startsWith("r:")) {
                theList.Replace(fn+"|", "");

            } else if (!theList.Contains(fn+"|")) {
                theList += (filename + "|");

            }
        }
        return theList;
    }
}  // namespace qblocks
