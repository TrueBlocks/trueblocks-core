/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool forEveryAppearance(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data) {
    // If we have nothing to do, return success
    if (!apps.size() || !traversers.size())
        return true;

    // for each traverser...
    for (auto trav : traversers) {
        if (shouldQuit())
            break;

        // Make sure we have something to work with...
        trav.filterFunc = trav.filterFunc ? trav.filterFunc : rangeFilter;
        trav.preFunc = trav.preFunc ? trav.preFunc : noopFunc;
        trav.postFunc = trav.postFunc ? trav.postFunc : noopFunc;
        trav.dataFunc = trav.dataFunc ? trav.dataFunc : noopFunc;

        // Prepare the export...
        if (!(*trav.preFunc)(&trav, data))
            return false;

        // For each appearance...
        for (trav.index = 0; trav.index < apps.size() && !shouldQuit(); trav.index++) {
            trav.app = &apps[trav.index];
            bool passedFilter = !trav.filterFunc || (*trav.filterFunc)(&trav, data);
            if (passedFilter) {
                if (trav.displayFunc) {
                    COptions* opt = (COptions*)trav.options;  // makes code clearer
                    // We freshen at most 5,000 new transactions to stay responsive
                    if (opt->freshen && trav.nProcessed > 5000)
                        break;

                    if (!(*trav.dataFunc)(&trav, data))
                        return (*trav.postFunc)(&trav, data) && false;

                    if (!(*trav.displayFunc)(&trav, data))
                        return (*trav.postFunc)(&trav, data) && false;
                }
            }
        }

        // Cleanup the export...
        if (!(*trav.postFunc)(&trav, data))
            return false;
    }

    return true;
};

//-----------------------------------------------------------------------
bool rangeFilter(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->app->blk > opt->scanRange.second || trav->app->blk >= expContext().tsCnt || shouldQuit())
        return false;

    return inRange(blknum_t(trav->app->blk), opt->scanRange.first, opt->scanRange.second);
}

//-----------------------------------------------------------------------
bool defPostFunc(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->lastExpBlock != NOPOS)
        for (auto monitor : opt->allMonitors)
            monitor.writeLastExport(trav->lastExpBlock);

    opt->reportNeighbors();

    end_Log(trav, data);
    return true;
}

#define REPORT_FREQ 5
//-----------------------------------------------------------------------
void prog_Log(CTraverser* trav, void* data, TraverserLog mode) {
    ASSERT(mode == TR_PROGRESS_CACHE || mode == TR_PROGRESS_NODE);
    if (!trav->logging)
        return;

    const COptions* opt = trav->options;
    if (trav->nProcessed % REPORT_FREQ)
        return;

    // TODO: Use inCache to distinguish between Reading and Fetching
    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t goal = opt->nTransactions;
    ostringstream post;
    post << " " << trav->op << " (max " << goal << ") for address " << opt->hackAppAddr;
    LOG_PROGRESS((mode == TR_PROGRESS_CACHE ? "Reading" : "Extracting"), prog, goal, post.str() + "\r");
    return;
}

//-----------------------------------------------------------------------
void end_Log(CTraverser* trav, void* data) {
    if (!trav->logging)
        return;

    const COptions* opt = trav->options;
    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t goal = opt->nTransactions;
    if (prog == goal) {
        string_q msg = opt->freshen ? "Finished updating" : "Finished reporting on";
        string_q endMsg = " " + trav->op + " for address " + opt->hackAppAddr;
        LOG_PROGRESS(msg, prog, goal, endMsg);
    }
    return;
}

//-----------------------------------------------------------------------
void start_Log(CTraverser* trav, void* data) {
    if (!trav->logging)
        return;
    return;
}

//-----------------------------------------------------------------------
bool loadData(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->block1 = CBlock();
    trav->trans1 = CTransaction();

    trav->block1.blockNumber = trav->app->blk;
    trav->trans1.pBlock = &trav->block1;

    string_q txFilename = getBinaryCacheFilename(CT_TXS, trav->app->blk, trav->app->txid);
    trav->inCache1 = trav->app->blk != 0 && fileExists(txFilename);
    if (trav->inCache1) {
        // we read the data, if we find it, but....
        readTransFromBinary(trav->trans1, txFilename);
        trav->trans1.finishParse();
        trav->trans1.pBlock = &trav->block1;
        trav->block1.timestamp = trav->trans1.timestamp = (timestamp_t)expContext().tsMemMap[(trav->app->blk * 2) + 1];

    } else {
        if (trav->app->blk == 0) {
            address_t addr = opt->prefundAddrMap[trav->app->txid];
            trav->trans1.loadTransAsPrefund(trav->app->blk, trav->app->txid, addr, expContext().prefundMap[addr]);

        } else if (trav->app->txid == 99997 || trav->app->txid == 99999) {
            trav->trans1.loadTransAsBlockReward(trav->app->blk, trav->app->txid, opt->blkRewardMap[trav->app->blk]);

        } else if (trav->app->txid == 99998) {
            uint64_t nUncles = getUncleCount(trav->app->blk);
            for (size_t u = 0; u < nUncles; u++) {
                CBlock uncle;
                getUncle(uncle, trav->app->blk, u);
                if (uncle.miner == opt->blkRewardMap[trav->app->blk]) {
                    trav->trans1.loadTransAsUncleReward(trav->app->blk, uncle.blockNumber, uncle.miner);
                }
            }

        } else {
            getTransaction(trav->trans1, trav->app->blk, trav->app->txid);
            getFullReceipt(&trav->trans1, true);
        }

        trav->trans1.pBlock = &trav->block1;
        trav->trans1.timestamp = trav->block1.timestamp = (timestamp_t)expContext().tsMemMap[(trav->app->blk * 2) + 1];

        // TODO: Must we write this data if the data has not changed?
        if (opt->cache_txs)
            writeTransToBinary(trav->trans1, txFilename);
    }
    return true;
}
