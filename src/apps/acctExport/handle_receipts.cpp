/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool receipts_Pre(const CTraverser* trav, void* data);
extern bool receipts_Display(const CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_receipts(void) {
    CTraverser trav(this, cout, "receipts");
    trav.preFunc = receipts_Pre;
    trav.filterFunc = rangeFilter;
    trav.displayFunc = receipts_Display;

    CTraverserArray traversers;
    traversers.push_back(trav);

    cache_txs = false;  // no need
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
    bool inCache = trav->app->blk != 0 && fileExists(txFilename);
    if (inCache) {
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

        // TODO: Must we write this data if the data has not changed?
        if (opt->cache_txs)
            writeTransToBinary(trans, txFilename);
    }

    opt->nProcessed++;
    if (!opt->freshen) {
        opt->markNeighbors(trans);
        opt->articulateAll(trans);
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trans.receipt.Format() << endl;
        opt->firstOut = false;
    }

    prog_Log(trav, data, inCache ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool receipts_Pre(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    SHOW_FIELD(CReceipt, "blockNumber");
    SHOW_FIELD(CReceipt, "transactionIndex");
    SHOW_FIELD(CReceipt, "isError");

    start_Log(trav, data);
    return true;
}
