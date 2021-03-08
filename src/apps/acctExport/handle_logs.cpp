/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::handle_logs(void) {
    ENTER("handle_logs");

    ASSERT(logs && !(accounting || balances));
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    CReconciliation lastStatement;
    if (apps.size() > 0 && first_record != 0)
        lastStatement.endBal = getBalanceAt(expContext().accountedFor, apps[0].blk - 1);

    bool first = true;
    blknum_t lastExported = exportRange.second;
    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < freshen_max)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= ts_cnt) {
            lastExported = app->blk - 1;
            break;
        }

        if (inRange((blknum_t)app->blk, exportRange.first, exportRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = app->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, app->blk, app->txid);
            if (app->blk != 0 && fileExists(txFilename)) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(app->blk * 2) + 1];

                // This data isn't stored, so we need to recreate it
                if (accounting || statements) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.blockNumber = trans.blockNumber;
                    nums.transactionIndex = trans.transactionIndex;
                    nums.timestamp = trans.timestamp;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, next, &trans);
                    // trans.reconciliations.clear();
                    trans.reconciliations.push_back(nums);
                    trans.statements.clear();
                    CReconciliationOutput st(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                markNeighbors(trans);
                articulateAll(trans);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(i % FREQ)) {
                    blknum_t current = first_record + nProcessed;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << allMonitors[0].address << "\r";
                    LOG_PROGRESS("Reading", current, nTransactions, post.str());
                }

            } else {
                if (app->blk == 0) {
                    address_t addr = prefundAddrMap[app->txid];
                    trans.loadTransAsPrefund(app->blk, app->txid, addr, prefundWeiMap[addr]);

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
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(app->blk * 2) + 1];

                if (accounting || statements) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.blockNumber = trans.blockNumber;
                    nums.transactionIndex = trans.transactionIndex;
                    nums.timestamp = trans.timestamp;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, next, &trans);
                    trans.reconciliations.push_back(nums);
                    trans.statements.clear();
                    CReconciliationOutput st(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                markNeighbors(trans);
                articulateAll(trans);

                if (cache_txs)
                    writeTransToBinary(trans, txFilename);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(i % FREQ)) {
                    blknum_t current = first_record + nProcessed;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << allMonitors[0].address;
                    if (!isApiMode())
                        post << " " << app->blk;
                    LOG_PROGRESS("Extracting", current, nTransactions, post.str() + "\r");
                }
            }

            for (auto log : trans.receipt.logs) {
                if (!emitter || log.address == allMonitors[0].address) {
                    nProcessed++;
                    if (shouldDisplay) {
                        cout << ((isJson() && !first) ? ", " : "");
                        cout << log.Format() << endl;
                        first = false;
                    }
                }
            }
        } else if (app->blk > exportRange.second) {
            break;
        }
    }

    if (!isTestMode()) {
        if ((first_record + nProcessed) == nTransactions)
            LOG_PROGRESS((freshen ? "Finished updating" : "Finished reporting on"), (first_record + nProcessed),
                         nTransactions, " transactions for address " + allMonitors[0].address);
    }

    for (auto monitor : allMonitors)
        monitor.updateLastExport(lastExported);

    reportNeighbors();

    EXIT_NOMSG(true);
}
