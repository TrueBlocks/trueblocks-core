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

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (verbose)
            cout << "running command: " << command << "\n";

        if (!options.parseArguments(command))
            return 0;

        // For each address
        while (!options.addrs.empty()) {
            SFAddress addr = nextTokenClear(options.addrs, '|');

            // For each block
            SFString blocks = options.blocks;
            while (!blocks.empty()) {

                blknum_t block = toLongU(nextTokenClear(blocks, '|'));
                SFUintBN bal = getBalance(addr, block, false);
                SFString sBal = to_string(bal).c_str();
                if (expContext().asEther)
                    sBal = wei2Ether(to_string(bal).c_str());

                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << block << "\t" << addr << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << addr << cOff;
                        cout << " at block " << cTeal << block << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                } else {
                    if (options.asData) {
                        cerr << block << "\t" << addr << "         \r";
                    } else {
                        cerr << "    Balance for account " << cGreen << addr << cOff;
                        cerr << " at block " << cTeal << block << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }
    return 0;
}
