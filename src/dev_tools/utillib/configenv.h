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
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "basetypes.h"
#include "sfarchive.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfigEnv {
  public:
    string_q chain;
    string_q configPath;
    string_q chainConfigPath;
    string_q cachePath;

  public:
    CConfigEnv(void);
    CConfigEnv(const CConfigEnv& co);
    virtual ~CConfigEnv(void);
    CConfigEnv& operator=(const CConfigEnv& co);

  private:
    bool operator==(const CConfigEnv& it) const;
    bool operator!=(const CConfigEnv& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CConfigEnv& v1, const CConfigEnv& v2);
    friend ostream& operator<<(ostream& os, const CConfigEnv& it);
};

inline CConfigEnv::CConfigEnv(void) {
    chain = "";
    configPath = "";
    chainConfigPath = "";
    cachePath = "";
}

inline CConfigEnv::CConfigEnv(const CConfigEnv& co) {
    chain = co.chain;
    configPath = co.configPath;
    chainConfigPath = co.chainConfigPath;
    cachePath = co.cachePath;
}

inline CConfigEnv::~CConfigEnv(void) {
}

inline CConfigEnv& CConfigEnv::operator=(const CConfigEnv& co) {
    chain = co.chain;
    configPath = co.configPath;
    chainConfigPath = co.chainConfigPath;
    cachePath = co.cachePath;
    return *this;
}

inline bool CConfigEnv::operator==(const CConfigEnv& it) const {
    return false;
}

inline bool operator<(const CConfigEnv& v1, const CConfigEnv& v2) {
    return true;
}

extern const CConfigEnv* getConfigEnv(void);
extern void loadEnvironmentPaths(void);
extern string_q getPathToCommands(const string_q& _part);

}  // namespace qblocks
