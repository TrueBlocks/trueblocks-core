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
#include "etherlib.h"
#include "options.h"

//------------------------------------------------------------
int main(int argc, const char * argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
        if (!options.quiet && !options.addrs)
            cout << (options.isMulti() ? "[" : "");

        string_q list = options.getBlockNumList();
        while (!list.empty() && !shouldQuit()) {
            blknum_t bn = toLongU(nextTokenClear(list, '|'));
            if (options.addrs) {
                string_q checkResults = getAddresses(bn, options);
                cout << checkResults;
                cout.flush();

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
                    cout << result;
                    if (!list.empty())
                        cout << ",";
                    cout << "\n";

                } else {
                    interumReport(cerr, bn);
                }
            }
        }

        if (!options.quiet && !options.addrs)
            cout << (options.isMulti() ? "]" : "");
    }

    return 0;
}

//------------------------------------------------------------
string_q doOneBlock(uint64_t num, const COptions& opt) {

    CBlock latest;
    getBlock(latest, "latest");

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    string_q numStr = asStringU(num);
    if (opt.isRaw) {

        if (!queryRawBlock(result, numStr, true, opt.hashes)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        } else {
            if (opt.force) {  // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson(cleanUpJson((char*)result.c_str()));  // NOLINT
                result = generic.result;
                gold.parseJson((char*)result.c_str());  // NOLINT
                string_q fileName = getBinaryFilename(num);
                gold.finalized = isBlockFinal(gold.timestamp, latest.timestamp);
                writeBlockToBinary(gold, fileName);
            }
        }

    } else {
        string_q fileName = getBinaryFilename(gold.blockNumber);
        if (opt.isCache) {

            // --source::cache mode doesn't include timestamp in transactions
            readBlockFromBinary(gold, fileName);
            for (size_t t = 0 ; t < gold.transactions.size() ; t++)
                gold.transactions.at(t).timestamp = gold.timestamp;  // .at cannot go past end of vector!

        } else {
            queryBlock(gold, numStr, true, false);
        }

        if (opt.force) {  // turn this on to force a write of the block to the disc
            gold.finalized = isBlockFinal(gold.timestamp, latest.timestamp);
            writeBlockToBinary(gold, fileName);
        }

        if (!opt.silent) {
            string_q format = opt.format;
//            if (false) { //opt.priceBlocks) {
//                SFUintBN oneWei = canonicalWei("1000000000000000000");
//                string_q dollars = "$" + asDollars(gold.timestamp, oneWei);
//                replace(format, "{PRICE:CLOSE}", dollars);
//            }
            result = gold.Format(format);
            if (opt.hashes) {
                result = substitute(result, "        {\n            \"hash\":", "       ");
                result = substitute(result, "\n            \"receipt\": {\n            }\n        }", "");
                result = substitute(result, ",,", ",");
                result = substitute(result, ",\n    ]", "\n    ]");
            }
        }
    }
    return result;
}

//------------------------------------------------------------
string_q checkOneBlock(uint64_t num, const COptions& opt) {

    if (opt.quiet == 2) {
        cout << "Checking block " << cYellow << asStringU(num) << cOff << "...       \r";
        cout.flush();
    }
    string_q numStr = asStringU(num);

    // Get the block raw from the node...
    string_q fromNode;
    queryRawBlock(fromNode, numStr, true, false);
    replace(fromNode, "\"hash\":", "\"blockHash\":");
    if (verbose)
        cout << num << "\n";
    fromNode = normalizeBlock(fromNode, true, num >= byzantiumBlock);

    // Now get the same block from quickBlocks
    string_q fromQblocks;
    CBlock qBlocks;
    queryBlock(qBlocks, numStr, true, false);
    for (size_t i = 0 ; i < qBlocks.transactions.size() ; i++) {
        // quickBlocks pulls the receipt for each transaction, but the RPC does
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
            return "Difference at block " + cYellow + asStringU(num) + cOff + ".\n" +
            "\t" + diffStr(fromNode, fromQblocks) + "\t" + diffStr(fromQblocks, fromNode);
        cout << "Checking block " + cYellow + asStringU(num) + cOff + "...";
        cout << greenCheck << "         \r";
        cout.flush();
        return "";
    }

    return head + "from Node:\n" + fromNode +
            head + "from Quickblocks:\n" + fromQblocks +
            head + result +
            head + diffA +
            head + diffB;
}

//------------------------------------------------------------
void interumReport(ostream& os, blknum_t i) {
    os << (!(i%150) ? "." : (!(i%1000)) ? "+" : "");  // dots '.' at every 150, '+' at every 1000
    os.flush();
}

extern bool visitAddrs(const CAddressItem& item, void *data);
extern bool transFilter(const CTransaction *trans, void *data);
//------------------------------------------------------------
string_q getAddresses(uint64_t num, const COptions& opt) {

    SFAddressArray array;
    CBlock block;
    getBlock(block, num);
    block.forEveryUniqueAddress(visitAddrs, transFilter, &array);
    ostringstream os;
    for (auto elem : array)
        os << num << "\t" << elem << "\n";
    return os.str().c_str();
}

//----------------------------------------------------------------
bool visitAddrs(const CAddressItem& item, void *data) {
    SFAddressArray *array = (SFAddressArray*)data;
    array->push_back(item.addr);
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    // TODO: Use an option here for deep trace
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}
