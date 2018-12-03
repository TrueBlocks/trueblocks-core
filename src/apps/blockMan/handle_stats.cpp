/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool buildStatistics(blknum_t bn, void *data);
//----------------------------------------------------------------------------------
void handle_stats(COptions& options) {

    uint64_t highest = 0;
    blknum_t latest = (isTestMode() ? 4000000 : getLatestBlockFromClient());
    forEveryNonEmptyBlockByNumber(buildStatistics, &highest, options.startBlock, latest);

    uint64_t nItems = (isTestMode() ? 3000000 : fileSize(fullBlockIndex) / sizeof(uint64_t));
    cout << cGreen << "fullBlocks stats:\n" << cOff;
    cout << "\tFull blocks:\t"  << cYellow <<  nItems          << cOff << "\n";
    cout << "\tEmpty blocks:\t" << cYellow << (highest-nItems) << cOff << "\n";
    cout << "\tTotal blocks:\t" << cYellow <<  highest         << cOff << "\n";
}

//----------------------------------------------------------------------------------
bool buildStatistics(blknum_t bn, void *data) {
    uint64_t *counter = (uint64_t*)data;
    *counter = bn;
    if (verbose)
        cout << "\t" << bn << "\n";
    return true;
}
