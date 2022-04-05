/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    //    loadEnvironmentPaths("mainnet", "/Users/jrush/Development/trueblocks-index/wild.0/unchained/", "");
    acctlib_init(quickQuitHandler);

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
                    : (options.neighbors
                    ? GETRUNTIME_CLASS(CAppearanceDisplay)->m_ClassName
                      : (options.logs
                      ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                        : GETRUNTIME_CLASS(CTransaction)->m_ClassName))))));
            // clang-format on

            if (once)
                cout << exportPreamble(expContext().fmtMap["header"], options.className);

            if (options.appearances) {
                CAppearanceTraverser at;
                at.traverserRange = options.exportRange;
                traversers.push_back(at);
            }

            if (options.receipts) {
                CReceiptTraverser rt;
                rt.traverserRange = options.exportRange;
                traversers.push_back(rt);
            }

            if (options.statements) {
                CStatementTraverser st;
                st.traverserRange = options.exportRange;
                traversers.push_back(st);
            }

            if (options.neighbors) {
                CNeighborTraverser nt;
                nt.traverserRange = options.exportRange;
                traversers.push_back(nt);
            }

            if (options.logs) {
                CLogTraverser lt;
                lt.traverserRange = options.exportRange;
                traversers.push_back(lt);
            }

            if (options.traces) {
                CTraceTraverser tt;
                tt.traverserRange = options.exportRange;
                traversers.push_back(tt);
            }

            if (traversers.empty()) {
                CTransactionTraverser tt;
                tt.traverserRange = options.exportRange;
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
        findName(options.accountedFor.address, options.allMonitors[0]);
        if (options.abi_spec.nInterfaces() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.allMonitors[0] << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    options.writePerformanceData();

    acctlib_cleanup();
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
// Returns false if the loop shouldQuit
bool prog_Log(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;
    if (!trav->logging)
        return !shouldQuit();

    size_t freq = opt->reportFreq;
    opt->reportFreq = opt->reportDef;  // reset
    if (trav->nProcessed % freq)
        return !shouldQuit();

    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t nApps = opt->stats.nFileRecords;

    ostringstream post;
    if (trav->searchType == "appearances" || trav->searchType == "receipts" || trav->searchType == "txs") {
        // We report differently if there are the same number of items as appearances...
        // Reports as "searchType index of total operation for address A"
        post << " " << trav->searchType << " for address " << opt->accountedFor.address;
    } else {
        // ...or if there are more than such as statements, logs, traces, or neighbors
        // Reports as "searchType index of total txs (found X operation) for address A"
        post << " txs (" << prog << " " << trav->searchType << ") for address " << opt->accountedFor.address;
    }
    LOG_PROGRESS(trav->searchOp, blknum_t(opt->first_record + trav->index), nApps, post.str() + "\r");

    return (opt->slowQueries <= opt->maxSlowQueries && !shouldQuit());
}

//-----------------------------------------------------------------------
void end_Log(CTraverser* trav, void* data) {
    const COptions* opt = (const COptions*)data;
    if (!trav->logging)
        return;

    blknum_t prog = opt->first_record + trav->nProcessed;
    blknum_t nApps = opt->stats.nFileRecords;

    ostringstream post;
    if (trav->searchType == "appearances" || trav->searchType == "receipts" || trav->searchType == "txs") {
        // We report differently if there are the same number of items as appearances...
        // Reports as "searchType index of total operation for address A"
        post << " " << trav->searchType << " for address " << opt->accountedFor.address;
    } else {
        // ...or if there are more than such as statements, logs, traces, or neighbors
        // Reports as "searchType index of total txs (found X operation) for address A"
        post << " txs (" << prog << " " << trav->searchType << ") for address " << opt->accountedFor.address;
    }
    LOG_PROGRESS(COMPLETE, blknum_t(opt->first_record + trav->index), nApps, post.str());
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
        trav->searchOp = searchOpType(READ);

    } else {
        trav->searchOp = EXTRACT;
        opt->slowQueries++;
        opt->reportFreq = 1;
        dirty = true;
        if (trav->app->blk == 0) {
            address_t addr = opt->prefundAddrMap[trav->app->txid];
            trav->trans.loadTransAsPrefund(trav->app->blk, trav->app->txid, addr, prefundAt(addr));

        } else if (trav->app->txid == 99996 || trav->app->txid == 99997 || trav->app->txid == 99999) {
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
    if (opt->cache && dirty) {
        opt->stats.nCacheWrites++;
        // if the node is behind the index, this will sometimes happen - don't write in that case
        if (!trav->trans.hash.empty()) {
            writeTransToBinary(trav->trans, txFilename);
        }
    }

    opt->markNeighbors(trav->trans);

    return true;
}
