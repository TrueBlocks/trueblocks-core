#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBalanceHistory : public CBaseNode {
public:
    blknum_t bn;
    SFUintBN balance;

public:
    CBalanceHistory(void);
    CBalanceHistory(const CBalanceHistory& ba);
    virtual ~CBalanceHistory(void);
    CBalanceHistory& operator=(const CBalanceHistory& ba);

    DECLARE_NODE(CBalanceHistory);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBalanceHistory& item) const;
    bool operator!=(const CBalanceHistory& item) const { return !operator==(item); }
    friend bool operator<(const CBalanceHistory& v1, const CBalanceHistory& v2);
    friend ostream& operator<<(ostream& os, const CBalanceHistory& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBalanceHistory& ba);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBalanceHistory::CBalanceHistory(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalanceHistory::CBalanceHistory(const CBalanceHistory& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBalanceHistory::~CBalanceHistory(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceHistory::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceHistory::initialize(void) {
    CBaseNode::initialize();

    bn = 0;
    balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceHistory::duplicate(const CBalanceHistory& ba) {
    clear();
    CBaseNode::duplicate(ba);

    bn = ba.bn;
    balance = ba.balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBalanceHistory& CBalanceHistory::operator=(const CBalanceHistory& ba) {
    duplicate(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBalanceHistory::operator==(const CBalanceHistory& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default equality operator as defined in class definition
    return bn == item.bn;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBalanceHistory& v1, const CBalanceHistory& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.bn < v2.bn;
}

//---------------------------------------------------------------------------
typedef vector<CBalanceHistory> CBalanceHistoryArray;
extern CArchive& operator>>(CArchive& archive, CBalanceHistoryArray& array);
extern CArchive& operator<<(CArchive& archive, const CBalanceHistoryArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

