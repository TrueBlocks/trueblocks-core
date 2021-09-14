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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceFilter : public CBaseNode {
  public:
    blknum_t fromBlock;
    blknum_t toBlock;
    CAddressArray fromAddress;
    CAddressArray toAddress;
    uint64_t after;
    uint64_t count;

  public:
    CTraceFilter(void);
    CTraceFilter(const CTraceFilter& tr);
    virtual ~CTraceFilter(void);
    CTraceFilter& operator=(const CTraceFilter& tr);

    DECLARE_NODE(CTraceFilter);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTraceFilter& it) const;
    bool operator!=(const CTraceFilter& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTraceFilter& v1, const CTraceFilter& v2);
    friend ostream& operator<<(ostream& os, const CTraceFilter& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTraceFilter& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceFilter::CTraceFilter(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceFilter::CTraceFilter(const CTraceFilter& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceFilter::~CTraceFilter(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceFilter::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceFilter::initialize(void) {
    CBaseNode::initialize();

    fromBlock = 0;
    toBlock = 0;
    fromAddress.clear();
    toAddress.clear();
    after = 0;
    count = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceFilter::duplicate(const CTraceFilter& tr) {
    clear();
    CBaseNode::duplicate(tr);

    fromBlock = tr.fromBlock;
    toBlock = tr.toBlock;
    fromAddress = tr.fromAddress;
    toAddress = tr.toAddress;
    after = tr.after;
    count = tr.count;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceFilter& CTraceFilter::operator=(const CTraceFilter& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTraceFilter::operator==(const CTraceFilter& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTraceFilter& v1, const CTraceFilter& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTraceFilter> CTraceFilterArray;
extern CArchive& operator>>(CArchive& archive, CTraceFilterArray& array);
extern CArchive& operator<<(CArchive& archive, const CTraceFilterArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTraceFilter& tra);
extern CArchive& operator>>(CArchive& archive, CTraceFilter& tra);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRACEFILTER;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
