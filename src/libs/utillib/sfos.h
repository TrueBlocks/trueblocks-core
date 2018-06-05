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

namespace qblocks {

    //-------------------------------------------------------------------------------
    class SFTime;

    //-------------------------------------------------------------------------------
    extern uint64_t fileSize(const SFString& file);

    extern void listFiles(uint32_t& nFiles, SFString *files, const SFString& mask);
    extern void listFolders(uint32_t& nDirs, SFString *dirs, const SFString& mask);
    extern void listFilesOrFolders(uint32_t& nItems, SFString *items, const SFString& mask);

    extern int copyFile(const SFString& from, const SFString& to);

    extern bool folderExists(const SFString& path);
    extern bool fileExists(const SFString& file);

    extern bool establishFolder(const SFString& path, SFString& created);
    inline bool establishFolder(const SFString& path) { SFString unused; return establishFolder(path, unused); }

    extern SFTime fileLastModifyDate(const SFString& filename);

    extern SFString getCWD(const SFString& filename = "");
    extern SFString doCommand(const SFString& cmd);

    #define kMaxPathSize _POSIX_PATH_MAX
}  // namespace qblocks
