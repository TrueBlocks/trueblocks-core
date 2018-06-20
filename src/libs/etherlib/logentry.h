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
#include "abilib.h"

namespace qblocks {

// EXISTING_CODE
class CReceipt;
#define SFTopicArray SFBigUintArray
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogEntry : public CBaseNode {
public:
    SFAddress address;
    string_q data;
    uint64_t logIndex;
    SFTopicArray topics;

public:
    CLogEntry(void);
    CLogEntry(const CLogEntry& lo);
    virtual ~CLogEntry(void);
    CLogEntry& operator=(const CLogEntry& lo);

    DECLARE_NODE(CLogEntry);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    const CReceipt *pReceipt;
    bool operator==(const CLogEntry& le) const;
    bool operator!=(const CLogEntry& le) const { return !operator==(le); };
    // EXISTING_CODE
    friend bool operator<(const CLogEntry& v1, const CLogEntry& v2);
    friend ostream& operator<<(ostream& os, const CLogEntry& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogEntry& lo);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogEntry::CLogEntry(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogEntry::CLogEntry(const CLogEntry& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogEntry::~CLogEntry(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::initialize(void) {
    CBaseNode::initialize();

    address = "";
    data = "";
    logIndex = 0;
    topics.clear();

    // EXISTING_CODE
#if 0
    blockHash = "";
    blockNumber = 0;
    transactionHash = "";
    transactionIndex = 0;
#endif
    pReceipt = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::duplicate(const CLogEntry& lo) {
    clear();
    CBaseNode::duplicate(lo);

    address = lo.address;
    data = lo.data;
    logIndex = lo.logIndex;
    topics = lo.topics;

    // EXISTING_CODE
#if 0
    blockHash = lo.blockHash;
    blockNumber = lo.blockNumber;
    transactionHash = lo.transactionHash;
    transactionIndex = lo.transactionIndex;
#endif
    // no deep copy because it's const
    pReceipt = lo.pReceipt;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CLogEntry& CLogEntry::operator=(const CLogEntry& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogEntry& v1, const CLogEntry& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef SFArrayBase<CLogEntry> CLogEntryArray;
extern SFArchive& operator>>(SFArchive& archive, CLogEntryArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CLogEntryArray& array);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CLogEntry& log);
extern SFArchive& operator>>(SFArchive& archive, CLogEntry& log);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern string_q nextReceiptChunk(const string_q& fieldIn, const void *data);
extern string_q nextLogentryChunk(const string_q& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

