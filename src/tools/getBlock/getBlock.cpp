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

    SFString checkResults;
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
        if (!options.quiet)
            cout << (options.isMulti() ? "[" : "");

        SFString list = options.getBlockNumList();
        while (!list.empty() && !shouldQuit()) {
            blknum_t bn = toLongU(nextTokenClear(list, '|'));
            if (options.isCheck) {
                checkResults += checkOneBlock(bn, options);
            } else {
                SFString result = doOneBlock(bn, options);
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

        if (!options.quiet)
            cout << (options.isMulti() ? "]" : "");

        if (options.isCheck) {
            cout << checkResults << (options.quiet > 1 ? "" : "\n");
            cout.flush();
        }
    }

    return 0;
}

//------------------------------------------------------------
SFString doOneBlock(uint64_t num, const COptions& opt) {

    CBlock latest;
    getBlock(latest, "latest");

    CBlock gold;
    gold.blockNumber = num;
    SFString result;
    SFString numStr = asStringU(num);
    if (opt.isRaw) {

        if (!queryRawBlock(result, numStr, true, opt.hashes)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        } else {
            if (opt.force) { // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson(cleanUpJson((char*)(const char*)result));
                result = generic.result;
                gold.parseJson((char*)(const char*)result);
                SFString fileName = getBinaryFilename(num);
                gold.finalized = isBlockFinal(gold.timestamp, latest.timestamp);
                writeBlockToBinary(gold, fileName);
            }
        }

    } else {
        SFString fileName = getBinaryFilename(gold.blockNumber);
        if (opt.isCache) {

            // --source::cache mode doesn't include timestamp in transactions
            readBlockFromBinary(gold, fileName);
            for (uint32_t t = 0 ; t < gold.transactions.getCount() ; t++)
                gold.transactions[t].timestamp = gold.timestamp;

        } else {
            queryBlock(gold, numStr, true, false);
        }

        if (opt.force) { // turn this on to force a write of the block to the disc
            gold.finalized = isBlockFinal(gold.timestamp, latest.timestamp);
            writeBlockToBinary(gold, fileName);
        }

        if (!opt.silent) {
            SFString format = opt.format;
//            if (false) { //opt.priceBlocks) {
//                SFUintBN oneWei = canonicalWei("1000000000000000000");
//                SFString dollars = "$" + asDollars(gold.timestamp, oneWei);
//                format.Replace("{PRICE:CLOSE}", dollars);
//            }
            result = gold.Format(format);
        }
    }
    return result;
}

//------------------------------------------------------------
SFString checkOneBlock(uint64_t num, const COptions& opt) {

    if (opt.quiet == 2) {
        cout << "Checking block " << cYellow << asStringU(num) << cOff << "...       \r";
        cout.flush();
    }
    SFString numStr = asStringU(num);

    // Get the block raw from the node...
    SFString fromNode;
    queryRawBlock(fromNode, numStr, true, false);
    fromNode.Replace("\"hash\":","\"blockHash\":");
    if (verbose)
        cout << num << "\n";
    fromNode = normalizeBlock(fromNode, true, num >= byzantiumBlock);

    // Now get the same block from quickBlocks
    SFString fromQblocks;
    CBlock qBlocks;
    queryBlock(qBlocks, numStr, true, false);
    for (uint32_t i = 0 ; i < qBlocks.transactions.getCount() ; i++) {
        // quickBlocks pulls the receipt for each transaction, but the RPC does
        // not. Therefore, we must set the transactions' gasUsed and logsBloom
        // to be the same as the block's (even though they are not) so they
        // are removed as duplicates. Otherwise, the blocks won't match
        qBlocks.transactions[i].receipt.gasUsed = qBlocks.gasUsed;
        //qBlocks.transactions[i].receipt.logsBloom = qBlocks.logsBloom;
    }
    if (verbose)
        cout << num << "\n";
    fromQblocks = normalizeBlock(qBlocks.Format(), true, num >= byzantiumBlock);

extern SFString hiddenFields(void);
    SFString result = hiddenFields() + "The strings are "; result += ((fromNode != fromQblocks) ? "different\n" : "the same\n");
    SFString diffA  = "In fromNode but not fromQblocks:\n" + diffStr(fromNode, fromQblocks);
    SFString diffB  = "In fromQblocks but not fromNode:\n" + diffStr(fromQblocks, fromNode);

    // return the results
    SFString head = "\n" + SFString('-',80) + "\n";
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
