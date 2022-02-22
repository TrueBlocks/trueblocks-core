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
#include "basenode.h"
#include "exportcontext.h"
#include "sfarchive.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfigEnv : public CBaseNode {
  public:
    string_q chain;
    string_q configPath;
    string_q chainConfigPath;
    string_q cachePath;
    string_q indexPath;
    string_q defChain;
    string_q rpcProvider;

  public:
    CConfigEnv(void);
    CConfigEnv(const CConfigEnv& co);
    virtual ~CConfigEnv(void);
    CConfigEnv& operator=(const CConfigEnv& co);

    DECLARE_NODE(CConfigEnv);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CConfigEnv& it) const;
    bool operator!=(const CConfigEnv& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CConfigEnv& v1, const CConfigEnv& v2);
    friend ostream& operator<<(ostream& os, const CConfigEnv& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfigEnv& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfigEnv::CConfigEnv(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigEnv::CConfigEnv(const CConfigEnv& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfigEnv::~CConfigEnv(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigEnv::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigEnv::initialize(void) {
    CBaseNode::initialize();

    chain = "";
    configPath = "";
    chainConfigPath = "";
    cachePath = "";
    indexPath = "";
    defChain = "";
    rpcProvider = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigEnv::duplicate(const CConfigEnv& co) {
    clear();
    CBaseNode::duplicate(co);

    chain = co.chain;
    configPath = co.configPath;
    chainConfigPath = co.chainConfigPath;
    cachePath = co.cachePath;
    indexPath = co.indexPath;
    defChain = co.defChain;
    rpcProvider = co.rpcProvider;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigEnv& CConfigEnv::operator=(const CConfigEnv& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfigEnv::operator==(const CConfigEnv& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfigEnv& v1, const CConfigEnv& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfigEnv> CConfigEnvArray;
extern CArchive& operator>>(CArchive& archive, CConfigEnvArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfigEnvArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CConfigEnv& con);
extern CArchive& operator>>(CArchive& archive, CConfigEnv& con);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CONFIGENV;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
