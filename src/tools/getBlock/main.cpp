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
#include "etherlib.h"
#include "options.h"

extern bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance);
//------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // We want to get the latestBlock prior to turning on --prove for example
    COptions options;
    getBlock_light(options.latest, "latest");
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
#ifndef PROVING
        if (!options.quiet && options.filterType.empty())
            cout << (options.isMulti() ? "[" : "");
#else
        if (!options.quiet && !options.showAddrs && !options.uniqAddrs && !options.counting && !expContext().proving)
            cout << (options.isMulti() ? "[" : "");
        if (expContext().proving)
            expContext().proof << "[";
#endif

        string_q list = options.getBlockNumList();
        while (!list.empty() && !shouldQuit()) {
            blknum_t bn = str_2_Uint(nextTokenClear(list, '|'));
            if (!options.filterType.empty()) {
                getAddresses(bn, options);

            } else if (options.isCheck) {
                string_q checkResults = checkOneBlock(bn, options);
                cout << checkResults << (options.quiet > 1 ? "" : "\n");
                cout.flush();

            } else {
                string_q result = doOneBlock(bn, options);
                if (options.normalize) {
                    if (verbose)
                        cout << bn << "\n";
                    result = normalizeBlock(result, false, bn >= byzantiumBlock);
                }
                if (!options.quiet) {
                    if (!result.empty()) {
                        cout << result;
                        if (!list.empty())
                            cout << ",";
                        cout << "\n";
                    }

                } else {
                    interumReport(cerr, bn);
                }
            }
        }

#ifndef PROVING
        if (!options.quiet && options.filterType.empty())
            cout << (options.isMulti() ? "]" : "");
#else
        if (!options.quiet && !options.showAddrs && !options.uniqAddrs && !options.counting && !expContext().proving)
            cout << (options.isMulti() ? "]" : "");
        if (expContext().proving)
            expContext().proof << "]";
#endif
    }

    etherlib_cleanup();
    return 0;
}

//------------------------------------------------------------
string_q doOneBlock(uint64_t num, const COptions& opt) {

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    string_q numStr = uint_2_Str(num);
    if (opt.isRaw) {

//        double start = qbNow();
        if (!queryRawBlock(result, numStr, true, opt.hashes)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        } else {
//            double end = qbNow();
//            cerr << numStr << "\t" << (end - start) << "\n";
            if (opt.force) {  // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson3(result);
                result = generic.result;
                if (gold.parseJson3(result)) {
                    string_q fileName = getBinaryCacheFilename(CT_BLOCKS, num);
                    gold.finalized = isBlockFinal(gold.timestamp, opt.latest.timestamp, opt.secsFinal);
                    writeBlockToBinary(gold, fileName);
                }
            }
        }

    } else {
        string_q fileName = getBinaryCacheFilename(CT_BLOCKS, gold.blockNumber);
        if (opt.isCache) {

            // --source::cache mode doesn't include timestamp in transactions
            readBlockFromBinary(gold, fileName);
            for (size_t t = 0 ; t < gold.transactions.size() ; t++)
                gold.transactions.at(t).timestamp = gold.timestamp;  // .at cannot go past end of vector!

        } else {
            queryBlock(gold, numStr, (opt.api_mode ? false : true));
        }
        if (gold.blockNumber == 0 && gold.timestamp == 0)
            gold.timestamp = blockZeroTs;
        gold.finalized = isBlockFinal(gold.timestamp, opt.latest.timestamp, opt.secsFinal);

        if (opt.force) {  // turn this on to force a write of the block to the disc
            writeBlockToBinary(gold, fileName);
            LOG2("writeBlockToBinary(" + uint_2_Str(gold.blockNumber) + ", " + fileName + ": " + bool_2_Str(fileExists(fileName)));
        }

        result = gold.Format(opt.format);
    }

#ifndef PROVING
#else
    if (expContext().proving) {
        if (!expContext().proof.str().empty())
            expContext().proof << ",";
        expContext().proof << substitute(substitute(result, " ", ""), "\n", "");
        result = "";
    }
#endif
    return result;
}

