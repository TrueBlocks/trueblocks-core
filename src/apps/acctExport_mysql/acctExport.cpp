/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <mysql.h>
#include "options.h"
#include "sql_database.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

#if 1
    cerr << "This tool is disabled because of issue #132 FALLBACK BUG in the\n";
    cerr << "private repo.\n";
    cerr.flush();
#else
    etherlib_init(quickQuitHandler);
    tokenlib_init();

    COptions visitor;
    if (!visitor.prepareArguments(argc, argv))
        return 0;

    if (!visitor.parseArguments(visitor.commandList))
        return 0;

    visitor.displayFromCache();

    etherlib_cleanup();
#endif

    return 0;
}

//-----------------------------------------------------------------------
class CAddrAccumulator;
extern bool isInteresting(const COptions *opts, const CTransaction *trans, CAddrAccumulator *acc);

//-----------------------------------------------------------------------
string_q fmt = "([{BLOCKNUMBER}], [{TRANSACTIONINDEX}], [{TRACEID}], '[{FROM}]', '[{TO}]', [{VALUE}], [{GASUSED}], [{GASPRICE}], [{ISERROR}], '[{ENCODING}]', '[{ARTICULATED}]')";
//-----------------------------------------------------------------------
class CAddrAccumulator {
public:
    address_t current;
    CAddressArray results;
    string_q toJsonArray(void) {
        ostringstream os;
        for (uint32_t i = 0 ; i < results.size() ; i++) {
            os << results[i];
            if (i < results.size()-1)
                os << ", ";
        }
        return os.str().c_str();
    }
    string_q toMySQL(blknum_t bn, blknum_t tx, blknum_t tc) {
        string_q ret;
        for (uint32_t i = 0 ; i < results.size() ; i++) {
            ret += "('" + results[i] + "', " + uint_2_Str(bn) + ", " + uint_2_Str(tx) + ", " + uint_2_Str(tc) + ")";
            if (i < results.size()-1)
                ret += ", ";
        }
        return ret;
    }
    string_q toMySQL4Trans(const CTransaction *t, blknum_t tc) {
        string_q ret;
        ret += t->Format(substitute(fmt, "[{TRACEID}]", uint_2_Str(tc)));
        return ret;
    }
};

