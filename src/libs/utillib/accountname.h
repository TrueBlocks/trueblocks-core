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
    string_q addr;
    string_q symbol;
    string_q name;
    string_q source;
    string_q description;
    string_q logo;
    bool isContract;
    bool custom;
    string_q shared;
    string_q fn;
    double size;
    blknum_t lb;
    blknum_t le;
    uint64_t nrecs;

public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    virtual ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    explicit CAccountName(const string_q& nameIn);
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
    addr = "";
    symbol = "";
    name = "";
    source = "";
    description = "";
    logo = "";
    isContract = 0;
    custom = 0;
    shared = "";
    fn = "";
    size = 0.0;
    lb = 0;
    le = 0;
    nrecs = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::duplicate(const CAccountName& ac) {
    clear();
    CBaseNode::duplicate(ac);

    group = ac.group;
    addr = ac.addr;
    symbol = ac.symbol;
    name = ac.name;
    source = ac.source;
    description = ac.description;
    logo = ac.logo;
    isContract = ac.isContract;
    custom = ac.custom;
    shared = ac.shared;
    fn = ac.fn;
    size = ac.size;
    lb = ac.lb;
    le = ac.le;
    nrecs = ac.nrecs;

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
    return v1.addr < v2.addr;
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

