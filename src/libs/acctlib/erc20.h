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
#include "acctlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTokenState_erc20 : public CAccountWatch {
public:
    address_t address;
    wei_t totalSupply;
    uint64_t decimals;
    string_q version;
    string_q symbol;
    CAddressArray holders;

public:
    CTokenState_erc20(void);
    CTokenState_erc20(const CTokenState_erc20& to);
    virtual ~CTokenState_erc20(void);
    CTokenState_erc20& operator=(const CTokenState_erc20& to);

    DECLARE_NODE(CTokenState_erc20);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTokenState_erc20& item) const;
    bool operator!=(const CTokenState_erc20& item) const { return !operator==(item); }
    friend bool operator<(const CTokenState_erc20& v1, const CTokenState_erc20& v2);
    friend ostream& operator<<(ostream& os, const CTokenState_erc20& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTokenState_erc20& to);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTokenState_erc20::CTokenState_erc20(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTokenState_erc20::CTokenState_erc20(const CTokenState_erc20& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTokenState_erc20::~CTokenState_erc20(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenState_erc20::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenState_erc20::initialize(void) {
    CAccountWatch::initialize();

    address = "";
    totalSupply = 0;
    decimals = 0;
    version = "";
    symbol = "";
    holders.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTokenState_erc20::duplicate(const CTokenState_erc20& to) {
    clear();
    CAccountWatch::duplicate(to);

    address = to.address;
    totalSupply = to.totalSupply;
    decimals = to.decimals;
    version = to.version;
    symbol = to.symbol;
    holders = to.holders;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTokenState_erc20& CTokenState_erc20::operator=(const CTokenState_erc20& to) {
    duplicate(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTokenState_erc20::operator==(const CTokenState_erc20& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTokenState_erc20& v1, const CTokenState_erc20& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTokenState_erc20> CTokenState_erc20Array;
extern CArchive& operator>>(CArchive& archive, CTokenState_erc20Array& array);
extern CArchive& operator<<(CArchive& archive, const CTokenState_erc20Array& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTokenState_erc20& tok);
extern CArchive& operator>>(CArchive& archive, CTokenState_erc20& tok);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TOKENSTATE_ERC20;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