//-----------------------------------------------------------------------
bool COptions::displayFromCache(void) {

    CMySqlServer sqlServer;
    if (sql_on) {
        string_q host = getGlobalConfig()->getConfigStr("mysql", "host", "");
        string_q user = getGlobalConfig()->getConfigStr("mysql", "user", "");
        string_q pw   = getGlobalConfig()->getConfigStr("mysql", "pw",   "");
        sqlServer.setConnection(host, user, pw, true);
        if (!sqlServer.useDatabase("tokenomics_local"))
            return false;
    }

    string_q cacheFilename = "./cache/" + watches[0].address + ".acct.bin";
    if (fileExists(cacheFilename + ".lck")) {
        cout << usageStr("The cache lock file is present. The program is either already running or it did not end cleanly the\n"
                         "\tlast time it ran. Quit the already running program or, if it is not running, remove the lock\n"
                         "\tfile: " + cacheFilename + ".lck'. Quitting...");
        cout.flush();
        return false;
    }

    CArchive txCache(READING_ARCHIVE);
    if (!txCache.Lock(cacheFilename, binaryReadOnly, LOCK_NOWAIT)) {
        cout << usageStr("Cannot lock file: " + cacheFilename + ". Quitting...");
        cout.flush();
        return false;
    }

    if (!sql_on) {
        CTransaction t;
        cout << toLower(t.Format(substitute(substitute(substitute(transExport, "{IS", "{IS_"), "w:10:INPUT", "encoding"), "{", "{p:"))) << "\n";
    }

    string_q mtFields = "monitorAddress, blockNumber, transID, traceID";
    string_q tFields = "blockNumber, transID, traceID, addr_2_Str, str_2_Addr, valueWei, gasUsed, gasPrice, isError, encoding, articulated";
    string_q mtRows;
    string_q tRows;
    ASSERT(txCache.isOpen());

    CBlock latest;
    getBlock(latest, "latest");

    uint32_t cnt = 0;
    CBlock block;
    while (!txCache.Eof() && ++cnt < maxRows) {

        uint64_t blockNum, transID;
        txCache >> blockNum >> transID;
        if (blockNum > block.blockNumber) {  // only re-read if it's a new block

            if (block.blockNumber) {
                // not the first block we've read
                if (sql_on) {
                    // SEE TODO NOTE BELOW
                    sqlServer.insertRows("block", "blockNumber, timeStamp, isFinalized", block.Format("([{BLOCKNUMBER}],[{TIMESTAMP}],1)"));
                    sqlServer.insertRows("transaction",          tFields,  tRows);
                    sqlServer.insertRows("monitor_transaction", mtFields, mtRows);
                    if (!verbose) {
                        cerr << "Writing record " << cnt;
                        if (maxRows != NOPOS)
                            cerr << " of " << maxRows;
                        cerr << " at block " << block.blockNumber << "\r";
                        cerr.flush();
                    }
                }
                mtRows = "";
                tRows = "";
            }
            block = CBlock();
            bool ret = readBlockFromBinary(block, getBinaryFilename(blockNum));
            if (!ret) {
                getBlock(block, blockNum);
                block.finalized = isBlockFinal(block.timestamp, latest.timestamp, (60 * 4));
                writeBlockToBinary(block, getBinaryFilename(blockNum));
            }
        }

        CTransaction *trans = &block.transactions.at(transID);
        trans->pBlock = &block;
        trans->function = promoteToFunc(trans);

        CAddrAccumulator acc;
        if (isInteresting(this, trans, &acc)) {
            if (!sql_on) {
                cout << trans->Format(substitute(transExport, "[{TRACEID}]", "0")) << "\n";
            } else {
                string_q res = acc.toMySQL4Trans(trans, 0);
                if (!res.empty()) {
                    if (!tRows.empty())
                        tRows += ",\n";
                    tRows += res;
                }
                res = acc.toMySQL(trans->blockNumber, trans->transactionIndex, 0);
                if (!res.empty()) {
                    if (!mtRows.empty())
                        mtRows += ",\n";
                    mtRows += res;
                }
            }
        }

        if ((!ddosRange(block.blockNumber) || getTraceCount(trans->hash) < 250))
            getTraces(trans->traces, trans->hash);

        for (uint32_t ttt = 0 ; ttt < trans->traces.size() ; ttt++) {
            CTrace *trace = &trans->traces.at(ttt);
            CTransaction trans1;
            trans1.pBlock                  = &block;
            trans1.blockNumber             = trans->blockNumber;
            trans1.timestamp               = block.timestamp;
            trans1.transactionIndex        = trans->transactionIndex;
            trans1.gas                     = trans->gas;
            trans1.gasPrice                = trans->gasPrice;
            trans1.to                      = trace->action.to;
            trans1.from                    = trace->action.from;
            trans1.input                   = trace->action.input;
            trans1.value                   = trace->action.value;
            trans1.receipt.contractAddress = trace->action.address;
            trans1.receipt.gasUsed         = trace->result.gasUsed;
            trans1.function                = promoteToFunc(&trans1);
            CAddrAccumulator acc1;
            if (isInteresting(this, &trans1, &acc1)) {
                if (!sql_on) {
                    cout << trans1.Format(substitute(transExport, "[{TRACEID}]", uint_2_Str(ttt+10))) << "\n";
                } else {
                    string_q res = acc.toMySQL4Trans(trans, ttt+10);
                    if (!res.empty()) {
                        if (!tRows.empty())
                            tRows += ",\n";
                        tRows += res;
                    }
                    res = acc.toMySQL(trans->blockNumber, trans->transactionIndex, ttt+10);
                    if (!res.empty()) {
                        if (!mtRows.empty())
                            mtRows += ",\n";
                        mtRows += res;
                    }
                }
            }
        }
        cout.flush();
    }

    if (sql_on) {
        // TODO: Is the last record installed okay? Need to test
        sqlServer.insertRows("block", "blockNumber, timeStamp, isFinalized", block.Format("([{BLOCKNUMBER}],[{TIMESTAMP}],1)"));
        sqlServer.insertRows("transaction", tFields, substitute(tRows, ", ,", ""));
        sqlServer.insertRows("monitor_transaction", mtFields, substitute(mtRows, ", ,", ""));
    }

    txCache.Release();
    return true;
}

//---------------------------------------------------------------------------
bool shouldFilter(const CTransaction *trans, void *data) {
    // return true to filter all traces
    return true;
}

//-----------------------------------------------------------------------------
bool addAddress(const CAddressAppearance& item, void *data) {
    CAddrAccumulator *acc = (CAddrAccumulator*)data;
    if (acc->current == item.addr) {
        acc->results.push_back(item.addr);
    }
    return true;
}

//-----------------------------------------------------------------------
bool isInteresting(const COptions *opts, const CTransaction *trans, CAddrAccumulator *acc) {
    acc->results.clear();
    for (uint32_t i = 0 ; i < opts->watches.size() ; i++) {
        acc->current = opts->watches[i].address;
        CBlock block;
        block.transactions.push_back(*trans);
        block.forEveryUniqueAddress(addAddress, shouldFilter, acc); // Note: we look at no traces here
    }
    return (acc->results.size() > 0);
}

/*
 cerr << "Loading abis...";
 CAbi abis;
 if (fileExists("./all_funcs.bin")) {
 CArchive abiCache(READING_ARCHIVE);
 if (abiCache.Lock("./all_funcs.bin", binaryReadOnly, LOCK_NOWAIT)) {
 abiCache >> abis.abiByEncoding;
 abiCache.Release();
 }
 } else {
 abis.loadABIFromCSV("./all_funcs.txt");
 CArchive abiCache(WRITING_ARCHIVE);
 if (abiCache.Lock("./all_funcs.bin", binaryWriteCreate, LOCK_WAIT)) {
 abiCache << abis.abiByEncoding;
 abiCache.Release();
 }
 }
 cerr << "done\n";
 */
