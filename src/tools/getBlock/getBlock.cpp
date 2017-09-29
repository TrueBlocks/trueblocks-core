/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//------------------------------------------------------------
int main(int argc, const char * argv[]) {

    // Tell the system where the blocks are and which version to use
    etherlib_init("binary");

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
        if (options.blocks.isRange) {
            for (SFUint32 i = options.blocks.start ; i < options.blocks.stop ; i++) {
                if (options.isCheck) {
                    checkResults += checkOneBlock(i, options);

                } else {
                    SFString result = doOneBlock(i, options);
                    if (options.normalize) {
                        if (verbose)
                            cout << i << "\n";
                        result = normalizeBlock(result, false);
                    }
                    if (!options.quiet) {
                        cout << result;
                        if (i < options.blocks.stop-1)
                            cout << ",";
                        cout << "\n";

                    } else {
                        interumReport(cerr, i);
                    }
                }
            }
        } else {
            for (SFUint32 i = 0 ; i < options.blocks.nNums ; i++) {
                if (options.isCheck) {
                    checkResults += checkOneBlock(options.blocks.nums[i], options);

                } else {
                    SFString result = doOneBlock(options.blocks.nums[i], options);
                    if (options.normalize) {
                        if (verbose)
                            cout << options.blocks.nums[i] << "\n";
                        result = normalizeBlock(result, false);
                    }
                    if (!options.quiet) {
                        cout << result;
                        if (i < options.blocks.nNums - 1)
                            cout << ",";
                        cout << "\n";

                    } else {
                        interumReport(cerr, i);
                    }
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
SFString doOneBlock(SFUint32 num, const COptions& opt) {

    CBlock gold;
    SFString result;
    SFString numStr = asStringU(num);
    if (opt.isRaw) {

        if (!queryRawBlock(result, numStr, true, opt.terse)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        } else {
            if (opt.force) { // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson(cleanUpJson((char*)(const char*)result));
                result = generic.result;
                gold.parseJson((char*)(const char*)result);
                SFString fileName = getBinaryFilename1(num);
                writeToBinary(gold, fileName);
            }
        }

    } else {
        if (opt.asks4Cache) {
            CFilename fileName(getBinaryFilename1(num));
            if (!fileExists(fileName.getFullPath())) {
                cout << usageStr("You asked for a block (" + asStringU(num) + ") from the cache, but that block is not in the cache. Quitting.");
                exit(0);
            }
        }
        queryBlock(gold, numStr, true);
        if (opt.force) { // turn this on to force a write of the block to the disc
            SFString fileName = getBinaryFilename1(gold.blockNumber);
            writeToBinary(gold, fileName);
        }

        if (getSource().Contains("Only")) {
            // --source::cache mode doesn't include timestamp in transactions
            for (txnum_t t = 0 ; t < gold.transactions.getCount() ; t++) {
                gold.transactions[t].timestamp = (SFUint32)gold.timestamp;
            }

        }
        if (!opt.silent)
            result = gold.Format();
    }
    return result;
}

//------------------------------------------------------------
SFString checkOneBlock(SFUint32 num, const COptions& opt) {

    if (opt.quiet == 2) {
        cout << "Checking block " + cYellow + asStringU(num) + cOff + "...       \r";
        cout.flush();
    }
    SFString numStr = asStringU(num);

    // Get the block raw from the node...
    SFString fromNode;
    queryRawBlock(fromNode, numStr, true, false);
    fromNode.Replace("\"hash\":","\"blockHash\":");
    if (verbose)
        cout << num << "\n";
    fromNode = normalizeBlock(fromNode, true);

    // Now get the same block from quickBlocks
    SFString fromQblocks;
    CBlock qBlocks;
    queryBlock(qBlocks, numStr, true);
    for (uint32_t i = 0 ; i < qBlocks.transactions.getCount() ; i++) {
        // quickBlocks pulls the receipt for each transaction, but the RPC does
        // not. Therefore, we must set the transactions' gasUsed and logsBloom
        // to be the same as the block's (even though they are not) so they
        // are removed as duplicates. Otherwise, the blocks won't match
        qBlocks.transactions[i].receipt.gasUsed = qBlocks.gasUsed;
        qBlocks.transactions[i].receipt.logsBloom = qBlocks.logsBloom;
    }
    if (verbose)
        cout << num << "\n";
    fromQblocks = normalizeBlock(qBlocks.Format(), true);

    SFString result = "The strings are "; result += ((fromNode != fromQblocks) ? "different\n" : "the same\n");
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


