/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool listEmptyBlocks(blknum_t bn, void *data);
extern bool listFullBlocks(blknum_t bn, void *data);
//----------------------------------------------------------------------------------
void handle_list(COptions& options) {

    CStatistics stats(&options);
    if (options.incEmpty) {
        forEveryEmptyBlockByNumber(listEmptyBlocks, &stats, options.startBlock, options.nBlocks, options.skip);
    }

    if (options.incFull) {
        forEveryNonEmptyBlockByNumber(listFullBlocks, &stats, options.startBlock, options.nBlocks, options.skip);
    }

    if (options.modes & OP_STATS) {
        cout << "  full blocks:\t"  << cYellow << stats.fullCnt;
            cout << " (" << double_2_Str(100. * (double)stats.fullCnt/double(stats.visited),1) << "%)" << cOff << "\n";
        cout << "  empty blocks:\t" << cYellow << stats.emptyCnt;
            cout << " (" << double_2_Str(100. * (double)stats.emptyCnt/double(stats.visited),1) << "%)" << cOff << "\n";
        cout << "  total blocks:\t" << cYellow << stats.visited  << cOff << "\n";
    }
}

//----------------------------------------------------------------------------------
bool listFullBlocks(blknum_t bn, void *data) {
    CStatistics *stats = (CStatistics*)data;
    stats->visited++;
    stats->fullCnt++;
    if (stats->opt->modes & OP_LIST)
        cout << bn << "\tf\n";
    return true;
}

//----------------------------------------------------------------------------------
bool listEmptyBlocks(blknum_t bn, void *data) {
    CStatistics *stats = (CStatistics*)data;
    stats->visited++;
    stats->emptyCnt++;
    if (stats->opt->modes & OP_LIST)
        cout << bn << "\te\n";
    return true;
}
