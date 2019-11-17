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
class CBalanceDelta : public CBaseNode {
  public:
    blknum_t blockNumber;
    address_t address;
    wei_t balance;
    bigint_t diff;

  public:
    CBalanceDelta(void);
    CBalanceDelta(const CBalanceDelta& ba);
    virtual ~CBalanceDelta(void);
    CBalanceDelta& operator=(const CBalanceDelta& ba);

    DECLARE_NODE(CBalanceDelta);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBalanceDelta& item) const;
    bool operator!=(const CBalanceDelta& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CBalanceDelta& v1, const CBalanceDelta& v2);
    friend ostream& operator<<(ostream& os, const CBalanceDelta& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBalanceDelta& ba);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBalanceDelta::CBalanceDelta(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalanceDelta::CBalanceDelta(const CBalanceDelta& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBalanceDelta::~CBalanceDelta(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceDelta::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceDelta::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    address = "";
    balance = 0;
    diff = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceDelta::duplicate(const CBalanceDelta& ba) {
    clear();
    CBaseNode::duplicate(ba);

    blockNumber = ba.blockNumber;
    address = ba.address;
    balance = ba.balance;
    diff = ba.diff;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalanceDelta& CBalanceDelta::operator=(const CBalanceDelta& ba) {
    duplicate(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBalanceDelta::operator==(const CBalanceDelta& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBalanceDelta& v1, const CBalanceDelta& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBalanceDelta> CBalanceDeltaArray;
extern CArchive& operator>>(CArchive& archive, CBalanceDeltaArray& array);
extern CArchive& operator<<(CArchive& archive, const CBalanceDeltaArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CBalanceDelta& bal);
extern CArchive& operator>>(CArchive& archive, CBalanceDelta& bal);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_BALANCEDELTA;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
