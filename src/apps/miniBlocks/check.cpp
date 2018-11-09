/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
void checkMiniBlocks(const COptions& opts, CVisitor& reporter) {
    uint64_t save = verbose;
    verbose = true;
    reporter.deep = opts.deep;
    forEveryMiniBlockInMemory(checkMiniBlock, &reporter, reporter.firstBlock(), reporter.size());
    verbose = save;
    return;
}

//-----------------------------------------------------------------------------------------------
bool checkMiniBlock(CMiniBlock& miniBlock, const CMiniTrans *, void *data) {

    CVisitor *reporter = reinterpret_cast<CVisitor *>(data);
    string_q fn = getBinaryFilename(miniBlock.blockNumber);
    if (fileExists(fn)) {

        // tell user where we are
        cerr << "Checking block: " << miniBlock.blockNumber;
        cerr << " at " << ts_2_Date(miniBlock.timestamp) << " : ";
        if (reporter->deep) {
            CBlock block;
            getBlock(block, miniBlock.blockNumber);
            bool same = (miniBlock == block);
            cerr << (same ? greenCheck : redX) << " ";
            if (!same) {
                cerr << "Block " << miniBlock.blockNumber << " is different than miniBlock. Quitting...\n";
                cerr.flush();
                exit(0);
            }
        }
        cerr << "              \r";
        cerr.flush();

    } else {
        cerr << "Block file '" << fn << "' not found. Quitting...\n";
        cerr.flush();
        exit(0);
    }
    return true;
}
