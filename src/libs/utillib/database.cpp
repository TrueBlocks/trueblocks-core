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
// NOTE: This file has a lot of NOLINT's in it. Because it's someone else's code, I wanted
// to be conservitive in changing it. It's easier to hide the lint than modify the code

#include "basetypes.h"

#include "database.h"
#include "sfos.h"
#include "filenames.h"
#include "conversions.h"
#include "logging.h"
#include "options_base.h"

namespace qblocks {

#define maxSecondsLock 3

#define LK_NO_CREATE_LOCK_FILE 1
#define LK_FILE_NOT_EXIST 2
#define LK_BAD_OPEN_MODE 3
#define LK_NO_REMOVE_LOCK 4

//----------------------------------------------------------------------
extern string_q manageRemoveList(const string_q& filename = "");
extern size_t quitCount(size_t s = 0);

//----------------------------------------------------------------------
bool CSharedResource::createLockFile(const string_q& lockfilename) {
    m_ownsLock = false;
    FILE* fp = fopen(lockfilename.c_str(), modeWriteCreate);
    if (fp) {
        fprintf(fp, "%s\n", m_lockingUser.c_str());
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
    string_q lockFilename = m_filename + ".lck";

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
    string_q lockFilename = m_filename + ".lck";

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
        CSharedResource* pResource = (CSharedResource*)this;  // NOLINT
        pResource->m_ownsLock = true;
        pResource->Release();
        pResource->m_ownsLock = owns;
        if (!isTestMode())
            cerr << "Lock cleared...\n";
        return true;
    }

    return false;
}

//----------------------------------------------------------------------
bool CSharedResource::ReLock(const string_q& mode) {
    ASSERT(isOpen());
    ASSERT(m_ownsLock);

    // Close and re-open the file without relinqishing the lock
    Close();
    m_fp = fopen(m_filename.c_str(), mode.c_str());

    return isOpen();
}

//----------------------------------------------------------------------
bool CSharedResource::Lock(const string_q& fn, const string_q& mode, size_t lockType) {
    ASSERT(!isOpen());

    m_filename = fn;
    m_mode = mode;
    m_fp = NULL;

    // If the file we are trying to lock does not exist but we are not trying to open
    // it under one of the create modes then do not create a lock, do not open the file,
    // and let the user know.
    if (!fileExists(m_filename) && (m_mode != modeWriteCreate && m_mode != modeWriteAppend)) {
        m_error = LK_FILE_NOT_EXIST;
        m_errorMsg = "File does not exist: " + m_filename;
        return false;
    }

    bool openIt = true;
    if (m_mode == modeReadOnly) {
        // Wait for lock to clear...
        if (lockType == LOCK_WAIT)
            waitOnLock(true);

        // ... proceed even if it doesn't....
        openIt = true;

    } else if (m_mode == modeReadWrite || m_mode == modeWriteAppend || m_mode == modeWriteCreate) {
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
        m_fp = fopen(m_filename.c_str(), m_mode.c_str());  // operator on event database
    }

    return isOpen();
}

//----------------------------------------------------------------------
void CSharedResource::Release(void) {
    Close();

    if (m_ownsLock) {
        string_q lockFilename = m_filename + ".lck";
        bool ret = remove(lockFilename.c_str());
        manageRemoveList("r:" + lockFilename);
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
}

//----------------------------------------------------------------------
string_q CSharedResource::LockFailure(void) const {
    // In some cases (for example when can't open event file because it has not yet
    // been created) this may not be an error -- Lock should set an error flag
    // which this guy should read and do right
    return uint_2_Str(m_error) + ": " + m_errorMsg;
}

//----------------------------------------------------------------------
bool CSharedResource::Eof(void) const {
    ASSERT(isOpen());
    return feof(m_fp);
}

//----------------------------------------------------------------------
void CSharedResource::Seek(long offset, int whence) const {  // NOLINT
    ASSERT(isOpen());
    fseek(m_fp, offset, whence);
}

//----------------------------------------------------------------------
long CSharedResource::Tell(void) const {  // NOLINT
    ASSERT(isOpen());
    return ftell(m_fp);
}

//----------------------------------------------------------------------
size_t CSharedResource::Read(void* buff, size_t size, size_t cnt) {
    ASSERT(isOpen());
    return fread(buff, cnt, size, m_fp);
}

//----------------------------------------------------------------------
// clang-format off
size_t CSharedResource::Read(bool& val) { return Read(&val, sizeof(bool), 1); }
size_t CSharedResource::Read(char& val) { return Read(&val, sizeof(char), 1); }
size_t CSharedResource::Read(int& val) { return Read(&val, sizeof(int), 1); }
size_t CSharedResource::Read(unsigned int& val) { return Read(&val, sizeof(int), 1); }
size_t CSharedResource::Read(long& val) { return Read(&val, sizeof(long), 1); }  // NOLINT
size_t CSharedResource::Read(unsigned long& val) { return Read(&val, sizeof(long), 1); }  // NOLINT
size_t CSharedResource::Read(long long& val) { return Read(&val, sizeof(long long), 1); }  // NOLINT
size_t CSharedResource::Read(unsigned long long& val) { return Read(&val, sizeof(long long), 1); }  // NOLINT
size_t CSharedResource::Read(float& val) { return Read(&val, sizeof(float), 1); }
size_t CSharedResource::Read(double& val) { return Read(&val, sizeof(double), 1); }
// clang-format on

//----------------------------------------------------------------------
size_t CSharedResource::Read(string_q& str) {
    ASSERT(isOpen());

    unsigned long len;                     // NOLINT
    Read(&len, sizeof(unsigned long), 1);  // NOLINT

    if (len < 8192) {
        char buff[8192];
        Read(buff, sizeof(char), len);
        buff[len] = '\0';
        str = buff;
    } else {
        char* buff = new char[len + 1];
        Read(buff, sizeof(char), len);
        buff[len] = '\0';
        str = buff;
        delete[] buff;
    }

    return len;
}

//----------------------------------------------------------------------
char* CSharedResource::ReadLine(char* buff, size_t maxBuff) {
    ASSERT(isOpen());
    return fgets(buff, static_cast<int>(maxBuff), m_fp);
}

//----------------------------------------------------------------------
size_t CSharedResource::Write(const void* buff, size_t size, size_t cnt) const {
    ASSERT(isOpen());
    return fwrite(buff, size, cnt, m_fp);
}

//----------------------------------------------------------------------
// clang-format off
size_t CSharedResource::Write(bool val) const { return Write(&val, sizeof(bool), 1); }
size_t CSharedResource::Write(char val) const { return Write(&val, sizeof(char), 1); }
size_t CSharedResource::Write(int val) const { return Write(&val, sizeof(int), 1); }
size_t CSharedResource::Write(unsigned int val) const { return Write(&val, sizeof(int), 1); }
size_t CSharedResource::Write(long val) const { return Write(&val, sizeof(long), 1); }  // NOLINT
size_t CSharedResource::Write(unsigned long val) const { return Write(&val, sizeof(long), 1); }  // NOLINT
size_t CSharedResource::Write(long long val) const { return Write(&val, sizeof(long long), 1); }  // NOLINT
size_t CSharedResource::Write(unsigned long long val) const { return Write(&val, sizeof(long long), 1); }  // NOLINT
size_t CSharedResource::Write(float val) const { return Write(&val, sizeof(float), 1); }
size_t CSharedResource::Write(double val) const { return Write(&val, sizeof(double), 1); }
// clang-format on

//----------------------------------------------------------------------
size_t CSharedResource::Write(const string_q& val) const {
    ASSERT(isOpen());

    unsigned long len = val.length();                    // NOLINT
    size_t ret = Write(&len, sizeof(unsigned long), 1);  // NOLINT
    return Write(val.c_str(), sizeof(char), len) + ret;
}

//----------------------------------------------------------------------
void CSharedResource::WriteLine(const string_q& str) {
    ASSERT(isOpen());
    fprintf(m_fp, "%s", str.c_str());
}

//----------------------------------------------------------------------
string_q excelFileToString(const string_q& excelFilename) {
    if (contains(excelFilename, ".xlsx"))
        return "Only .xls Excel files are supported";
    return doCommand("exportExcel " + excelFilename);
}

//----------------------------------------------------------------------
string_q docxToString(const string_q& filename) {
    if (!contains(filename, ".docx"))
        return "Only .docx files are supported";
    return doCommand(getHomeFolder() + "source/docx2txt.pl " +
                     substitute(substitute(filename, " ", "\\ "), "'", "\\'") + " -");
}

//----------------------------------------------------------------------
size_t stringToDocxFile(const string_q& fileName, const string_q& contents) {
    string_q cmd =
        getHomeFolder() + "source/createDocx \"" + fileName + "\" \"" + substitute(contents, "\"", "''") + "\"";
    string_q ret = doCommand(cmd);
    cerr << "ret: " << ret << endl;
    return true;
}

//----------------------------------------------------------------------
size_t stringToPDF(const string_q& fileName, const string_q& contents) {
    string_q tmpName = "/tmp/toPDF.txt";
    string_q pdfName = "/tmp/toPDF.pdf";
    stringToAsciiFile(tmpName, contents);

    string_q cmd = getHomeFolder() + "source/toPDF \"" + tmpName + "\" \"" + pdfName + "\" 2>/dev/null";
    doCommand(cmd);
    copyFile(pdfName, fileName);

    remove(tmpName.c_str());
    remove(pdfName.c_str());

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool writeTheCode(const codewrite_t& cw) {
    string_q codeOut = cw.codeOutIn;
    string_q orig;
    asciiFileToString(cw.fileName, orig);
    string_q existingCode = substitute(orig, "//EXISTING_CODE", "// EXISTING_CODE");

    string_q checkCode = existingCode;
    uint64_t cnt = 0;
    while (contains(checkCode, "// EXISTING_CODE")) {
        replace(checkCode, "// EXISTING_CODE", "");
        cnt++;
    }
    if ((cnt % 2))
        codeOut = "#error \"Uneven number of EXISTING_CODE blocks in the file.\"\n" + codeOut;
    if (cw.nSpaces) {
        replaceAll(existingCode, string_q(cw.nSpaces, ' '), "\t");
        replaceAll(codeOut, string_q(cw.nSpaces, ' '), "\t");
    }

    string_q tabs;
    int nTabs = 6;
    while (nTabs >= 0) {
        tabs = string_q((size_t)nTabs, '\t');
        nTabs--;
        //--------------------------------------------------------------------------------------
        while (contains(existingCode, tabs + "// EXISTING_CODE")) {
            replace(existingCode, tabs + "// EXISTING_CODE", "<code>");
            replace(existingCode, tabs + "// EXISTING_CODE", "</code>");
        }
        while (contains(existingCode, "</code>")) {
            string_q snipit = trim(snagFieldClear(existingCode, "code"), '\n');
            string_q r1 = tabs + "// EXISTING_CODE\n" + tabs + "// EXISTING_CODE";
            string_q r2 = tabs + "// EXISTING_CODE\n" + snipit + "\n" + tabs + "// EXISTING_CODE";
            replace(codeOut, r1, r2);
        }

        replaceAll(codeOut, "// EXISTING_CODE\n\n" + tabs + "// EXISTING_CODE",
                   "// EXISTING_CODE\n" + tabs + "// EXISTING_CODE");
        //--------------------------------------------------------------------------------------
    }

    // One final cleanup
    replaceAll(codeOut, "\n\n}", "\n}");
    replaceAll(codeOut, "\n\n\n", "\n\n");
    if (cw.stripEOFNL) {
        if (endsWith(codeOut, "\n"))
            replaceReverse(codeOut, "\n", "");
    }

    if (cw.nSpaces)
        replaceAll(codeOut, "\t", string_q(cw.nSpaces, ' '));
    replaceAll(codeOut, "[PTAB]", "\\t");

    if (contains(codeOut, "virtual") || contains(codeOut, "override")) {
        replace(codeOut, "~C", "virtual ~C");
        replace(codeOut, "~Q", "virtual ~Q");
    }

    bool testing = cw.testing;
    if (isTestMode())
        testing = true;

    if (cw.force || orig != codeOut) {
        // Do the actual writing of the data only if we're not testing or the user has told us not to
        if (cw.force || !testing) {
            LOG_INFO("Writing: ", cTeal, cw.fileName, cOff);
            stringToAsciiFile(cw.fileName, codeOut);
        } else {
            LOG8("Not writing: ", cw.fileName);
        }
        // We return 'true' if we WOULD HAVE have written the file (even if we didn't).
        return true;
    }

    // We return 'false' if we would NOT have written the file (not if we actually did).
    return false;
}

//-----------------------------------------------------------------------
// TODO(tjayrush): global data
static uint32_t sectionLocks = 0;
void lockSection(bool lock) {
    if (lock)
        sectionLocks++;
    else if (sectionLocks > 0)
        sectionLocks--;
    //        LOG_INFO(COptionsBase::g_progName, ": lockSection: ", sectionLocks);
}

//-----------------------------------------------------------------------
bool isSectionLocked(void) {
    return (sectionLocks > 0);
}

//-----------------------------------------------------------------------
size_t quitCount(size_t s) {
    // TODO(tjayrush): global data
    // This is global data, and therefore not thread safe, but it's okay since
    // we want to count a quit request no matter which thread it's from.
    static size_t g_QuitCount = 0;
    if (g_QuitCount && isSectionLocked())  // ignore if we're locked
        return false;
    g_QuitCount += s;
    return g_QuitCount;
}

//-----------------------------------------------------------------------
bool shouldQuit(void) {
    if (quitCount() == 0) {
        //            qblocks::eLogger->setEndline('\r');
        //            LOG_INFO("\t\t\t\t", COptionsBase::g_progName, ": shouldQuit: ", sectionLocks, ": ", quitCount());
        //            qblocks::eLogger->setEndline('\n');
        return false;
    }
    //        LOG_INFO(bGreen, COptionsBase::g_progName, ": shouldQuit - Quitting: ", quitCount(), cOff);
    cout << "\nFinishing work...\n";
    cleanFileLocks();
    cout.flush();
    return true;
}

//-----------------------------------------------------------------------
void cleanFileLocks(void) {
    // We want to clean the list entirely, once we start
    mutex aMutex;
    lock_guard<mutex> lock(aMutex);

    string_q list = manageRemoveList();
    CStringArray files;
    explode(files, list, '|');
    for (auto file : files) {
        remove(file.c_str());
        cerr << "Removing file: " << file << "\n";
        cerr.flush();
    }
    manageRemoveList("clear");
}

//-----------------------------------------------------------------------
void defaultQuitHandler(int signum) {
    if (quitCount(1) > 2) {
        // LOG_INFO(cBlue, COptionsBase::g_progName, ": defaultQuitHander (>2): ", quitCount(), ": ", signum, cOff);
        cleanFileLocks();
        if (signum != -1) {
            // LOG_INFO(cYellow, COptionsBase::g_progName, ": defaultQuitHander (>2): ", quitCount(), ": ", signum,
            // cOff);
            exit(EXIT_SUCCESS);
        }
    }
    // LOG_INFO(cTeal, COptionsBase::g_progName, ": defaultQuitHander (<2): ", quitCount(), ": ", signum, cOff);
}

//-----------------------------------------------------------------------
void quickQuitHandler(int signum) {
    cleanFileLocks();
    if (signum != -1)
        exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------
void registerQuitHandler(QUITHANDLER qh) {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = qh;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    sigaction(SIGTERM, &sigIntHandler, NULL);
}

//-----------------------------------------------------------------------
string_q manageRemoveList(const string_q& filename) {
    // We want to protect this so it doesn't get messed up. We don't
    // add the same string twice, so it's all good
    mutex aMutex;
    lock_guard<mutex> lock(aMutex);

    // TODO(tjayrush): global data
    static string_q theList;
    if (filename == "clear") {
        theList = "";
        return "";
    }

    string_q fn = substitute(filename, "r:", "");
    if (!fn.empty()) {
        if (startsWith(filename, "r:")) {
            replace(theList, fn + "|", "");

        } else if (!contains(theList, fn + "|")) {
            theList += (filename + "|");
        }
    }
    return theList;
}
}  // namespace qblocks
