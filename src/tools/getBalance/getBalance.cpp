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
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("parity");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        while (!options.addrs.empty()) {
            SFAddress addr = nextTokenClear(options.addrs, '|');
            SFString blocks = options.blocks;
            while (!blocks.empty()) {
                blknum_t block = toLongU(nextTokenClear(blocks, '|'));
                SFUintBN bal = getBalance(addr, block, false); //isDemo);
                if (options.asData)
                    cout << block << "\t" << addr << "\t" << bal << "\n";
                else
                    cout << "Balance of address " << addr << " at block " << block << ": " << bal << "\n";
                cout.flush();
            }
        }
    }
    return 0;
}