//------------------------------------------------------------
string_q checkOneBlock(uint64_t num, const COptions& opt) {

    if (opt.quiet == 2) {
        cout << "Checking block " << cYellow << uint_2_Str(num) << cOff << "...       \r";
        cout.flush();
    }
    string_q numStr = uint_2_Str(num);

    // Get the block raw from the node...
    string_q fromNode;
    queryRawBlock(fromNode, numStr, true, false);
    replace(fromNode, "\"hash\":", "\"blockHash\":");
    if (verbose)
        cout << num << "\n";
    fromNode = normalizeBlock(fromNode, true, num >= byzantiumBlock);

    // Now get the same block from QBlocks
    string_q fromQblocks;
    CBlock qBlocks;
    queryBlock(qBlocks, numStr, true);
    for (size_t i = 0 ; i < qBlocks.transactions.size() ; i++) {
        // QBlocks pulls the receipt for each transaction, but the RPC does
        // not. Therefore, we must set the transactions' gasUsed and logsBloom
        // to be the same as the block's (even though they are not) so they
        // are removed as duplicates. Otherwise, the blocks won't match
        qBlocks.transactions.at(i).receipt.gasUsed = qBlocks.gasUsed;  // .at cannot go past end of vector!
        // qBlocks.transactions[i].receipt.logsBloom = qBlocks.logsBloom;
    }
    if (verbose)
        cout << num << "\n";
    fromQblocks = normalizeBlock(qBlocks.Format(), true, num >= byzantiumBlock);

extern string_q hiddenFields(void);
    string_q result = hiddenFields() + "The strings are ";
             result += ((fromNode != fromQblocks) ? "different\n" : "the same\n");
    string_q diffA  = "In fromNode but not fromQblocks:\n" + diffStr(fromNode, fromQblocks);
    string_q diffB  = "In fromQblocks but not fromNode:\n" + diffStr(fromQblocks, fromNode);

    // return the results
    string_q head = "\n" + string_q(80, '-') + "\n";
    if (opt.quiet == 2) {
        // only report results if we're being very quiet
        if (fromNode != fromQblocks)
            return "Difference at block " + cYellow + uint_2_Str(num) + cOff + ".\n" +
            "\t" + diffStr(fromNode, fromQblocks) + "\t" + diffStr(fromQblocks, fromNode);
        cout << "Checking block " + cYellow + uint_2_Str(num) + cOff + "...";
        cout << greenCheck << "         \r";
        cout.flush();
        return "";
    }

    return head + "from Node:\n" + fromNode +
            head + "from QBlocks:\n" + fromQblocks +
            head + result +
            head + diffA +
            head + diffB;
}

//------------------------------------------------------------
void interumReport(ostream& os, blknum_t i) {
    os << (!(i%150) ? "." : (!(i%1000)) ? "+" : "");  // dots '.' at every 150, '+' at every 1000
    os.flush();
}

//------------------------------------------------------------
bool sortByBlocknumTxId(const CAppearance& v1, const CAppearance& v2) {
    if (v1.bn != v2.bn)
        return v1.bn < v2.bn;
    else if (v1.tx != v2.tx)
        return v1.tx < v2.tx;
    else if (v1.tc != v2.tc)
        return v1.tc < v2.tc;
    else if (v1.reason != v2.reason)
        return v1.reason < v2.reason;
    return v1.addr < v2.addr;
}

extern bool visitAddrs(const CAppearance& item, void *data);
extern bool transFilter(const CTransaction *trans, void *data);

//------------------------------------------------------------
bool passesFilter(const COptions *opts, const CAppearance& item) {
    if (item.tc == 10 && !opts->showZeroTrace)
        return false;
    if (opts->filters.size() == 0)
        return true;
    for (auto elem : opts->filters)
        if (elem % item.addr)
            return true;
    return false;
}

//------------------------------------------------------------
string_q getAddresses(uint64_t num, const COptions& opt) {

    CBlock block;
    getBlock(block, num);
    if (opt.filterType == "uniq")
        block.forEveryUniqueAddress(visitAddrs, transFilter, (void*)&opt);
    else if (opt.filterType == "uniqTx")
        block.forEveryUniqueAddressPerTx(visitAddrs, transFilter, (void*)&opt);
    else
        block.forEveryAddress(visitAddrs, transFilter, (void*)&opt);
    if (opt.counting) {
        uint64_t cnt = opt.addrCnt;
        string_q be  = (cnt == 1 ? "was " : "were ");
        string_q adj = (contains(opt.filterType, "uniq") ? " unique" : "");
        cout << "There " << be << opt.addrCnt << adj << " addreses" << (cnt == 1 ? "" : "es") << " found in block " << block.blockNumber << ".\n";
        ((COptions*)&opt)->addrCnt = 0;
    }
    return "";
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void *data) {
    if (!isZeroAddr(item.addr)) {
        COptions *opt = (COptions*)data;
        if (passesFilter(opt, item)) {
            if (opt->counting) {
                opt->addrCnt++;

            } else {
                cout << item.Format(opt->format);
                if (!isTestMode())
                    cout << "                   ";
                cout << "\n";
                cout.flush();
            }
        }
    }
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//---------------------------------------------------------------------------
bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance) {
    // If the distance from the front of the node's current view of the front of the chain
    // is more than the number of seconds provided, consider the block final (even if it isn't
    // in a perfectly mathematical sense)
    return ((ts_chain - ts_block) > distance);
}
