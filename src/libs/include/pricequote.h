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
    double open;
    double high;
    double low;
    double close;
    double quoteVolume;
    double volume;
    double weightedAvg;

public:
    CPriceQuote(void);
    CPriceQuote(const CPriceQuote& pr);
    ~CPriceQuote(void);
    CPriceQuote& operator=(const CPriceQuote& pr);

    DECLARE_NODE(CPriceQuote);

    // EXISTING_CODE
    SFTime date;
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CPriceQuote& pr);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPriceQuote::CPriceQuote(void) {
    Init();
    // EXISTING_CODE
    extern bool loadPriceData(CPriceQuoteArray& quotes, bool freshen, SFString& message, SFUint32 step = 1);
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
    open = 0.0;
    high = 0.0;
    low = 0.0;
    close = 0.0;
    quoteVolume = 0.0;
    volume = 0.0;
    weightedAvg = 0.0;

    // EXISTING_CODE
    date = earliestDate;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Copy(const CPriceQuote& pr) {
    Clear();
    CBaseNode::Copy(pr);

    timestamp = pr.timestamp;
    open = pr.open;
    high = pr.high;
    low = pr.low;
    close = pr.close;
    quoteVolume = pr.quoteVolume;
    volume = pr.volume;
    weightedAvg = pr.weightedAvg;

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
inline SFString CPriceQuote::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CPriceQuoteArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CPriceQuoteArray);
IMPLEMENT_ARCHIVE_LIST(CPriceQuoteList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool loadPriceData(CPriceQuoteArray& quotes, bool freshen, SFString& message, SFUint32 step = 1);
// EXISTING_CODE
}  // namespace qblocks

