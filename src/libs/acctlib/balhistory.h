#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "abilib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CBalHistory;
typedef SFArrayBase<CBalHistory>         CBalHistoryArray;
typedef SFList<CBalHistory*>             CBalHistoryList;
typedef SFUniqueList<CBalHistory*>       CBalHistoryListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBalHistory : public CBaseNode {
public:
    SFString recordID;
    timestamp_t timestamp;
    SFIntBN balance;

public:
    CBalHistory(void);
    CBalHistory(const CBalHistory& ba);
    virtual ~CBalHistory(void);
    CBalHistory& operator=(const CBalHistory& ba);

    DECLARE_NODE(CBalHistory);

    // EXISTING_CODE
    CBalHistory(const SFString& _recID, SFIntBN bal) : recordID(_recID), balance(bal) { }
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CBalHistory& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CBalHistory& ba);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBalHistory::CBalHistory(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBalHistory::CBalHistory(const CBalHistory& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBalHistory::~CBalHistory(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalHistory::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalHistory::Init(void) {
    CBaseNode::Init();

    recordID = "";
    timestamp = 0;
    balance = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBalHistory::Copy(const CBalHistory& ba) {
    Clear();
    CBaseNode::Copy(ba);

    recordID = ba.recordID;
    timestamp = ba.timestamp;
    balance = ba.balance;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBalHistory& CBalHistory::operator=(const CBalHistory& ba) {
    Copy(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CBalHistoryArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CBalHistoryArray);
IMPLEMENT_ARCHIVE_LIST(CBalHistoryList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CBalHistory& bal);
extern SFArchive& operator>>(SFArchive& archive, CBalHistory& bal);

//---------------------------------------------------------------------------
// EXISTING_CODE
inline SFString toRecordID(const SFString& addr, blknum_t bn, uint64_t tn) {
    return addr + "_" + padNum9(bn) + "_" + padNum5(tn);
}
// EXISTING_CODE
}  // namespace qblocks

