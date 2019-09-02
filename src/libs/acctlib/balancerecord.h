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
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBalanceRecord : public CBaseNode {
public:
    blknum_t blockNumber;
    blknum_t transactionIndex;
    address_t address;
    wei_t priorBalance;
    wei_t balance;

public:
    CBalanceRecord(void);
    CBalanceRecord(const CBalanceRecord& ba);
    virtual ~CBalanceRecord(void);
    CBalanceRecord& operator=(const CBalanceRecord& ba);

    DECLARE_NODE(CBalanceRecord);

    // EXISTING_CODE
    CBalanceRecord(string_q& line);
    // EXISTING_CODE
    bool operator==(const CBalanceRecord& item) const;
    bool operator!=(const CBalanceRecord& item) const { return !operator==(item); }
    friend bool operator<(const CBalanceRecord& v1, const CBalanceRecord& v2);
    friend ostream& operator<<(ostream& os, const CBalanceRecord& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBalanceRecord& ba);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBalanceRecord::CBalanceRecord(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalanceRecord::CBalanceRecord(const CBalanceRecord& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBalanceRecord::~CBalanceRecord(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceRecord::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceRecord::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    transactionIndex = 0;
    address = "";
    priorBalance = 0;
    balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalanceRecord::duplicate(const CBalanceRecord& ba) {
    clear();
    CBaseNode::duplicate(ba);

    blockNumber = ba.blockNumber;
    transactionIndex = ba.transactionIndex;
    address = ba.address;
    priorBalance = ba.priorBalance;
    balance = ba.balance;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalanceRecord& CBalanceRecord::operator=(const CBalanceRecord& ba) {
    duplicate(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBalanceRecord::operator==(const CBalanceRecord& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBalanceRecord& v1, const CBalanceRecord& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBalanceRecord> CBalanceRecordArray;
extern CArchive& operator>>(CArchive& archive, CBalanceRecordArray& array);
extern CArchive& operator<<(CArchive& archive, const CBalanceRecordArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CBalanceRecord& bal);
extern CArchive& operator>>(CArchive& archive, CBalanceRecord& bal);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_BALANCERECORD;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

