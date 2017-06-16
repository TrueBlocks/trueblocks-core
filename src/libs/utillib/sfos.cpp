/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <glob.h>
#include <libgen.h>
#include <algorithm>

#include "basetypes.h"

#include "sfos.h"
#include "sftime.h"
#include "string.h"

namespace qblocks {

    #define ANY_FILETYPE -1
    #define remove unlink

    //------------------------------------------------------------------
    static SFString escapePath(const SFString& nameIn) {
        SFString name = nameIn;
        name.ReplaceAll(" ", "\\ ");
        name.ReplaceAll("&", "\\&");
        name.ReplaceAll("(", "\\(");
        name.ReplaceAll(")", "\\)");
        name.ReplaceAll("'", "\\'");
        return name;
    }

    //------------------------------------------------------------------
    int copyFile(const SFString& fromIn, const SFString& toIn) {
        SFString from = escapePath(fromIn);
        SFString to   = escapePath(toIn);

        const SFString copyCmd = "cp -pf";

        SFString command = copyCmd + " " + from + " " + to;
        if (system((const char *)command)) { }  // do not remove. The test just silences compiler warnings
        return static_cast<int>(fileExists(to));
    }

    //------------------------------------------------------------------
    static int globErrFunc(const char *epath, int eerrno) {
        //  perror(epath);
        return 0;
    }

    //------------------------------------------------------------------
    // Returns a list of either files or folders, but not both.
    //------------------------------------------------------------------
    void doGlob(uint32_t& nStrs, SFString *strs, const SFString& maskIn, int wantFiles, bool keepPaths ) {
        ASSERT(!strs || nStrs);

        glob_t globBuf;

        SFString mask = maskIn;

        // should check return code
        glob((const char *)mask, GLOB_MARK, globErrFunc, &globBuf);

        size_t n = globBuf.gl_pathc;
        uint32_t mx = nStrs;
        nStrs = 0;
        char c;

        for (int i = 0 ; i < n ; i++) {
            // get path
            char *tmp = globBuf.gl_pathv[i];
            // last char
            c   = tmp[strlen(tmp) - 1];

            // if path ends in '/' then this is directory, filter accordingly

            bool isDir = ('/' == c);
            bool listEm = ((isDir) ? !wantFiles : wantFiles);
            if (wantFiles == ANY_FILETYPE)
                listEm = true;

            if (listEm) {
                if (NULL != strs) {
                    SFString path = globBuf.gl_pathv[i];

                    // filter specified directories and remove trailing '/'
                    if (path.endsWith('/'))
                        path = path.Left(path.length() - 1);

                    if (!keepPaths) {
                        // trim path to last directory / file
                        char res[kMaxPathSize];
                        path = basename_r(path.c_str(), res);  // NOLINT
                        if (path.startsWith('/'))
                            path = path.substr(1);
                        // The path we return is always just the name of the folder or file
                        // without any leading (or even existing) '/'
                        ASSERT(path.length() && path[0] != '/');
                    }

                    if (wantFiles == ANY_FILETYPE) {
                        if (isDir)
                            path = "d-" + path;
                        else
                            path = "f-" + path;
                    }

                    strs[nStrs] = path;
                }

                nStrs++;
                if (NULL != strs && nStrs >= mx) {
                    break;
                }
            }
        }

        globfree(&globBuf);
    }

    //-------------------------------------------------------------------------------------------------------------
    inline bool waitForCreate(const SFString& filename) {
        uint32_t mx = 1000;
        uint32_t cnt = 0;
        while (cnt < mx && !fileExists(filename))
            cnt++;

        return fileExists(filename);
    }

    //------------------------------------------------------------------------------------------
    SFString doCommand(const SFString& cmd) {

        SFTime now = Now();
        SFString tmpPath = "/tmp/";
        SFString filename = makeValidName(tmpPath + "qb_" + now.Format("%Y%m%d%H%M%S"));
        if (system((const char *)(cmd + ">" + filename))) { }  // Don't remove. Silences compiler warnings

        // Check twice for existance since the previous command creates the file but may take some time
        waitForCreate(filename);
extern SFString binaryFileToString(const SFString& filename);
        SFString ret = binaryFileToString(filename);
        removeFile(filename);
        return Strip(ret, '\n');
    }

    //------------------------------------------------------------------
    SFString getCWD(const SFString& filename) {
        char buffer[kMaxPathSize];
        if (::getcwd(buffer, kMaxPathSize)) { }  // do not remove. The test just silences compiler warnings
        SFString folder = buffer;
        if (!folder.endsWith('/'))
            folder += "/";
        return folder + filename;  // may be empty
    }

    //------------------------------------------------------------------
    int removeFile(const SFString& name) {
        return ::remove((const char *)name);
    }

    //------------------------------------------------------------------
    int moveFile(const SFString& from, const SFString& to) {
        if (from % to)
            return true;

        if (copyFile(from, to))
            return !removeFile(from);  // remove file returns '0' on success
        return false;
    }

