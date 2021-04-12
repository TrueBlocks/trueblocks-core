/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool traces_Pre(const CTraverser* trav, void* data);
extern bool traces_Display(const CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_traces(void) {
    CTraverser trav(this, cout, "traces");
    trav.preFunc = traces_Pre;
    trav.filterFunc = rangeFilter;
    trav.displayFunc = traces_Display;

    CTraverserArray traversers;
    traversers.push_back(trav);

    cache_txs = false;
    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool traces_Display(const CTraverser* trav, void* data) {
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

    loadTraces(trans, trav->app->blk, trav->app->txid, opt->cache_traces,
               (opt->skip_ddos && excludeTrace(&trans, opt->max_traces)));

    for (auto trace : trans.traces) {
        opt->nProcessed++;
        bool isSuicide = trace.action.selfDestructed != "";
        bool isCreation = trace.result.newContract != "";

        if (!isSuicide) {
            if (!isTestMode() && isApiMode()) {
                qblocks::eLogger->setEndline('\r');
                LOG_INFO("\t\t\t\t\t\tGetting trace ", trans.blockNumber, ".", trans.transactionIndex, "-",
                         trace.getValueByName("traceAddress"), string_q(50, ' '));
                qblocks::eLogger->setEndline('\n');
            }
            opt->markNeighbors(trans);
            if (opt->articulate)
                opt->abi_spec.articulateTrace(&trace);
            if (!opt->freshen && !opt->factory) {
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << trace.Format() << endl;
                opt->firstOut = false;
            }
        }

        if (isSuicide) {  // suicide
            CTrace copy = trace;
            copy.action.from = trace.action.selfDestructed;
            copy.action.to = trace.action.refundAddress;
            copy.action.callType = "suicide";
            copy.action.value = trace.action.balance;
            copy.traceAddress.push_back("s");
            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
            copy.action.input = "0x";
            if (!opt->freshen && !opt->factory) {
                opt->markNeighbors(trans);
                if (opt->articulate)
                    opt->abi_spec.articulateTrace(&trace);
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy.Format() << endl;
                opt->firstOut = false;
            }
        }

        if (isCreation) {  // contract creation
            CTrace copy = trace;
            copy.action.from = "0x0";
            copy.action.to = trace.result.newContract;
            copy.action.callType = "creation";
            copy.action.value = trace.action.value;
            if (copy.traceAddress.size() == 0)
                copy.traceAddress.push_back("null");
            copy.traceAddress.push_back("s");
            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
            copy.action.input = trace.action.input;
            if (!opt->freshen && !opt->factory) {
                opt->markNeighbors(trans);
                if (opt->articulate)
                    opt->abi_spec.articulateTrace(&trace);
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy.Format() << endl;
                opt->firstOut = false;
            } else if (opt->factory) {
                opt->markNeighbors(trans);
                if (opt->articulate)
                    opt->abi_spec.articulateTrace(&trace);
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy.Format() << endl;
                opt->firstOut = false;
            }
        }
    }

    prog_Log(trav, data, inCache ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool traces_Pre(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    start_Log(trav, data);
    return true;
}
