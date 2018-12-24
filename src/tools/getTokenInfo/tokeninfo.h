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
#include <vector>
#include <map>
#include "acctlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTokenInfo : public CAccountWatch {
public:
    address_t addr;
    wei_t totalSupply;
    uint64_t decimals;
    string_q version;
    string_q symbol;
    CAddressArray holders;

public:
    CTokenInfo(void);
    CTokenInfo(const CTokenInfo& to);
    virtual ~CTokenInfo(void);
    CTokenInfo& operator=(const CTokenInfo& to);

    DECLARE_NODE(CTokenInfo);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTokenInfo& item) const;
    bool operator!=(const CTokenInfo& item) const { return !operator==(item); }
    friend bool operator<(const CTokenInfo& v1, const CTokenInfo& v2);
    friend ostream& operator<<(ostream& os, const CTokenInfo& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTokenInfo& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTokenInfo::CTokenInfo(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTokenInfo::CTokenInfo(const CTokenInfo& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTokenInfo::~CTokenInfo(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenInfo::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenInfo::initialize(void) {
    CAccountWatch::initialize();

    addr = "";
    totalSupply = 0;
    decimals = 0;
    version = "";
    symbol = "";
    holders.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenInfo::duplicate(const CTokenInfo& to) {
    clear();
    CAccountWatch::duplicate(to);

    addr = to.addr;
    totalSupply = to.totalSupply;
    decimals = to.decimals;
    version = to.version;
    symbol = to.symbol;
    holders = to.holders;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTokenInfo& CTokenInfo::operator=(const CTokenInfo& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTokenInfo::operator==(const CTokenInfo& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTokenInfo& v1, const CTokenInfo& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTokenInfo> CTokenInfoArray;
extern CArchive& operator>>(CArchive& archive, CTokenInfoArray& array);
extern CArchive& operator<<(CArchive& archive, const CTokenInfoArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTokenInfo& tok);
extern CArchive& operator>>(CArchive& archive, CTokenInfo& tok);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

