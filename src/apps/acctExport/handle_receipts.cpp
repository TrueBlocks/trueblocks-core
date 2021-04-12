/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool receipts_Pre(const CTraverser* trav, void* data);
extern bool receipts_Display(const CTraverser* trav, void* data);
extern bool receipts_Post(const CTraverser* trav, void* data);
extern void receipts_Log(const CTraverser* trav, void* data, TraverserLog mode);
//-----------------------------------------------------------------------
bool COptions::handle_receipts(void) {
    CTraverser trav(this, cout, "receipts");
    trav.preFunc = receipts_Pre;
    trav.filterFunc = rangeFilter;
    trav.displayFunc = receipts_Display;
    trav.postFunc = receipts_Post;
    trav.logFunc = receipts_Log;

    CTraverserArray traversers;
    traversers.push_back(trav);

    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool receipts_Display(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    CBlock block;  // do not move this from this scope
    block.blockNumber = trav->app->blk;
    CTransaction trans;
    trans.pBlock = &block;

    string_q txFilename = getBinaryCacheFilename(CT_TXS, trav->app->blk, trav->app->txid);
    if (trav->app->blk != 0 && fileExists(txFilename)) {
        // we read the data, if we find it, but....
        readTransFromBinary(trans, txFilename);
        trans.finishParse();
        trans.pBlock = &block;
        block.timestamp = trans.timestamp = (timestamp_t)expContext().tsMemMap[(trav->app->blk * 2) + 1];

    } else {
        if (trav->app->blk == 0) {
            address_t addr = opt->prefundAddrMap[trav->app->txid];
            trans.loadTransAsPrefund(trav->app->blk, trav->app->txid, addr, expContext().prefundMap[addr]);

        } else if (trav->app->txid == 99997 || trav->app->txid == 99999) {
            trans.loadTransAsBlockReward(trav->app->blk, trav->app->txid, opt->blkRewardMap[trav->app->blk]);

        } else if (trav->app->txid == 99998) {
            uint64_t nUncles = getUncleCount(trav->app->blk);
            for (size_t u = 0; u < nUncles; u++) {
                CBlock uncle;
                getUncle(uncle, trav->app->blk, u);
                if (uncle.miner == opt->blkRewardMap[trav->app->blk]) {
                    trans.loadTransAsUncleReward(trav->app->blk, uncle.blockNumber, uncle.miner);
                }
            }

        } else {
            getTransaction(trans, trav->app->blk, trav->app->txid);
            getFullReceipt(&trans, true);
        }

        trans.pBlock = &block;
        trans.timestamp = block.timestamp = (timestamp_t)expContext().tsMemMap[(trav->app->blk * 2) + 1];

        // ... we don't write the data here since it will not be complete.
        // if (false) // (cache_txs && !fileExists(txFilename))
        //    writeTransToBinary(trans, txFilename);
    }

    if (opt->articulate)
        opt->abi_spec.articulateTransaction(&trans);

    if (!opt->freshen) {
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trans.receipt.Format() << endl;
    }

    if (!isTestMode() && (isApiMode() || !(opt->nProcessed % 3))) {
        receipts_Log(trav, nullptr, TR_PROGRESS);
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool receipts_Pre(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    SHOW_FIELD(CReceipt, "blockNumber");
    SHOW_FIELD(CReceipt, "transactionIndex");
    SHOW_FIELD(CReceipt, "isError");

    receipts_Log(trav, nullptr, TR_START);

    return true;
}

//-----------------------------------------------------------------------
bool receipts_Post(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->lastExpBlock != NOPOS)
        for (auto monitor : opt->allMonitors)
            monitor.writeLastExport(trav->lastExpBlock);
    opt->reportNeighbors();

    if (!isTestMode())
        receipts_Log(trav, data, TR_END);

    return true;
}

//-----------------------------------------------------------------------
void receipts_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (mode == TR_END) {
        end_Log(trav, data, mode);

    } else if (mode == TR_PROGRESS) {
        prog_Log(trav, data, mode);
    }
}
