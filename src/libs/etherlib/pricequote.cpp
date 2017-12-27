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
#include "pricequote.h"
#include "pricesource.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPriceQuote, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextPricequoteChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextPricequoteChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CPriceQuote::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPricequoteChunk, this);
}

//---------------------------------------------------------------------------
SFString nextPricequoteChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CPriceQuote *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CPriceQuote::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if ( fieldName % "date" || fieldName % "timestamp" ) {
        timestamp = toLong(fieldValue);
        date = dateFromTimeStamp((timestamp_t)timestamp);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "close" ) { close = toDouble(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPriceQuote::finishParse() {
    // EXISTING_CODE
    date = dateFromTimeStamp((timestamp_t)timestamp);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPriceQuote::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CPriceQuote*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> timestamp;
    archive >> close;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPriceQuote::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    ((CPriceQuote*)this)->m_schema = getVersionNum();
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << timestamp;
    archive << close;

    return true;
}

//---------------------------------------------------------------------------
void CPriceQuote::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CPriceQuote, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CPriceQuote, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CPriceQuote, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CPriceQuote, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CPriceQuote, "close", T_DOUBLE, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPriceQuote, "schema");
    HIDE_FIELD(CPriceQuote, "deleted");
    HIDE_FIELD(CPriceQuote, "showing");

    // EXISTING_CODE
    ADD_FIELD(CPriceQuote, "date", T_DATE, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextPricequoteChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CPriceQuote *pri = (const CPriceQuote *)dataPtr;
    if (pri) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if ( fieldIn % "date" ) return pri->date.Format(FMT_JSON);
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, pri);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CPriceQuote::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CPriceQuote::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    if (m_schema < 2000) {
        //
        // we used to store a lot more data than we do now, so
        // we have to read this older format old format was:
        // timestamp, open, high, low, close, quoteVolume, volume,
        // weightedAvg but now we read only timstamp and close
        double junk;
        archive >> timestamp;
        archive >> junk;
        archive >> junk;
        archive >> junk;
        archive >> close;
        archive >> junk;
        archive >> junk;
        archive >> junk;
        finishParse();
        done = true;

    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CPriceQuote::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextPricequoteChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "close" ) return fmtFloat(close);
            break;
        case 't':
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPriceQuote& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//-----------------------------------------------------------------------------------
uint64_t indexFromTimeStamp(const CPriceQuoteArray& quotes, timestamp_t ts) {
    timestamp_t first = (timestamp_t)quotes[0].timestamp;
    if (ts < first)
        return 0;
    timestamp_t since = ts - first;
    return min(quotes.getCount()-1, uint32_t(since / (5*60)));
}

//-----------------------------------------------------------------------
SFString asDollars(timestamp_t ts, SFUintBN weiIn) {
    if (weiIn == 0)
        return "";
    static CPriceQuoteArray quotes;
    if (!quotes.getCount()) {
        SFString message;
        CPriceSource source;
        if (!loadPriceData(source, quotes, false, message, 1)) {
            cerr << "Cannot load price data. Quitting.\n";
            exit(0);
        }
    }
    uint64_t index = indexFromTimeStamp(quotes, ts);
    CPriceQuote *q = &quotes[(uint32_t)index];
    double price = q->close * 100.0;
    uint64_t pInt = (uint64_t)price;
    weiIn *= pInt;
    weiIn /= 100;
    return wei2Ether(to_string(weiIn).c_str());
}

//-----------------------------------------------------------------------
SFString insertCommas(const SFString& dIn) {
    SFString d = dIn;
    d.Reverse();
    SFString ret;
    while (!d.empty()) {
        SFString three = d.substr(0,3);
        d = d.substr(3);
        three.Reverse();
        ret = (d.empty()?"":",") + three + ret;
    }
    return ret;
}

//-----------------------------------------------------------------------
SFString dispDollars(timestamp_t ts, SFUintBN weiIn) {
    SFString sBal = asDollars(ts, weiIn);
    SFString d = nextTokenClear(sBal,'.');
    d = insertCommas(d);
    sBal = (sBal.empty() ? "0.00" : d + "." + sBal.substr(0,2));
    return sBal;
}
// EXISTING_CODE
}  // namespace qblocks

