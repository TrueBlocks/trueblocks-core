#include "basetypes.h"

#include "database.h"
#include "sfos.h"
#include "filenames.h"

namespace qblocks {

#define maxSecondsLock 3

#define LK_NO_CREATE_LOCK_FILE 1
#define LK_FILE_NOT_EXIST 2
#define LK_BAD_OPEN_MODE 3
#define LK_NO_REMOVE_LOCK 4

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

bool CSharedResource::createLock(bool createOnFail) {
    string_q lockFilename = m_filename + ".lck";

    int i = 0;
    while (i < maxSecondsLock) {
        if (!fileExists(lockFilename))
            return createLockFile(lockFilename);
        usleep(1000000);
        i++;
    }

    if (createOnFail) {
        m_ownsLock = true;
        Release();
        return createLockFile(lockFilename);
    }

    return false;
}

bool CSharedResource::waitOnLock(bool deleteOnFail) const {
    string_q lockFilename = m_filename + ".lck";

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
        return true;
    }

    return false;
}

bool CSharedResource::Lock(const string_q& fn, const string_q& mode, size_t lockType) {
    m_filename = fn;
    m_mode = mode;
    m_fp = NULL;

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
    return m_fp != NULL;
}

void CSharedResource::Release(void) {
    if (m_fp) {
        fflush(m_fp);
        fclose(m_fp);
    }
    m_fp = NULL;
    if (m_ownsLock) {
        string_q lockFilename = m_filename + ".lck";
        bool ret = remove(lockFilename.c_str());
        manageRemoveList("r:" + lockFilename);
        if (ret != 0) {
            m_error = LK_NO_REMOVE_LOCK;
            m_errorMsg = "Could not remove lock";
        }
    }

    m_ownsLock = false;
}

string_q manageRemoveList(const string_q& filename) {
    mutex aMutex;
    lock_guard<mutex> lock(aMutex);

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

size_t asciiFileToBuffer(const string_q& fileName, vector<char>& buffer) {
    size_t len = fileSize(fileName);
    buffer.resize(len);
    CSharedResource archive;
    if (archive.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        fread(buffer.data(), 1, len, archive.m_fp);
        if (archive.m_fp) {
            fflush(archive.m_fp);
            fclose(archive.m_fp);
        }
        archive.m_fp = NULL;
    }
    return buffer.size();
}

string_q asciiFileToString(const string_q& filename) {
    vector<char> buffer;
    asciiFileToBuffer(filename, buffer);
    buffer.push_back('\0');  // not sure if this is needed or not. At worse it's redundant
    return buffer.data();
}

size_t asciiFileToLines(const string_q& fileName, CStringArray& lines) {
    string_q contents = asciiFileToString(fileName);
    explode(lines, contents, '\n');
    return lines.size();
}

size_t appendToAsciiFile(const string_q& fileName, const string_q& contents) {
    CSharedResource lock;
    if (lock.Lock(fileName, modeWriteAppend, LOCK_WAIT)) {
        fprintf(lock.m_fp, "%s", contents.c_str());
        if (lock.m_fp) {
            fflush(lock.m_fp);
            fclose(lock.m_fp);
        }
        lock.m_fp = NULL;
        return fileSize(fileName);
    } else {
        return false;
    }
}

size_t stringToAsciiFile(const string_q& fileName, const string_q& contents) {
    CSharedResource lock;
    if (lock.Lock(fileName, modeWriteCreate, LOCK_WAIT)) {
        fprintf(lock.m_fp, "%s", contents.c_str());
        if (lock.m_fp) {
            fflush(lock.m_fp);
            fclose(lock.m_fp);
        }
        lock.m_fp = NULL;
        return fileSize(fileName);
    } else {
        return false;
    }
}

}  // namespace qblocks
