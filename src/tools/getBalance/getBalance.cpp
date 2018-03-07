/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool needsNewline = true;
    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (verbose)
            cout << "running command: " << command << "\n";

        if (!options.parseArguments(command))
            return 0;

        blknum_t latestBlock = getLatestBlockFromClient();
        SFUintBN totalVal = 0;
        uint64_t nAccts = countOf('|', options.addrs) + 1;
        bool needsTotal = (nAccts > 1 && options.total);

        // For each address
        while (!options.addrs.empty()) {
            SFAddress addr = nextTokenClear(options.addrs, '|');
            SFUintBN lastBal(0);

            // For each block
            SFString blocks = options.getBlockNumList();
            while (!blocks.empty()) {
                blknum_t blockNum = toLongU(nextTokenClear(blocks, '|'));
                if (blockNum > latestBlock) {
                    SFString late = (isTestMode() ? "--" : asStringU(latestBlock));
                    return usage("Block " + asStringU(blockNum) + " is later than the last valid block " + late + ". Quitting...");
                }

                SFUintBN bal = getBalance(addr, blockNum, false);
                totalVal += bal;
                SFString sBal = to_string(bal).c_str();
                if (expContext().asEther) {
                    sBal = wei2Ether(to_string(bal).c_str());
                } else if (expContext().asDollars) {
                    CBlock blk;
                    getBlock(blk, blockNum);
                    sBal = padLeft("$" + dispDollars(blk.timestamp, bal),14);
                }

                needsNewline = true;
                bool show = true;
                if (options.changes) {
                    if (bal == lastBal)
                        show = false;
                    lastBal = bal;
                }

                if (show && (bal > 0 || !options.noZero)) {

                    if (options.asData) {
                        cout << blockNum << "\t" << addr << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << addr << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                    needsNewline = false;

                } else if (!isTestMode()) {
                    if (options.asData) {
                        cerr << blockNum << "\t" << addr << "         \r";
                    } else {
                        cerr << "    Balance for account " << cGreen << addr << cOff;
                        cerr << " at block " << cTeal << blockNum << cOff;
                        cerr << " is " << cYellow << sBal << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }

        if (needsTotal) {
            SFString sBal = to_string(totalVal).c_str();
            if (expContext().asEther) {
                sBal = wei2Ether(to_string(totalVal).c_str());
            } else if (expContext().asDollars) {
                CBlock blk;
                getBlock(blk, getLatestBlockFromClient());
                sBal = padLeft("$" + dispDollars(blk.timestamp, totalVal),14);
            }
            cout << "        Total for " << cGreen << nAccts << cOff;
            cout << " accounts at " << cTeal << "latest" << cOff << " block";
            cout << " is " << cYellow << sBal.Substitute("  "," ") << cOff << "\n";
        }
    }

    if (needsNewline)
        cerr << "                                                                                                                 \n";
    return 0;
}
