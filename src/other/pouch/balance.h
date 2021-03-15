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
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBalance : public CBaseNode {
  public:
    string_q asset;
    string_q balance;

  public:
    CBalance(void);
    CBalance(const CBalance& ba);
    virtual ~CBalance(void);
    CBalance& operator=(const CBalance& ba);

    DECLARE_NODE(CBalance);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBalance& it) const;
    bool operator!=(const CBalance& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CBalance& v1, const CBalance& v2);
    friend ostream& operator<<(ostream& os, const CBalance& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBalance& ba);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBalance::CBalance(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalance::CBalance(const CBalance& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBalance::~CBalance(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalance::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalance::initialize(void) {
    CBaseNode::initialize();

    asset = "";
    balance = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalance::duplicate(const CBalance& ba) {
    clear();
    CBaseNode::duplicate(ba);

    asset = ba.asset;
    balance = ba.balance;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalance& CBalance::operator=(const CBalance& ba) {
    duplicate(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBalance::operator==(const CBalance& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBalance& v1, const CBalance& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBalance> CBalanceArray;
extern CArchive& operator>>(CArchive& archive, CBalanceArray& array);
extern CArchive& operator<<(CArchive& archive, const CBalanceArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_BALANCE;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<string, CBalance> CBalanceMap;
// EXISTING_CODE
}  // namespace qblocks
