/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    pinlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        // clang-format off
        options.className =
            (options.count ? GETRUNTIME_CLASS(CMonitorCount)->m_ClassName
             : options.appearances
                 ? GETRUNTIME_CLASS(CAppearance)->m_ClassName
                 : (options.traces
                        ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                        : (options.receipts
                               ? GETRUNTIME_CLASS(CReceipt)->m_ClassName
                               : (options.logs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                                               : GETRUNTIME_CLASS(CTransaction)->m_ClassName))));
        // clang-format on

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], options.className);

        CTraverserArray traversers;
        if (options.appearances) {
            CAppearanceTraverser at;
            traversers.push_back(at);
        }

        if (options.receipts) {
            CReceiptTraverser rt;
            traversers.push_back(rt);
        }

        if (options.logs) {
            CLogTraverser lt;
            traversers.push_back(lt);
        }

        if (options.traces) {
            CTraceTraverser tt;
            traversers.push_back(tt);
        }

        if (traversers.empty()) {
            CTransactionTraverser tt;
            if (options.freshenOnly) {
                tt.filterFunc = noopFunc;
                tt.preFunc = noopFunc;
                tt.displayFunc = noopFunc;
                tt.dataFunc = noopFunc;
            }
            traversers.push_back(tt);
        }

        forEveryAppearance(traversers, options.apps, &options);
        if (shouldQuit())
            break;

        once = false;
    }

    ostringstream os;
    os << ", \"start\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.scanRange.first)) << endl;
    os << ", \"end\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.scanRange.second)) << endl;
    if (!options.count && options.allMonitors.size() == 1) {
        options.getNamedAccount(options.allMonitors[0], options.accountedFor);
        if (options.abi_spec.nInterfaces() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.allMonitors[0] << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    options.writePerformanceData();

    pinlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool pre_Func(CTraverser* trav, void* data) {
    start_Log(trav, data);
    return true;
}

//-----------------------------------------------------------------------
bool post_Func(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    // TODO(tjayrush): We used to call writeLastEncountered (since removed) that would
    // TODO(tjayrush): keep track of the last encountered block to avoid starting
    // TODO(tjayrush): each freshen cycle at the previous stored block since we
    // TODO(tjayrush): already processed encountered blocks.
    opt->reportNeighbors();

    end_Log(trav, data);
    return true;
}

//-----------------------------------------------------------------------
void start_Log(CTraverser* trav, void* data) {
    if (!trav->logging)
        return;
    return;
}

//-----------------------------------------------------------------------
void prog_Log(CTraverser* trav, void* data) {
    const COptions* opt = (const COptions*)data;
    if (!trav->logging)
        return;

#define REPORT_FREQ 5
    if (trav->nProcessed % REPORT_FREQ)
        return;

    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t goal = opt->stats.nFileRecords;

    ostringstream post;
    post << " " << trav->operation << " (max " << goal << ") for address " << opt->accountedFor;
    LOG_PROGRESS(trav->readStatus, prog, goal, post.str() + "\r");

    return;
}

//-----------------------------------------------------------------------
void end_Log(CTraverser* trav, void* data) {
    const COptions* opt = (const COptions*)data;
    if (!trav->logging)
        return;

    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t goal = opt->stats.nFileRecords;
    if (prog == goal) {
        string_q msg = opt->freshenOnly ? "Finished updating" : "Finished reporting on";
        string_q endMsg = " " + trav->operation + " for address " + opt->accountedFor;
        LOG_PROGRESS(msg, prog, goal, endMsg);
    }
    return;
}

//-----------------------------------------------------------------------
bool loadTx_Func(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    trav->block = CBlock();
    trav->trans = CTransaction();

    trav->block.blockNumber = trav->app->blk;
    trav->trans.pBlock = &trav->block;

    bool dirty = false;
    string_q txFilename = getBinaryCacheFilename(CT_TXS, trav->app->blk, trav->app->txid);
    bool inCache = trav->app->blk != 0 && fileExists(txFilename);
    if (inCache) {
        readTransFromBinary(trav->trans, txFilename);
        trav->readStatus = "Reading";

    } else {
        dirty = true;
        if (trav->app->blk == 0) {
            address_t addr = opt->prefundAddrMap[trav->app->txid];
            trav->trans.loadTransAsPrefund(trav->app->blk, trav->app->txid, addr, expContext().prefundMap[addr]);

        } else if (trav->app->txid == 99997 || trav->app->txid == 99999) {
            trav->trans.loadTransAsBlockReward(trav->app->blk, trav->app->txid, opt->blkRewardMap[trav->app->blk]);

        } else if (trav->app->txid == 99998) {
            uint64_t nUncles = getUncleCount(trav->app->blk);
            for (size_t u = 0; u < nUncles; u++) {
                CBlock uncle;
                getUncle(uncle, trav->app->blk, u);
                if (uncle.miner == opt->blkRewardMap[trav->app->blk]) {
                    trav->trans.loadTransAsUncleReward(trav->app->blk, uncle.blockNumber, uncle.miner);
                }
            }

        } else {
            getTransaction(trav->trans, trav->app->blk, trav->app->txid);
            getFullReceipt(&trav->trans, true);
        }
    }

    trav->trans.pBlock = &trav->block;
    trav->trans.timestamp = trav->block.timestamp = (timestamp_t)expContext().tsMemMap[(trav->app->blk * 2) + 1];

    if (opt->traces && trav->trans.traces.size() == 0) {
        dirty = true;
        loadTraces(trav->trans, trav->app->blk, trav->app->txid, opt->cache_traces,
                   (opt->skip_ddos && excludeTrace(&trav->trans, opt->max_traces)));
    }

    dirty |= opt->articulateAll(trav->trans);

    // TODO(tjayrush): This could be in post_Func so that _Display can also make it dirty
    if (opt->cache_txs && dirty)
        writeTransToBinary(trav->trans, txFilename);

    opt->markNeighbors(trav->trans);

    return true;
}
