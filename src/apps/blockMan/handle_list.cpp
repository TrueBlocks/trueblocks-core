/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool listEmptyBlocks(CBlockIndexItem& bi, void *data);
extern bool listNonEmptyBlocks(CBlockIndexItem& bi, void *data);
//----------------------------------------------------------------------------------
void handle_list(COptions& options) {

    CStatistics stats(&options);
    if (options.incEmpty) {
        if (!forEveryBlockIndexItem(F_EMPTY, listEmptyBlocks, &stats, options.startBlock, options.nBlocks, options.skip))
            return;
    }

    if (options.incFull) {
        if (!forEveryBlockIndexItem(F_FULL, listNonEmptyBlocks, &stats, options.startBlock, options.nBlocks, options.skip))
            return;
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
bool listNonEmptyBlocks(CBlockIndexItem& item, void *data) {
    CStatistics *stats = (CStatistics*)data;
    stats->visited++;
    stats->fullCnt++;
    if (stats->opt->modes & OP_LIST) {
        if (verbose)
            cout << item.bn << "\t" << item.ts << "\t" << item.cnt << endl;
        else
            cout << item.bn << "\tf\n";
    }
    return true;
}

//----------------------------------------------------------------------------------
bool listEmptyBlocks(CBlockIndexItem& item, void *data) {
    CStatistics *stats = (CStatistics*)data;
    stats->visited++;
    stats->emptyCnt++;
    if (stats->opt->modes & OP_LIST) {
        if (verbose)
            cout << item.bn << "\t" << item.ts << "\t" << item.cnt << endl;
        else
            cout << item.bn << "\te\n";
    }
    return true;
}
