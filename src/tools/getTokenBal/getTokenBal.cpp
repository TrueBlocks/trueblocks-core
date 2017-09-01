/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#define NOWALLETLIB
#include "tokenlib.h"
#include "options.h"

using namespace qblocks;

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    etherlib_init("parity");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        while (!options.holders.empty()) {
            SFAddress addr = nextTokenClear(options.holders, '|');
            SFString blocks = options.blocks;
            while (!blocks.empty()) {

                blknum_t block = toLongU(nextTokenClear(blocks, '|'));

                SFUintBN bal = getTokenBalance(options.token, addr, block);
                SFString sBal = to_string(bal).c_str();
                if (options.asEther)
                    sBal = wei2Ether(to_string(bal).c_str());
                if (options.asData)
                    cout << block << "\t" << options.token << "\t" << addr << "\t" << sBal << "\n";
                else
                    cout << "\n\tBalance of address " << bBlue << addr << cOff
                            << " at block " << bBlue << block << cOff
                            << ": " << bYellow << sBal << cOff << "\n\n";
                cout.flush();
            }
        }
    }
    return 0;
}
