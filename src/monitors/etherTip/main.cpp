/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <signal.h>
#include "tokenlib.h"
#include "processing.h"

// EXISTING_CODE
// EXISTING_CODE

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(myQuitHandler);
    //parselib_init();
    tokenlib_init();

    if (argc < 2)
        verbose = true;
    cerr << "Starting monitor...\r";
    cerr.flush();

    blknum_t clientHeight;
    uint64_t cacheHeight;
    getLatestBlocks(cacheHeight, clientHeight);

    // Parse command line, allowing for command files
    COptions visitor;
    if (!visitor.prepareArguments(argc, argv)) {
        etherlib_cleanup();
        return 0;
    }

    // while (!visitor.commandList.empty())
    {
        string_q command = nextTokenClear(visitor.commandList, '\n');
        if (!visitor.parseArguments(command)) {
            etherlib_cleanup();
            return 0;
        }

        uint64_t blockNum = visitor.blockStats.minWatchBlock-1;
        if (visitor.kBlock > blockNum)
            blockNum = visitor.kBlock;

        // Figure out which block to start on. Use earliest block from the watches. Note that
        // 'displayFromCache' may modify this to lastest visited block
        cout << "\r\n";
        if (visitor.kBlock) {
            // we're not starting at the beginning
            for (uint32_t i = 0 ; i < visitor.watches.size() ; i++) {
                visitor.watches.at(i).qbis.endBal = getNodeBal(visitor.watches.at(i).balanceHistory, visitor.watches.at(i).address, blockNum);
            }
        }

        visitor.displayFromCache(blockNum);

        time_q now = Now();
        cerr << argv[0] << ": " << now.Format(FMT_JSON) << ": "
        << "{ "
        << cYellow << visitor.transStats.nDisplayed    << cOff << " displayed from cache; "
        << cYellow << visitor.transStats.nAccountedFor << cOff << " accounted for"
        << " }                        \r\n";
    }

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
void myQuitHandler(int s) {
    quickQuitHandler(-1);
    remove("./cache/debug");
    exit(1);
}

// EXISTING_CODE
// EXISTING_CODE
