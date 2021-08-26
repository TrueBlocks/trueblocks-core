x
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

        CTraverserArray traversers;
        if (options.load.empty()) {
            // clang-format off
            options.className =
            (options.count ? GETRUNTIME_CLASS(CMonitorCount)->m_ClassName
             : options.appearances
             ? GETRUNTIME_CLASS(CAppearance)->m_ClassName
             : (options.traces
                ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                : (options.receipts
                ? GETRUNTIME_CLASS(CReceipt)->m_ClassName
                  : (options.statements
                  ? GETRUNTIME_CLASS(CReconciliation)->m_ClassName
                    : (options.logs
                    ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                      : GETRUNTIME_CLASS(CTransaction)->m_ClassName)))));
            // clang-format on

            once = once && !options.freshenOnly;
            if (once)
                cout << exportPreamble(expContext().fmtMap["header"], options.className);

            if (options.appearances) {
                CAppearanceTraverser at;
                at.exportRange = options.exportRange;
                traversers.push_back(at);
            }

            if (options.receipts) {
                CReceiptTraverser rt;
                rt.exportRange = options.exportRange;
                traversers.push_back(rt);
            }

            if (options.statements) {
                CStatementTraverser st;
                st.exportRange = options.exportRange;
                traversers.push_back(st);
            }

            if (options.logs) {
                CLogTraverser lt;
                lt.exportRange = options.exportRange;
                traversers.push_back(lt);
            }

            if (options.traces) {
                CTraceTraverser tt;
                tt.exportRange = options.exportRange;
                traversers.push_back(tt);
            }

            if (traversers.empty()) {
                CTransactionTraverser tt;
                tt.exportRange = options.exportRange;
                traversers.push_back(tt);
            }

            forEveryAppearance(traversers, options.monApps, &options);

        } else {
            options.handle_traversers();
        }

        if (shouldQuit())
            break;

        once = false;
    }

    ostringstream os;
    os << ", \"first_block\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.exportRange.first)) << endl;
    os << ", \"last_block\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.exportRange.second)) << endl;
    if (!options.count && options.allMonitors.size() == 1) {
        options.findName(options.accountedFor.address, options.allMonitors[0]);
        if (options.abi_spec.nInterfaces() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.allMonitors[0] << endl;
    }
    if (!isTestMode() && options.slowQueries) {
        os << ", \"slowQueries\": true" << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    options.writePerformanceData();

    pinlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool tsRangeFunc(CTraverser* trav, void* data) {
    if (!getTimestampAt(trav->app->blk) || shouldQuit())
        return false;
    return inRange(blknum_t(trav->app->blk), trav->exportRange.first, trav->exportRange.second);
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

    if (trav->nProcessed % opt->reportFreq())
        return;

    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t goal = opt->stats.nFileRecords;

    ostringstream post;
    post << " " << trav->operation << " (max " << goal << ") for address " << opt->accountedFor.address;
    LOG_PROGRESS(trav->readStatus, prog, goal, post.str() + "\r");

    return;
}

//-----------------------------------------------------------------------
void end_Log(CTraverser* trav, void* data) {
    const COptions* opt = (const COptions*)data;
    if (!trav->logging || opt->freshenOnly)
        return;

    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t goal = opt->stats.nFileRecords;
    if (prog == goal) {
        string_q endMsg = " " + trav->operation + " for address " + opt->accountedFor.address;
        LOG_PROGRESS("Completed", prog, goal, endMsg);
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
        trav->readStatus = "Extracting";
        opt->slowQueries++;
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
    trav->trans.timestamp = getTimestampAt(trav->app->blk);
    trav->block.timestamp = getTimestampAt(trav->app->blk);

    if (opt->traces && trav->trans.traces.size() == 0) {
        dirty = true;
        loadTraces(trav->trans, trav->app->blk, trav->app->txid, opt->cache_traces,
                   (opt->skip_ddos && excludeTrace(&trav->trans, opt->max_traces)));
    }

    dirty |= opt->articulateAll(trav->trans);

    // TODO(tjayrush): This could be in post_Func so that other functions can also make it dirty
    if (opt->cache_txs && dirty) {
        opt->stats.nCacheWrites++;
        // if the node is behind the index, this will sometimes happen - don't write in that case
        if (!trav->trans.hash.empty()) {
            writeTransToBinary(trav->trans, txFilename);
        }
    }

    opt->markNeighbors(trav->trans);

    return true;
}
