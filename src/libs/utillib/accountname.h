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
    string_q name;
    string_q address;
    string_q symbol;
    string_q description;
    string_q source;
    string_q logo;
    string_q path;
    string_q color;
    bool is_contract;
    bool is_private;
    bool is_shared;
    blknum_t firstAppearance;
    blknum_t latestAppearance;
    blknum_t lastExport;
    uint64_t nRecords;
    uint64_t sizeInBytes;

public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    virtual ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    explicit CAccountName(const string_q& strIn);
    // EXISTING_CODE
    bool operator==(const CAccountName& item) const;
    bool operator!=(const CAccountName& item) const { return !operator==(item); }
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
    name = "";
    address = "";
    symbol = "";
    description = "";
    source = "";
    logo = "";
    path = "";
    color = "";
    is_contract = 0;
    is_private = 0;
    is_shared = 0;
    firstAppearance = 0;
    latestAppearance = 0;
    lastExport = 0;
    nRecords = 0;
    sizeInBytes = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::duplicate(const CAccountName& ac) {
    clear();
    CBaseNode::duplicate(ac);

    group = ac.group;
    name = ac.name;
    address = ac.address;
    symbol = ac.symbol;
    description = ac.description;
    source = ac.source;
    logo = ac.logo;
    path = ac.path;
    color = ac.color;
    is_contract = ac.is_contract;
    is_private = ac.is_private;
    is_shared = ac.is_shared;
    firstAppearance = ac.firstAppearance;
    latestAppearance = ac.latestAppearance;
    lastExport = ac.lastExport;
    nRecords = ac.nRecords;
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
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccountName& v1, const CAccountName& v2) {
    // EXISTING_CODE
    return v1.address < v2.address;
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAccountName> CAccountNameArray;
extern CArchive& operator>>(CArchive& archive, CAccountNameArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccountNameArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<address_t, CAccountName> CAccountNameMap;
// EXISTING_CODE
}  // namespace qblocks

