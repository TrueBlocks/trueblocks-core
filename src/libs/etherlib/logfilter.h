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
#include "logentry.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogFilter : public CBaseNode {
  public:
    blknum_t fromBlock;
    blknum_t toBlock;
    hash_t blockHash;
    CAddressArray emitters;
    CTopicArray topics;

  public:
    CLogFilter(void);
    CLogFilter(const CLogFilter& lo);
    virtual ~CLogFilter(void);
    CLogFilter& operator=(const CLogFilter& lo);

    DECLARE_NODE(CLogFilter);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    string_q toRPC(void) const;
    bool wasEmittedBy(const address_t& test) const;
    bool passes(const CLogEntry& log);
    // EXISTING_CODE
    bool operator==(const CLogFilter& it) const;
    bool operator!=(const CLogFilter& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CLogFilter& v1, const CLogFilter& v2);
    friend ostream& operator<<(ostream& os, const CLogFilter& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CLogFilter& lo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogFilter::CLogFilter(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogFilter::CLogFilter(const CLogFilter& lo) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogFilter::~CLogFilter(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogFilter::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogFilter::initialize(void) {
    CBaseNode::initialize();

    fromBlock = 0;
    toBlock = 0;
    blockHash = "";
    emitters.clear();
    topics.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogFilter::duplicate(const CLogFilter& lo) {
    clear();
    CBaseNode::duplicate(lo);

    fromBlock = lo.fromBlock;
    toBlock = lo.toBlock;
    blockHash = lo.blockHash;
    emitters = lo.emitters;
    topics = lo.topics;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogFilter& CLogFilter::operator=(const CLogFilter& lo) {
    duplicate(lo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CLogFilter::operator==(const CLogFilter& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CLogFilter& v1, const CLogFilter& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CLogFilter> CLogFilterArray;
extern CArchive& operator>>(CArchive& archive, CLogFilterArray& array);
extern CArchive& operator<<(CArchive& archive, const CLogFilterArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CLogFilter& log);
extern CArchive& operator>>(CArchive& archive, CLogFilter& log);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_LOGFILTER;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
