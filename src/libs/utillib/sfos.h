#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
    extern uint64_t fileSize(const string_q& file);

    extern int copyFile(const string_q& from, const string_q& to);
    extern int moveFile(const string_q& from, const string_q& to);
    extern int cleanFolder(const string_q& path, bool recurse=false, bool interactive=false);

    extern bool folderExists(const string_q& path);
    extern bool fileExists(const string_q& file);

    extern bool establishFolder(const string_q& path, string_q& created);
    inline bool establishFolder(const string_q& path) { string_q unused; return establishFolder(path, unused); }

    extern string_q getCWD(const string_q& filename = "");
    extern string_q doCommand(const string_q& cmd);

    extern string_q makeValidName(const string_q& inOut);

    extern bool isRunning(const string_q& progName, bool excludeSelf);
    extern size_t nRunning(const string_q& progName);
    extern string_q listRunning(const string_q& progName);

    #define kMaxPathSize _POSIX_PATH_MAX
}  // namespace qblocks
