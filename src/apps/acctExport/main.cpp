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

            forEveryAppearance(traversers, options.ledgerManager.appArray, &options);

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
        HIDE_FIELD(CMonitor, "abi_spec");
        if (!findName(options.ledgerManager.accountedFor, options.allMonitors[0])) {
            blknum_t blk = min(options.exportRange.second, options.meta.client);
            options.ledgerManager.name.isContract = isContractAt(options.allMonitors[0].address, blk);
            options.allMonitors[0].petname = addr_2_Petname(options.allMonitors[0].address, '-');
            HIDE_FIELD(CAccountName, "isCustom");
            HIDE_FIELD(CAccountName, "isPrefund");
            HIDE_FIELD(CAccountName, "isContract");
            HIDE_FIELD(CAccountName, "isErc20");
            HIDE_FIELD(CAccountName, "isErc721");
            options.allMonitors[0].decimals = 18;
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

extern const CStringArray searchOps;

//-----------------------------------------------------------------------
// Returns false if the loop shouldQuit
bool prog_Log(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;
    if (!trav->logging || isTestMode())
        return !shouldQuit();

    size_t freq = 7;
    if (trav->nProcessed % freq)
        return !shouldQuit();

    ostringstream found;
    if (trav->searchType != "appearances" && trav->searchType != "txs" && trav->searchType != "receipts") {
        found << " (found " << trav->nProcessed << " " << trav->searchType << ")";
    }

    LOG_PROG(searchOps[opt->ledgerManager.searchOp], " ", opt->first_record + trav->index, " of ",
             opt->stats.nFileRecords, " txs at block ", trav->trans.blockNumber, found.str(), " for address ",
             opt->ledgerManager.accountedFor, "\r");

    return !shouldQuit();
}

//-----------------------------------------------------------------------
void end_Log(CTraverser* trav, void* data) {
    const COptions* opt = (const COptions*)data;
    if (!trav->logging || isTestMode())
        return;

    ostringstream found;
    if (trav->searchType != "appearances" && trav->searchType != "txs" && trav->searchType != "receipts") {
        found << " (found " << trav->nProcessed << " " << trav->searchType << ")";
    }

    LOG_PROG(searchOps[opt->ledgerManager.searchOp], " ", opt->first_record + trav->index, " of ",
             opt->stats.nFileRecords, " txs at block ", trav->trans.blockNumber, found.str(), " for address ",
             opt->ledgerManager.accountedFor, "\r");

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
        opt->ledgerManager.searchOp = READ;

    } else {
        opt->ledgerManager.searchOp = EXTRACT;
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
    trav->trans.timestamp = bn_2_Timestamp(trav->app->blk);
    trav->block.timestamp = bn_2_Timestamp(trav->app->blk);

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

// clang-format off
const CStringArray searchOps = {
    "Extracting",
    "Reading",
    "Updating",
    "Reconciling",
    "Scanning",
    "Skipping",
    "Completed",
};
// clang-format on
