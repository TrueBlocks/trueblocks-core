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
class CLog : public CBaseNode {
  public:
    address_t address;
    hash_t blockHash;
    blknum_t blockNumber;
    uint64_t logIndex;
    CTopicArray topics;
    string_q data;
    CFunction articulatedLog;
    string_q compressedLog;
    hash_t transactionHash;
    uint64_t transactionIndex;
    timestamp_t timestamp;
    string_q type;
    bool unused;

  public:
    CLog(void);
    CLog(const CLog& lo);
    virtual ~CLog(void);
    CLog& operator=(const CLog& lo);

    DECLARE_NODE(CLog);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    const CReceipt* pReceipt;
    // EXISTING_CODE
    bool operator==(const CLog& it) const;
    bool operator!=(const CLog& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CLog& v1, const CLog& v2);
    friend ostream& operator<<(ostream& os, const CLog& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLog& lo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLog::CLog(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLog::CLog(const CLog& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLog::~CLog(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLog::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLog::initialize(void) {
    CBaseNode::initialize();

    address = "";
    blockHash = "";
    blockNumber = 0;
    logIndex = 0;
    topics.clear();
    data = "";
    articulatedLog = CFunction();
    compressedLog = "";
    transactionHash = "";
    transactionIndex = 0;
    timestamp = 0;
    type = "";
    unused = false;

    // EXISTING_CODE
    pReceipt = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLog::duplicate(const CLog& lo) {
    clear();
    CBaseNode::duplicate(lo);

    address = lo.address;
    blockHash = lo.blockHash;
    blockNumber = lo.blockNumber;
    logIndex = lo.logIndex;
    topics = lo.topics;
    data = lo.data;
    articulatedLog = lo.articulatedLog;
    compressedLog = lo.compressedLog;
    transactionHash = lo.transactionHash;
    transactionIndex = lo.transactionIndex;
    timestamp = lo.timestamp;
    type = lo.type;
    unused = lo.unused;

    // EXISTING_CODE
    // no deep copy because it's const
    pReceipt = lo.pReceipt;
    finishParse();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLog& CLog::operator=(const CLog& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLog::operator==(const CLog& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLog& v1, const CLog& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLog> CLogArray;
extern CArchive& operator>>(CArchive& archive, CLogArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CLog& log);
extern CArchive& operator>>(CArchive& archive, CLog& log);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_LOG;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
