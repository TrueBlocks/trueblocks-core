#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "ethtypes.h"
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
    SFUint32 timestamp;
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
extern bool loadPriceData(CPriceQuoteArray& quotes, bool freshen, SFString& message, SFUint32 step = 1);
extern uint64_t indexFromTimeStamp(const CPriceQuoteArray& quotes, timestamp_t ts);
extern SFString asDollars(timestamp_t ts, SFUintBN weiIn);
extern SFString dispDollars(timestamp_t ts, SFUintBN weiIn);
extern SFString priceDatabasePath(void);
// EXISTING_CODE
}  // namespace qblocks

