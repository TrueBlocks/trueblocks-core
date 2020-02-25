#pragma once
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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
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
    string_q group;
    address_t address;
    string_q name;
    string_q symbol;
    string_q source;
    string_q logo;
    string_q description;
    bool is_custom;
    bool is_prefund;
    blknum_t nAppearances;
    blknum_t lastExport;
    blknum_t firstAppearance;
    blknum_t latestAppearance;
    string_q path;
    uint64_t sizeInBytes;

  public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    virtual ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAccountName& item) const;
    bool operator!=(const CAccountName& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CAccountName& v1, const CAccountName& v2);
    friend ostream& operator<<(ostream& os, const CAccountName& item);

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

    group = "";
    address = "";
    name = "";
    symbol = "";
    source = "";
    logo = "";
    description = "";
    is_custom = false;
    is_prefund = false;
    nAppearances = 0;
    lastExport = 0;
    firstAppearance = 0;
    latestAppearance = 0;
    path = "";
    sizeInBytes = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::duplicate(const CAccountName& ac) {
    clear();
    CBaseNode::duplicate(ac);

    group = ac.group;
    address = ac.address;
    name = ac.name;
    symbol = ac.symbol;
    source = ac.source;
    logo = ac.logo;
    description = ac.description;
    is_custom = ac.is_custom;
    is_prefund = ac.is_prefund;
    nAppearances = ac.nAppearances;
    lastExport = ac.lastExport;
    firstAppearance = ac.firstAppearance;
    latestAppearance = ac.latestAppearance;
    path = ac.path;
    sizeInBytes = ac.sizeInBytes;

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
inline bool CAccountName::operator==(const CAccountName& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return address % item.address;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccountName& v1, const CAccountName& v2) {
    // EXISTING_CODE
    return v1.address < v2.address;
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.address < v2.address;
}

//---------------------------------------------------------------------------
typedef vector<CAccountName> CAccountNameArray;
extern CArchive& operator>>(CArchive& archive, CAccountNameArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccountNameArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ACCOUNTNAME;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<address_t, CAccountName> CAccountNameMap;
// EXISTING_CODE
}  // namespace qblocks
