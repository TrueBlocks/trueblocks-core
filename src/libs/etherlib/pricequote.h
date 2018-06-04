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
class CPriceQuote;
typedef SFArrayBase<CPriceQuote>         CPriceQuoteArray;
typedef SFList<CPriceQuote*>             CPriceQuoteList;
typedef SFUniqueList<CPriceQuote*>       CPriceQuoteListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPriceQuote : public CBaseNode {
public:
    timestamp_t timestamp;
    double close;

public:
    CPriceQuote(void);
    CPriceQuote(const CPriceQuote& pr);
    virtual ~CPriceQuote(void);
    CPriceQuote& operator=(const CPriceQuote& pr);

    DECLARE_NODE(CPriceQuote);

    // EXISTING_CODE
    SFTime date;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CPriceQuote& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CPriceQuote& pr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPriceQuote::CPriceQuote(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPriceQuote::CPriceQuote(const CPriceQuote& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPriceQuote::~CPriceQuote(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Init(void) {
    CBaseNode::Init();

    timestamp = 0;
    close = 0.0;

    // EXISTING_CODE
    date = earliestDate;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Copy(const CPriceQuote& pr) {
    Clear();
    CBaseNode::Copy(pr);

    timestamp = pr.timestamp;
    close = pr.close;

    // EXISTING_CODE
    date = pr.date;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPriceQuote& CPriceQuote::operator=(const CPriceQuote& pr) {
    Copy(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CPriceQuoteArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CPriceQuoteArray);
IMPLEMENT_ARCHIVE_LIST(CPriceQuoteList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern uint64_t indexFromTimeStamp(const CPriceQuoteArray& quotes, timestamp_t ts);
extern SFString asDollars(timestamp_t ts, SFUintBN weiIn);
extern SFString dispDollars(timestamp_t ts, SFUintBN weiIn);
// EXISTING_CODE
}  // namespace qblocks

