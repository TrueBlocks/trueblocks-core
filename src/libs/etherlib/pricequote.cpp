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
#include "transaction.h"
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPriceQuote, CBaseNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextPricequoteChunk(const SFString& fieldIn, const void *data);
static SFString nextPricequoteChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void CPriceQuote::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPricequoteChunk, this);
}

//---------------------------------------------------------------------------
SFString nextPricequoteChunk(const SFString& fieldIn, const void *data) {
    const CPriceQuote *pri = (const CPriceQuote *)data;
    if (pri) {
        // Give customized code a chance to override first
        SFString ret = nextPricequoteChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'c':
                if ( fieldIn % "close" ) return fmtFloat(pri->close);
                break;
            case 'h':
                if ( fieldIn % "high" ) return fmtFloat(pri->high);
                break;
            case 'l':
                if ( fieldIn % "low" ) return fmtFloat(pri->low);
                break;
            case 'o':
                if ( fieldIn % "open" ) return fmtFloat(pri->open);
                break;
            case 'q':
                if ( fieldIn % "quoteVolume" ) return fmtFloat(pri->quoteVolume);
                break;
            case 't':
                if ( fieldIn % "timestamp" ) return asStringU(pri->timestamp);
                break;
            case 'v':
                if ( fieldIn % "volume" ) return fmtFloat(pri->volume);
                break;
            case 'w':
                if ( fieldIn % "weightedAvg" ) return fmtFloat(pri->weightedAvg);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, pri);
        if (!ret.empty())
            return ret;
    }

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
        case 'h':
            if ( fieldName % "high" ) { high = toDouble(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "low" ) { low = toDouble(fieldValue); return true; }
            break;
        case 'o':
            if ( fieldName % "open" ) { open = toDouble(fieldValue); return true; }
            break;
        case 'q':
            if ( fieldName % "quoteVolume" ) { quoteVolume = toDouble(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toUnsigned(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "volume" ) { volume = toDouble(fieldValue); return true; }
            break;
        case 'w':
            if ( fieldName % "weightedAvg" ) { weightedAvg = toDouble(fieldValue); return true; }
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

    if (!preSerialize(archive))
        return false;

    archive >> timestamp;
    archive >> open;
    archive >> high;
    archive >> low;
    archive >> close;
    archive >> quoteVolume;
    archive >> volume;
    archive >> weightedAvg;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPriceQuote::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << timestamp;
    archive << open;
    archive << high;
    archive << low;
    archive << close;
    archive << quoteVolume;
    archive << volume;
    archive << weightedAvg;

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
    ADD_FIELD(CPriceQuote, "timestamp", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPriceQuote, "open", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CPriceQuote, "high", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CPriceQuote, "low", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CPriceQuote, "close", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CPriceQuote, "quoteVolume", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CPriceQuote, "volume", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CPriceQuote, "weightedAvg", T_DOUBLE, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPriceQuote, "schema");
    HIDE_FIELD(CPriceQuote, "deleted");

    // EXISTING_CODE
    ADD_FIELD(CPriceQuote, "date", T_DATE, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextPricequoteChunk_custom(const SFString& fieldIn, const void *data) {
    const CPriceQuote *pri = (const CPriceQuote *)data;
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
bool CPriceQuote::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CPriceQuote::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
SFString priceDatabasePath(void) {
    return blockCachePath("prices/poloniex.bin");
}

//---------------------------------------------------------------------------
bool loadPriceData(CPriceQuoteArray& quotes, bool freshen, SFString& message, SFUint32 step) {

    SFString cacheFile = priceDatabasePath();

    // Load and possibly refresh the price database
    SFTime lastRead = SFTime(2015, 1, 1, 0, 0, 0);  // Ethereum didn't even exist before July 2015
    if (fileExists(cacheFile)) {
        SFArchive archive(true, NO_SCHEMA, true);
        if (archive.Lock(cacheFile, binaryReadOnly, LOCK_NOWAIT)) {
            archive.readHeader(); // we read the header even though it may not be the current version...
            if (!archive.isSchema(NO_SCHEMA)) // ... if it's not the current version return to begin of file
                archive.Seek(0, SEEK_SET);
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
                cerr << "Retrieving data...\r";
                cerr.flush();
            }
            timestamp_t start = toTimeStamp(nextRead);
            // Polinex will give us as much as it has on the following day. Do this to account for time zones
            timestamp_t end   = toTimeStamp(EOD(BOND(now)));

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

                bool addToArray = (timestamp_t)quote.timestamp > toTimeStamp(lastRead);
                if (verbose > 1) {
                    cerr << "addToArray: " << addToArray
                    << " nFields: " << nFields
                    << " quote: " << dateFromTimeStamp((timestamp_t)quote.timestamp)
                    << " lastRead: " << lastRead
                    << " lastRead(ts): " << dateFromTimeStamp(toTimeStamp(lastRead)) << "\n";
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
            SFArchive archive(false, NO_SCHEMA, true);
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
    return " ($" + wei2Ether(to_string(weiIn).c_str()) + ")";
}
// EXISTING_CODE
}  // namespace qblocks
