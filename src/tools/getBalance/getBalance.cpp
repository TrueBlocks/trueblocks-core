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
    // Tell the system where the blocks are and which version to use
    etherlib_init("fastest");

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

        // For each address
        while (!options.addrs.empty()) {
            SFAddress addr = nextTokenClear(options.addrs, '|');

            // For each block
            SFString blocks = options.blocks;
            while (!blocks.empty()) {

                blknum_t blockNum = toLongU(nextTokenClear(blocks, '|'));
                if (blockNum > latestBlock) {
                    SFString late = (isTestMode() ? "--" : asStringU(latestBlock));
                    return usage("Block " + asStringU(blockNum) + " is later than the last valid block " + late + ". Quitting...");
                }

                SFUintBN bal = getBalance(addr, blockNum, false);
                SFString sBal = to_string(bal).c_str();
                if (expContext().asEther) {
                    sBal = wei2Ether(to_string(bal).c_str());
                } else if (expContext().asDollars) {
                    CBlock blk;
                    getBlock(blk, blockNum);
                    sBal = asDollars(blk.timestamp, bal);
                    if (sBal.empty())
                        sBal = "$0.00";
                }

                needsNewline = true;
                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << blockNum << "\t" << addr << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << addr << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else {
                    if (options.asData) {
                        cerr << blockNum << "\t" << addr << "         \r";
                    } else {
                        cerr << "    Balance for account " << cGreen << addr << cOff;
                        cerr << " at block " << cTeal << blockNum << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }
    if (needsNewline)
        cerr << "                                                                                              \n";
    return 0;
}
