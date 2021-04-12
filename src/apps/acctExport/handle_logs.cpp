/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool logs_Pre(const CTraverser* trav, void* data);
extern bool logs_Display(const CTraverser* trav, void* data);
extern bool logs_Post(const CTraverser* trav, void* data);
extern void logs_Log(const CTraverser* trav, void* data, TraverserLog mode);
//-----------------------------------------------------------------------
bool COptions::handle_logs(void) {
    CTraverser trav(this, cout, "txs");
    logs_Pre(&trav, nullptr);

    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < 5000)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= expContext().tsCnt)
            break;

        if (inRange((blknum_t)app->blk, scanRange.first, scanRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = app->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, app->blk, app->txid);
            bool inCache = app->blk != 0 && fileExists(txFilename);
            if (inCache) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                markNeighbors(trans);
                articulateAll(trans);

            } else {
                if (app->blk == 0) {
                    address_t addr = prefundAddrMap[app->txid];
                    trans.loadTransAsPrefund(app->blk, app->txid, addr, expContext().prefundMap[addr]);

                } else if (app->txid == 99997 || app->txid == 99999) {
                    trans.loadTransAsBlockReward(app->blk, app->txid, blkRewardMap[app->blk]);

                } else if (app->txid == 99998) {
                    uint64_t nUncles = getUncleCount(app->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, app->blk, u);
                        if (uncle.miner == blkRewardMap[app->blk]) {
                            trans.loadTransAsUncleReward(app->blk, uncle.blockNumber, uncle.miner);
                        }
                    }

                } else {
                    getTransaction(trans, app->blk, app->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                markNeighbors(trans);
                articulateAll(trans);

                if (cache_txs)
                    writeTransToBinary(trans, txFilename);
            }

            for (auto log : trans.receipt.logs) {
                bool showMe = true;
                if (relevant) {
                    showMe = isRelevant(log);
                    if (showMe && !emitted_by.empty())
                        showMe = wasEmittedBy(log.address);
                } else if (!emitted_by.empty()) {
                    showMe = wasEmittedBy(log.address);
                } else if (emitter) {
                    showMe = isEmitter(log.address);
                }
                nProcessed++;
                if (showMe && !freshen) {
                    cout << ((isJson() && !firstOut) ? ", " : "");
                    cout << log.Format() << endl;
                    firstOut = false;
                }
            }

            if (!isTestMode() && (isApiMode() || !(nProcessed % 3))) {
                logs_Log(&trav, nullptr, TR_PROGRESS);
            }
        } else if (app->blk > scanRange.second) {
            break;
        }
    }

    logs_Post(&trav, nullptr);
    return true;
}

//-----------------------------------------------------------------------
bool COptions::isEmitter(const address_t& test) const {
    for (auto monitor : allMonitors)
        if (monitor.address == test)
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool COptions::wasEmittedBy(const address_t& test) const {
    for (auto e : emitted_by)
        if (e == test)
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool COptions::isRelevant(const CLogEntry& log) const {
    string_q str = toLower(log.Format(STR_DISPLAY_LOGENTRY));
    for (auto monitor : allMonitors)
        if (contains(str, monitor.address.substr(2)))
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool logs_Pre(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;
    if (opt->apps.size() > 0 && opt->first_record != 0)
        opt->lastStatement.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);
    return true;
}

//-----------------------------------------------------------------------
bool logs_Post(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->lastExpBlock != NOPOS)
        for (auto monitor : opt->allMonitors)
            monitor.writeLastExport(trav->lastExpBlock);

    opt->reportNeighbors();

    if (!isTestMode())
        logs_Log(trav, data, TR_END);

    return true;
}

//-----------------------------------------------------------------------
void logs_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (mode == TR_END) {
        end_Log(trav, data, mode);

    } else if (mode == TR_PROGRESS) {
        prog_Log(trav, data, mode);
    }
}
