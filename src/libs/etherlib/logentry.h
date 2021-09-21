#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"
#include "abi.h"

namespace qblocks {

// EXISTING_CODE
class CReceipt;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogEntry : public CBaseNode {
  public:
    address_t address;
    blknum_t logIndex;
    CTopicArray topics;
    string_q data;
    CFunction articulatedLog;
    string_q compressedLog;
    blknum_t transactionLogIndex;
    string_q type;
    bool removed;

  public:
    CLogEntry(void);
    CLogEntry(const CLogEntry& lo);
    virtual ~CLogEntry(void);
    CLogEntry& operator=(const CLogEntry& lo);

    DECLARE_NODE(CLogEntry);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    const CReceipt* pReceipt;
    // EXISTING_CODE
    bool operator==(const CLogEntry& it) const;
    bool operator!=(const CLogEntry& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CLogEntry& v1, const CLogEntry& v2);
    friend ostream& operator<<(ostream& os, const CLogEntry& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogEntry& lo);
    bool readBackLevel(CArchive& archive) override;

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
    logIndex = 0;
    topics.clear();
    data = "";
    articulatedLog = CFunction();
    compressedLog = "";
    transactionLogIndex = 0;
    type = "";
    removed = false;

    // EXISTING_CODE
    pReceipt = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::duplicate(const CLogEntry& lo) {
    clear();
    CBaseNode::duplicate(lo);

    address = lo.address;
    logIndex = lo.logIndex;
    topics = lo.topics;
    data = lo.data;
    articulatedLog = lo.articulatedLog;
    compressedLog = lo.compressedLog;
    transactionLogIndex = lo.transactionLogIndex;
    type = lo.type;
    removed = lo.removed;

    // EXISTING_CODE
    // no deep copy because it's const
    pReceipt = lo.pReceipt;
    finishParse();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogEntry& CLogEntry::operator=(const CLogEntry& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLogEntry::operator==(const CLogEntry& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogEntry& v1, const CLogEntry& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLogEntry> CLogEntryArray;
extern CArchive& operator>>(CArchive& archive, CLogEntryArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogEntryArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CLogEntry& log);
extern CArchive& operator>>(CArchive& archive, CLogEntry& log);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_LOGENTRY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
