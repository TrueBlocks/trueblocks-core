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
class CName : public CBaseNode {
  public:
    string_q tags;
    address_t address;
    string_q name;
    string_q symbol;
    string_q source;
    uint64_t decimals;
    string_q petname;
    bool isCustom;
    bool isPrefund;
    bool isContract;
    bool isErc20;
    bool isErc721;

  public:
    CName(void);
    CName(const CName& na);
    virtual ~CName(void);
    CName& operator=(const CName& na);

    DECLARE_NODE(CName);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CName& it) const;
    bool operator!=(const CName& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CName& v1, const CName& v2);
    friend ostream& operator<<(ostream& os, const CName& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CName& na);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CName::CName(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CName::CName(const CName& na) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(na);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CName::~CName(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CName::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CName::initialize(void) {
    CBaseNode::initialize();

    tags = "";
    address = "";
    name = "";
    symbol = "";
    source = "";
    decimals = 0;
    petname = "";
    isCustom = false;
    isPrefund = false;
    isContract = false;
    isErc20 = false;
    isErc721 = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CName::duplicate(const CName& na) {
    clear();
    CBaseNode::duplicate(na);

    tags = na.tags;
    address = na.address;
    name = na.name;
    symbol = na.symbol;
    source = na.source;
    decimals = na.decimals;
    petname = na.petname;
    isCustom = na.isCustom;
    isPrefund = na.isPrefund;
    isContract = na.isContract;
    isErc20 = na.isErc20;
    isErc721 = na.isErc721;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CName& CName::operator=(const CName& na) {
    duplicate(na);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CName::operator==(const CName& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return address % it.address;
}

//-------------------------------------------------------------------------
inline bool operator<(const CName& v1, const CName& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.address < v2.address;
}

//---------------------------------------------------------------------------
typedef vector<CName> CNameArray;
extern CArchive& operator>>(CArchive& archive, CNameArray& array);
extern CArchive& operator<<(CArchive& archive, const CNameArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CName& nam);
extern CArchive& operator>>(CArchive& archive, CName& nam);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_NAME;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
