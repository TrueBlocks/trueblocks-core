#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

namespace qblocks {

    //-------------------------------------------------------------------------------
    class SFTime;

    //-------------------------------------------------------------------------------
    extern uint64_t fileSize(const SFString& file);

    extern void listFiles(uint32_t& nFiles, SFString *files, const SFString& mask);
    extern void listFolders(uint32_t& nDirs, SFString *dirs, const SFString& mask);
    extern void listFilesOrFolders(uint32_t& nItems, SFString *items, const SFString& mask);

    extern int copyFile(const SFString& from, const SFString& to);
    extern int moveFile(const SFString& from, const SFString& to);
    extern int removeFile(const SFString& file);
    extern int removeFolder(const SFString& folder);

    extern bool folderExists(const SFString& path);
    extern bool fileExists(const SFString& file);

    extern bool establishFolder(const SFString& path, SFString& created);
    inline bool establishFolder(const SFString& path) { SFString unused; return establishFolder(path, unused); }

    extern SFTime fileCreateDate(const SFString& filename);
    extern SFTime fileLastModifyDate(const SFString& filename);

    extern void qbSleep(float units);
    extern SFString getCWD(const SFString& filename = "");
    extern SFString doCommand(const SFString& cmd);

    #define kMaxPathSize _POSIX_PATH_MAX
}  // namespace qblocks