    //------------------------------------------------------------------
    // Send a float representing seconds - adjust because Windows takes 1/1000 (thousandth)
    // of a second and Linux takes 1/1000000 (millionth)
    void qbSleep(float units) {
        ::usleep(units * 1000000.);
    }

    //------------------------------------------------------------------
    bool fileExists(const SFString& file) {
        struct stat statBuf;
        return !file.empty() && stat((const char *)file, &statBuf) == 0;
    }

    //------------------------------------------------------------------
    bool folderExists(const SFString& folderName) {
        if (folderName.empty())
            return false;

        SFString folder = folderName;
        if (!folder.endsWith('/'))
            folder += '/';

        uint32_t nFiles = 0;
        listFiles(nFiles, NULL, folder+"*.*");
        // check to see if it is just folders
        if (!nFiles)
            listFolders(nFiles, NULL, folder+"*.*");
        if (!nFiles)
            listFolders(nFiles, NULL, folder+".");

        return (nFiles > 0);
    }

    //--------------------------------------------------------------------------------
    int removeFolder(const SFString& folderIn) {
        SFString folder = folderIn;
        if (!folder.endsWith('/'))
            folder += "/";

        uint32_t nFiles = 0;
        listFiles(nFiles, NULL, folder + "*");
        if (nFiles) {
            SFString *files = new SFString[nFiles];
            listFiles(nFiles, files, folder + "*");
            if (files) {
                for (uint32_t j = 0 ; j < nFiles ; j++)
                    if (files[j] != "." && files[j] != "..")
                        removeFile(folder + files[j]);
                delete [] files;
            }
        }
        rmdir(folder.c_str());

        return !fileExists(folder.c_str());
    }

    //------------------------------------------------------------------
    SFTime fileCreateDate(const SFString& filename) {
        if (!fileExists(filename))
            return earliestDate;

        struct stat statBuf;
        stat(filename, &statBuf);

        // Convert time_t to struct tm
        tm unused;
        tm *t = localtime_r(&statBuf.st_ctime, &unused);
        ASSERT(t);
        return SFTime(t->tm_year + 1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    }

    //------------------------------------------------------------------
    SFTime fileLastModifyDate(const SFString& filename) {
        if (!fileExists(filename))
            return earliestDate;

        struct stat statBuf;
        stat(filename, &statBuf);

        // Convert time_t to struct tm
        tm unused;
        tm *t = localtime_r(&statBuf.st_mtime, &unused);
        ASSERT(t);
        return SFTime(t->tm_year + 1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    }

    //------------------------------------------------------------------
    uint32_t nFiles(const SFString& path) {
        uint32_t count = 0;
        listFiles(count, NULL, path);
        return max(0U, count);
    }

    //------------------------------------------------------------------
    void listFiles(uint32_t& nStrs, SFString *strs, const SFString& mask) {
        uint32_t ret = 0;
        doGlob(ret, strs, mask, true, mask.Contains("/*/")); /* fixes color coding in pico */
        nStrs = ret;
    }

    //------------------------------------------------------------------
    uint32_t nFolders(const SFString& path) {
        uint32_t count = 0;
        listFolders(count, NULL, path);
        return count;  // don't count '.' or '..'
    }

    //------------------------------------------------------------------
    void listFolders(uint32_t& nStrs, SFString *strs, const SFString& mask) {
        uint32_t ret = 0;
        doGlob(ret, strs, mask, false, mask.Contains("/*/")); /* fixes color coding in pico */
        nStrs = ret;
    }

    //------------------------------------------------------------------
    void listFilesOrFolders(uint32_t& nStrs, SFString *strs, const SFString& mask) {
        uint32_t ret = 0;
        doGlob(ret, strs, mask, ANY_FILETYPE, mask.Contains("/*/"));
        nStrs = ret;
    }

    //------------------------------------------------------------------
    SFUint32 fileSize(const SFString& filename) {
        if (!fileExists(filename))
            return 0;

        struct stat statBuf;
        stat((const char *)filename, &statBuf);
        return statBuf.st_size;
    }

    //----------------------------------------------------------------------------
    bool establishFolder(const SFString& path, SFString& created) {
        if (fileExists(path) || folderExists(path))
            return true;

        SFString targetFolder = path;

        size_t find = targetFolder.ReverseFind('/');
        targetFolder = targetFolder.Left(find) + "/";
        SFString folder = targetFolder;
        SFString curFolder = "/";
        while (!folder.empty()) {
            curFolder += nextTokenClear(folder, '/') + "/";
            if (!folderExists(curFolder)) {
                mkdir(curFolder.c_str(), (mode_t)0755);
                if (created.empty())
                    created = curFolder;
            }
        }
        return folderExists(targetFolder);
    }
}  // namespace qblocks
