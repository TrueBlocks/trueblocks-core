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
#include <string>
#include "ethslurp.h"
#include "options.h"

extern void findInternalTxIndex(CTransaction& trans);
//--------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    CCachedAccount theAccount;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        cerr << "Processing: " << substitute(command, options.getOutputFn(), "--output_filename--") << "\n";

        if (!Slurp(theAccount, options)) {
            for (auto err : options.errors) {
                LOG_WARN(err);
            }

        } else {
            sort(theAccount.transactions.begin(), theAccount.transactions.end(), sortByBlockNumTxId);
            blknum_t lastBlock = NOPOS;
            blknum_t lastTxId = NOPOS;
            for (auto& trans : theAccount.transactions) {
                bool show1 = inRange(trans.blockNumber, options.blocks.start, options.blocks.stop);
                bool show2 = (trans.blockNumber != lastBlock || trans.transactionIndex != lastTxId);
                trans.m_showing = (show1 && show2);
                lastBlock = trans.blockNumber;
                lastTxId = trans.transactionIndex;
            }
            theAccount.displayString = options.displayString;
            ostringstream os;
            theAccount.Format(os, options.formatString);
            if (expContext().exportFmt == TXT1 || expContext().exportFmt == CSV1)
                cout << trim(trim(os.str(), '\n'), ',') << endl;
            else
                cout << "[" << trim(trim(substitute(os.str(), "\n", " "), ' '), ',') << "]" << endl;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------
bool Slurp(CCachedAccount& theAccount, COptions& options) {
    theAccount.transactions.clear();
    theAccount = CCachedAccount();
    theAccount.addr = options.addrs[0];
    cerr << "\t"
         << "Slurping " << theAccount.addr << "\n";

    bool first = true;
    for (auto type : options.typesList) {
        string_q cacheFilename =
            getPathToCache("slurps/" + theAccount.addr + (type == "ext" || type.empty() ? "" : "." + type) + ".bin");
        if (fileExists(cacheFilename)) {
            CArchive inArchive(READING_ARCHIVE);
            if (inArchive.Lock(cacheFilename, modeReadOnly, LOCK_NOWAIT)) {
                theAccount.Serialize(inArchive);
                inArchive.Close();
            } else {
                options.errors.push_back("Could not open file: '" + cacheFilename + "'");
            }
        }

        // Keep reading until we get less than a full page
        bool done = false;
        while (!done) {
            string_q url = string_q("https://api.etherscan.io/api?module=account&sort=asc") +
                           "&action=" + toEtherscan(type) + "&address=" + theAccount.addr +
                           "&page=" + uint_2_Str(theAccount.latestPage) + "&offset=" + uint_2_Str(5000) + "&apikey=" +
                           getApiKey("Etherscan",
                                     "http:/"
                                     "/api.etherscan.io/apis");

            string_q responseStr = urlToString(url);
            if (getGlobalConfig()->getConfigBool("dev", "debug_curl", false)) {
                cerr << "[calling EtherScan: " << url << endl;
                cerr << "[result: " << url << responseStr << endl;
            }
            if (!contains(responseStr, "\"message\":\"OK\"")) {
                options.errors.push_back("Error: " + responseStr + ".");
                done = true;

            } else {
                CESResult response;
                response.parseJson3(responseStr);
                uint64_t nRecords = countOf(response.result, '}');
                LOG4("Downloaded ", nRecords, " records from EtherScan.\r");

                // pre allocate the array (probably wrong input here--reserve takes max needed size, not addition size
                // needed)
                theAccount.transactions.reserve(theAccount.transactions.size() + nRecords);

                uint64_t nAdded = 0;
                CTransaction trans;
                while (trans.parseJson3(response.result)) {
                    if (type == "int") {
                        findInternalTxIndex(trans);
                    } else if (type == "token" || type == "nfts") {
                        trans.hasToken = true;
                    } else if (type == "miner") {
                        trans.from = "0xBlockReward";
                        trans.transactionIndex = 99999;
                    } else if (type == "uncles") {
                        trans.from = "0xUncleReward";
                        trans.transactionIndex = 99998;
                    }
                    theAccount.transactions.push_back(trans);
                    theAccount.markLatest(trans);
                    trans = CTransaction();  // reset
                    nAdded++;
                    LOG4("Adding records from block ", trans.blockNumber, "...\r");
                }

                // We're done if we got a page of less than 5,000 records, otherwise we will process the next page
                done = (nRecords < 5000);
                if (!done)
                    theAccount.latestPage++;

                if (nAdded) {
                    LOG4("Appending ", nAdded, " new records, total ", theAccount.transactions.size(), "\r");
                    CArchive outArchive(WRITING_ARCHIVE);
                    if (outArchive.Lock(cacheFilename, (first ? modeWriteCreate : modeWriteAppend), LOCK_CREATE)) {
                        lockSection();
                        theAccount.Serialize(outArchive);
                        outArchive.Close();
                        unlockSection();
                        first = false;

                    } else {
                        options.errors.push_back("\tCould not open file: " + cacheFilename);
                        continue;
                    }
                }
            }
        }
        if (shouldQuit())
            break;
        LOG4("Processed transaction of type ", type, ". nTrans: ", theAccount.transactions.size());
    }
    cerr << string_q(120, ' ') << "\r";
    return (theAccount.transactions.size() > 0);
}

//---------------------------------------------------------------------------
void findInternalTxIndex(CTransaction& trans) {
    CTransaction thing;
    getTransaction(thing, trans.hash);
    trans.transactionIndex = thing.transactionIndex;
    trans.hasToken = trans.hasToken;
}
