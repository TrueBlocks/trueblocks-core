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
#include "abilib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBalHistory : public CBaseNode {
public:
    string_q recordID;
    timestamp_t timestamp;
    bigint_t balance;

public:
    CBalHistory(void);
    CBalHistory(const CBalHistory& ba);
    virtual ~CBalHistory(void);
    CBalHistory& operator=(const CBalHistory& ba);

    DECLARE_NODE(CBalHistory);

    // EXISTING_CODE
    CBalHistory(const string_q& _recID, bigint_t bal) : recordID(_recID), balance(bal) { }
    // EXISTING_CODE
    bool operator==(const CBalHistory& item) const;
    bool operator!=(const CBalHistory& item) const { return !operator==(item); }
    friend bool operator<(const CBalHistory& v1, const CBalHistory& v2);
    friend ostream& operator<<(ostream& os, const CBalHistory& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBalHistory& ba);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBalHistory::CBalHistory(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalHistory::CBalHistory(const CBalHistory& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBalHistory::~CBalHistory(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalHistory::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalHistory::initialize(void) {
    CBaseNode::initialize();

    recordID = "";
    timestamp = 0;
    balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalHistory::duplicate(const CBalHistory& ba) {
    clear();
    CBaseNode::duplicate(ba);

    recordID = ba.recordID;
    timestamp = ba.timestamp;
    balance = ba.balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBalHistory& CBalHistory::operator=(const CBalHistory& ba) {
    duplicate(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBalHistory::operator==(const CBalHistory& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBalHistory& v1, const CBalHistory& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBalHistory> CBalHistoryArray;
extern CArchive& operator>>(CArchive& archive, CBalHistoryArray& array);
extern CArchive& operator<<(CArchive& archive, const CBalHistoryArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CBalHistory& bal);
extern CArchive& operator>>(CArchive& archive, CBalHistory& bal);

//---------------------------------------------------------------------------
// EXISTING_CODE
inline string_q toRecordID(const string_q& addr, blknum_t bn, uint64_t tn) {
    return addr + "_" + padNum9(bn) + "_" + padNum5(tn);
}
// EXISTING_CODE
}  // namespace qblocks

