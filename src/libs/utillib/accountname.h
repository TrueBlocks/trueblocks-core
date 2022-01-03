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
#include "sfarchive.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountName : public CBaseNode {
  public:
    string_q tags;
    address_t address;
    string_q name;
    string_q symbol;
    string_q source;
    uint64_t decimals;
    string_q description;
    bool isCustom;
    bool isPrefund;
    bool isContract;
    bool isErc20;
    bool isErc721;

  public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    virtual ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAccountName& it) const;
    bool operator!=(const CAccountName& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAccountName& v1, const CAccountName& v2);
    friend ostream& operator<<(ostream& os, const CAccountName& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAccountName& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountName::CAccountName(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountName::CAccountName(const CAccountName& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountName::~CAccountName(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::initialize(void) {
    CBaseNode::initialize();

    tags = "";
    address = "";
    name = "";
    symbol = "";
    source = "";
    decimals = 0;
    description = "";
    isCustom = false;
    isPrefund = false;
    isContract = false;
    isErc20 = false;
    isErc721 = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::duplicate(const CAccountName& ac) {
    clear();
    CBaseNode::duplicate(ac);

    tags = ac.tags;
    address = ac.address;
    name = ac.name;
    symbol = ac.symbol;
    source = ac.source;
    decimals = ac.decimals;
    description = ac.description;
    isCustom = ac.isCustom;
    isPrefund = ac.isPrefund;
    isContract = ac.isContract;
    isErc20 = ac.isErc20;
    isErc721 = ac.isErc721;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountName& CAccountName::operator=(const CAccountName& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAccountName::operator==(const CAccountName& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return address % it.address;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccountName& v1, const CAccountName& v2) {
    // EXISTING_CODE
    if (v1.address == v2.address)
        return v1.tags < v2.tags;
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.address < v2.address;
}

//---------------------------------------------------------------------------
typedef vector<CAccountName> CAccountNameArray;
extern CArchive& operator>>(CArchive& archive, CAccountNameArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccountNameArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAccountName& acc);
extern CArchive& operator>>(CArchive& archive, CAccountName& acc);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ACCOUNTNAME;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<address_t, CAccountName> CAccountNameMap;
extern CArchive& operator>>(CArchive& archive, CAccountNameMap& nameMap);
extern CArchive& operator<<(CArchive& archive, const CAccountNameMap& nameMap);
// EXISTING_CODE
}  // namespace qblocks
