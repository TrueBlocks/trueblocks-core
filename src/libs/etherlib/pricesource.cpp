/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "etherlib.h"

namespace qblocks {

    //---------------------------------------------------------------------------
    char *parsePoloniex(CPriceQuote& quote, char *p) {
        size_t nFields = 0;
        return quote.parseJson(p, nFields);
    }

    //---------------------------------------------------------------------------
    string_q CPriceSource::getDatabasePath(void) const {
        string_q source = substitute(substitute(url, "http://", ""), "https://", "");
        source = nextTokenClear(source, '.');
        string_q ret = blockCachePath("prices/" + source + "_" + pair + ".bin");
        establishFolder(ret);
        return ret;
    }

    extern size_t dotDot(char *ptr, size_t size, size_t nmemb, void *userdata);
    //---------------------------------------------------------------------------
    bool loadPriceData(const CPriceSource& source, CPriceQuoteArray& quotes, bool freshen,
                            string_q& message, uint64_t step) {

        string_q cacheFile = source.getDatabasePath();

        // Load and possibly refresh the price database
        SFTime lastRead = SFTime(2015, 1, 1, 0, 0, 0);
        if (contains(source.pair, "BTC"))
            lastRead = SFTime(2009, 1, 1, 0, 0, 0);
        if (fileExists(cacheFile)) {
            if (!isTestMode())
                cerr << "Updating prices...\r";
            SFArchive priceCache(READING_ARCHIVE);
            if (priceCache.Lock(cacheFile, binaryReadOnly, LOCK_NOWAIT)) {
                priceCache.readHeader();  // we read the header even though it may not be the current version...
                priceCache >> lastRead.m_nSeconds;
                priceCache >> quotes;
                priceCache.Close();
                if (verbose) {
                    string_q date = lastRead.Format(FMT_JSON);
                    string_q count = toStringU(quotes.size());
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
            freshen = true;
            if (verbose)
                cerr << "Price database not found. Creating it.\n";
        }

        string_q msg;
        SFTime firstDate = SFTime(2015, 6, 1, 0, 0, 0);
        SFTime now       = Now();
        SFTime nextRead  = (lastRead == SFTime(2015, 1, 1, 0, 0, 0) ? firstDate : lastRead + 5*60);  // 5 minutes

// #define DEBUGGING
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
                        cerr << "Retrieving price history data...";
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
                string_q url = source.url;
                replace(url, "[{PAIR}]",   source.pair);
                replace(url, "[{START}]",  toString(start));
                replace(url, "[{END}]",    toString(end));
                replace(url, "[{PERIOD}]", toString(5*60));
                if (verbose)
                    cerr << "Fetching: " << url << "\n";

                // Ask Poloniex for the latest data
                setCurlNoteFunc(dotDot);
                string_q response = urlToString(url);
                setCurlNoteFunc(NULL);
                cerr << "\r";

                // Figure out how many new records there are
                size_t nRecords = countOf(response, '}');
                nRecords--;
                if (verbose)
                    cerr << "Response: " << nRecords << " were sent from Poloniex\n";
                //            if (verbose > 1)
                //                cerr << "JSON: " << response << "\n";

                // And grow the array so we don't have to allocate for each new record
                quotes.reserve(nRecords+10);

                // Parse the response and populate the array
                char *p = cleanUpJson((char *)response.c_str());  // NOLINT
                while (p && *p) {
                    CPriceQuote quote;
                    quote.timestamp = toTimestamp(SFTime(2015, 1, 1, 0, 0, 0));  // Ensures we get a good parse
                    p = (*source.func)(quote, p);

                    bool addToArray = (timestamp_t)quote.timestamp > toTimestamp(lastRead);
                    if (verbose > 1) {
                        cerr << "addToArray: " << addToArray
                        << " quote: " << dateFromTimeStamp((timestamp_t)quote.timestamp)
                        << " lastRead: " << lastRead
                        << " lastRead(ts): " << dateFromTimeStamp(toTimestamp(lastRead)) << "\n";
                    } else {
                        if (!isTestMode())
                            cerr << dateFromTimeStamp(quote.timestamp) << "                    \r";
                    }

                    // So as to not inadvertantly add records we already have
                    if (addToArray) {
                        // First entry should be on a two hour mark so we hit midnight in default two hour case
                        if (quotes.size() || (quote.date.onTheHour() && (!quote.date.GetHour()%2))) {
                            quotes.push_back(quote);
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
                SFArchive priceCache(WRITING_ARCHIVE);
                if (!priceCache.Lock(cacheFile, binaryWriteCreate, LOCK_WAIT)) {
                    message = "Could not open cache file for writing: '" + cacheFile + "'";
                    return false;
                }
                priceCache.writeHeader();
                priceCache << lastRead.m_nSeconds;
                priceCache << quotes;
                priceCache.Close();
                if (verbose) {
                    cerr << "Wrote " << quotes.size() << " price quotes to file ";
                    cerr << "(lastRead: " << lastRead << ").\n";
                }
//            } else {
//                if (!isTestMode())
//                    msg = "Data not written because no new data, or no -freshen flag. ";
            }
        }

        if (!reportAtEnd) {
            string_q date = lastRead.Format(FMT_JSON);
            string_q count = toStringU(quotes.size());
            if (isTestMode()) {
                date = "Now";
                count = "cnt";
            }
            if (!isTestMode())
                cerr << msg << date << " : " << count << " records\n";
        }

        if (step != 1) {
            CPriceQuoteArray ret;
            for (size_t i = 0 ; i < quotes.size() ; i += step)
                ret.push_back(quotes[i]);  // grows the vector
            quotes = ret;
        }

        return true;
    }

    const char* STR_PRICE_URL =
    "https://poloniex.com/public"
        "?command=returnChartData"
        "&currencyPair=[{PAIR}]"
        "&start=[{START}]"
        "&end=[{END}]"
        "&period=[{PERIOD}]";

    //---------------------------------------------------------------------------
    size_t dotDot(char *ptr, size_t size, size_t nmemb, void *userdata) {
        if (!isTestMode()) {
            static int cnt=0;
            if (!(++cnt % 25)) {
                cerr << ".";
                cerr.flush();
            }
        }
        return size * nmemb;
    }

}  // namespace qblocks
