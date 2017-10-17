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
        timestamp = toLongU(fieldValue);
        date = dateFromTimeStamp((timestamp_t)timestamp);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "close" ) { close = toDouble(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toUnsigned(fieldValue); return true; }
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

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
#define MAJOR 0
#define MINOR 2
#define BUILD 0
    uint32_t vers = ((MAJOR * 1000000) + (MINOR * 1000) + (BUILD));
    ((CPriceQuote*)this)->m_schema = vers;
    CBaseNode::SerializeC(archive);
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
    ADD_FIELD(CPriceQuote, "timestamp", T_NUMBER, ++fieldNum);
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
    if (m_schema < 2000)
    {
        // timestamp, open, high, low, close, quoteVolume, volume, weightedAvg
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
            if ( fieldName % "timestamp" ) return asStringU(timestamp);
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
#include "node.h"
SFString priceDatabasePath(void) {
    SFString ret = blockCachePath("prices/poloniex.bin");
    establishFolder(ret);
    return ret;
}

//---------------------------------------------------------------------------
bool loadPriceData(CPriceQuoteArray& quotes, bool freshen, SFString& message, SFUint32 step) {

    SFString cacheFile = priceDatabasePath();

    // Load and possibly refresh the price database
    SFTime lastRead = SFTime(2015, 1, 1, 0, 0, 0);  // Ethereum didn't even exist before July 2015
    if (fileExists(cacheFile)) {
        SFArchive archive(READING_ARCHIVE);
        if (archive.Lock(cacheFile, binaryReadOnly, LOCK_NOWAIT)) {
            archive.readHeader(); // we read the header even though it may not be the current version...
            archive >> lastRead;
            archive >> quotes;
            archive.Close();
            if (verbose) {
                SFString date = lastRead.Format(FMT_DEFAULT);
                SFString count = asString(quotes.getCount());
                if (isTestMode()) {
                    date = "Now";
                    count = "cnt";
                }
                cerr << "Read " << count << " existing price quotes (lastRead: " << date << ")\n";
            }

        } else {
            message = "Could not open cache file for reading: '" + cacheFile + "'";
            return false;
        }

    } else {
        freshen=true;
        if (verbose)
            cerr << "Price database not found. Creating it.\n";
    }

    SFString msg;
    SFTime firstDate = SFTime(2015, 6, 1, 0, 0, 0);
    SFTime now       = Now();
    SFTime nextRead  = (lastRead == SFTime(2015, 1, 1, 0, 0, 0) ? firstDate : lastRead + 5*60);  // 5 minutes
//#define DEBUGGING
#ifdef DEBUGGING
    cerr << "firstDate: " << firstDate << "\n";
    cerr << "now: " << now << "\n";
    cerr << "nextRead: " << nextRead << "\n";
    cerr.flush();
#endif
    bool reportAtEnd = verbose;
    if (nextRead > now && !freshen) {
        if (!isTestMode())
            msg = "Price database is up-to-date as of ";
        reportAtEnd = false;

    } else {
        if (!isTestMode())
            msg = "Price database has been updated to ";
        SFTime prevLast = lastRead;
        if (freshen) {
            if (verbose < 2) {
                if (!isTestMode())
                    cerr << "Retrieving price history data...\r";
                cerr.flush();
            }
            timestamp_t start = toTimestamp(nextRead);
            // Polinex will give us as much as it has on the following day. Do this to account for time zones
            timestamp_t end   = toTimestamp(EOD(BOND(now)));

            if (verbose > 1) {
                cerr << "start: " << dateFromTimeStamp(start) << "\n";
                cerr << "end: " << dateFromTimeStamp(end) << "\n";
            }

            // we need to read some data
            SFString url;
            url  = "https://poloniex.com/public?command=returnChartData";
            url += "&currencyPair=USDT_ETH";
            url += "&start="  + asString(start);
            url += "&end="    + asString(end);
            url += "&period=" + asString(5*60);

            if (verbose)
                cerr << "Fetching: " << url << "\n";

            // Ask Poloniex for the latest data
            SFString response = urlToString(url);

            // Figure out how many new records there are
            uint32_t nRecords = (uint32_t)countOf('}', response);
            nRecords--;
            if (verbose)
                cerr << "Response: " << nRecords << " were sent from Poloniex\n";
//            if (verbose > 1)
//                cerr << "JSON: " << response << "\n";

            // And grow the array so we don't have to allocate for each new record
            quotes.Grow(nRecords+10);

            // Parse the response and populate the array
            char *p = cleanUpJson((char *)(const char*)response);  // NOLINT
            while (p && *p) {
                CPriceQuote quote; uint32_t nFields = 0;
                p = quote.parseJson(p, nFields);

                bool addToArray = (timestamp_t)quote.timestamp > toTimestamp(lastRead);
                if (verbose > 1) {
                    cerr << "addToArray: " << addToArray
                    << " nFields: " << nFields
                    << " quote: " << dateFromTimeStamp((timestamp_t)quote.timestamp)
                    << " lastRead: " << lastRead
                    << " lastRead(ts): " << dateFromTimeStamp(toTimestamp(lastRead)) << "\n";
                } else {
                    if (!isTestMode())
                        cerr << dateFromTimeStamp((timestamp_t)quote.timestamp) << "\r";
                }

                // So as to not inadvertantly add records we already have
                if (nFields && addToArray) {
                    // First entry should be on a two hour mark so we hit midnight in default two hour case
                    if (quotes.getCount() || (quote.date.onTheHour() && (!quote.date.GetHour()%2))) {
                        quotes[quotes.getCount()] = quote;
#ifdef DEBUGGING
                        cerr << quote.Format() << "\n";
#endif
                        lastRead = dateFromTimeStamp((timestamp_t)quote.timestamp);
                    }
                }
            }
        }

        // Write the database to the cache
        if (prevLast != lastRead && freshen) {
            SFArchive archive(WRITING_ARCHIVE);
            if (!archive.Lock(cacheFile, binaryWriteCreate, LOCK_WAIT)) {
                message = "Could not open cache file for writing: '" + cacheFile + "'";
                return false;
            }
            archive.writeHeader();
            archive << lastRead;
            archive << quotes;
            archive.Close();
            if (verbose) {
                cerr << "Wrote " << quotes.getCount() << " price quotes to file ";
                cerr << "(lastRead: " << lastRead << ").\n";
            }
        } else {
            if (!isTestMode())
                msg = "Data not written because no new data, or no -freshen flag. ";
        }
    }

    if (!reportAtEnd) {
        SFString date = lastRead.Format(FMT_DEFAULT);
        SFString count = asString(quotes.getCount());
        if (isTestMode()) {
            date = "Now";
            count = "cnt";
        }
        if (!isTestMode())
            cerr << msg << date << " : " << count << " records\n";
    }

    if (step != 1) {
        CPriceQuoteArray ret;
        uint32_t cur = 0;
        for (uint32_t i = 0 ; i < quotes.getCount() ; i += step)
            ret[cur++] = quotes[i];
        quotes = ret;
    }

    return true;
}

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
        if (!loadPriceData(quotes, false, message, 1)) {
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
    return "$" + wei2Ether(to_string(weiIn).c_str());
}

//-----------------------------------------------------------------------
SFString dispDollars(timestamp_t ts, SFUintBN weiIn) {
    SFString sBal = asDollars(ts, weiIn);
    SFString d = nextTokenClear(sBal,'.');
    sBal = (sBal.empty() ? "$0.00" : d + "." + sBal.substr(0,2));
    return sBal;
}
// EXISTING_CODE
}  // namespace qblocks

