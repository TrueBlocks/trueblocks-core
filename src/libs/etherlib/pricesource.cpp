/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
    bool parsePoloniex(CPriceQuote& quote, string_q& str) {
        return quote.parseJson3(str);
    }

    //---------------------------------------------------------------------------
    string_q CPriceSource::getDatabasePath(string_q& source) const {
        source = substitute(substitute(url, "http://", ""), "https://", "");
        source = nextTokenClear(source, '.');
        string_q ret = getCachePath("prices/" + source + "_" + pair + ".bin");
        establishFolder(ret);
        return ret;
    }

    extern size_t dotDot(char *ptr, size_t size, size_t nmemb, void *userdata);
    //---------------------------------------------------------------------------
    bool loadPriceData(const CPriceSource& source, CPriceQuoteArray& quotes, bool freshen,
                            string_q& message, uint64_t step) {

        string_q dataSource;
        string_q cacheFile = source.getDatabasePath(dataSource);

        // Load and possibly refresh the price database
        time_q lastRead = time_q(2015, 1, 1, 0, 0, 0);
        if (contains(source.pair, "BTC"))
            lastRead = time_q(2009, 1, 1, 0, 0, 0);

        if (!fileExists(cacheFile)) {
            string_q zipFile = configPath("cache/prices/") + dataSource + "_" + source.pair + ".bin.gz";
            if (zipFile != cacheFile + ".gz") {
                string_q cmd = "cp -f " + zipFile + " " + cacheFile + ".gz";
                cmd += (" ; cd " + getCachePath("prices/") + " ; gunzip *.gz");
                doCommand(cmd);
            }
        }

        if (fileExists(cacheFile)) {
            if (!isTestMode())
                cerr << "Updating prices...\r";
            CArchive priceCache(READING_ARCHIVE);
            if (priceCache.Lock(cacheFile, modeReadOnly, LOCK_NOWAIT)) {
                priceCache.readHeader();  // we read the header even though it may not be the current version...
                priceCache >> lastRead.m_nSeconds;
                priceCache >> quotes;
                priceCache.Close();
                if (verbose) {
                    string_q date = lastRead.Format(FMT_JSON);
                    string_q count = uint_2_Str(quotes.size());
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
        time_q firstDate = time_q(2015, 6, 1, 0, 0, 0);
        time_q now       = Now();
        time_q nextRead  = (lastRead == time_q(2015, 1, 1, 0, 0, 0) ? firstDate : lastRead + 5*60);  // 5 minutes

// #define DEBUG
#ifdef DEBUG
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
            time_q prevLast = lastRead;
            if (freshen) {
                if (verbose < 2) {
                    if (!isTestMode())
                        cerr << "Retrieving price history data...";
                    cerr.flush();
                }
                timestamp_t start = date_2_Ts(nextRead);
                // Polinex will give us as much as it has on the following day. Do this to account for time zones
                timestamp_t end   = date_2_Ts(EOD(BOND(now)));
                timestamp_t thirtyDays = (30 * 24 * 60 * 60);
                if (end > start + thirtyDays)
                    end = start + thirtyDays;

                if (verbose > 1) {
                    cerr << "start: " << ts_2_Date(start) << "\n";
                    cerr << "end: " << ts_2_Date(end) << "\n";
                }

                // we need to read some data
                string_q url = source.url;
                replace(url, "[{PAIR}]",   source.pair);
                replace(url, "[{START}]",  int_2_Str(start));
                replace(url, "[{END}]",    int_2_Str(end));
                replace(url, "[{PERIOD}]", int_2_Str(5*60));
                if (verbose)
                    cerr << "Fetching: " << url << "\n";

                // Ask Poloniex for the latest data
                string_q response = urlToString(url, dotDot);
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
                CPriceQuote quote;
                while ((*source.func)(quote, response)) {
                    bool addToArray = (timestamp_t)quote.timestamp > date_2_Ts(lastRead);
                    if (verbose > 1) {
                        cerr << "addToArray: " << addToArray
                        << " quote: " << ts_2_Date((timestamp_t)quote.timestamp)
                        << " lastRead: " << lastRead
                        << " lastRead(ts): " << ts_2_Date(date_2_Ts(lastRead)) << "\n";
                    } else {
                        if (!isTestMode())
                            cerr << ts_2_Date(quote.timestamp) << "                    \r";
                    }

                    // So as to not inadvertantly add records we already have
                    if (addToArray) {
                        // First entry should be on a two hour mark so we hit midnight in default two hour case
                        if (quotes.size() || (quote.date.onTheHour() && (!quote.date.GetHour()%2))) {
                            quotes.push_back(quote);
#ifdef DEBUG
                            cerr << quote.Format() << "\n";
#endif
                            lastRead = ts_2_Date((timestamp_t)quote.timestamp);
                        }
                    }
                    quote = CPriceQuote();  // reset
                }
            }

            // Write the database to the cache
            if (prevLast != lastRead && freshen) {
                CArchive priceCache(WRITING_ARCHIVE);
                if (!priceCache.Lock(cacheFile, modeWriteCreate, LOCK_WAIT)) {
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
            string_q count = uint_2_Str(quotes.size());
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
            thread_local int cnt = 0;
            if (!(++cnt % 25)) {
                cerr << ".";
                cerr.flush();
            }
        }
        return size * nmemb;
    }

}  // namespace qblocks
