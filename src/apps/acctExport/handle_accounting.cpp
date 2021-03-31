/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool handle_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prev, blknum_t next,
                           bool tokens) {
    CReconciliation nums;
    nums.blockNumber = trans.blockNumber;
    nums.transactionIndex = trans.transactionIndex;
    nums.timestamp = trans.timestamp;
    CStringArray corrections;
    nums.reconcileEth(corrections, prev, next, &trans);
    trans.statements.clear();
    trans.statements.push_back(nums);
    prev[expContext().accountedFor + "_eth"] = nums;
    if (tokens) {
        CAddressBoolMap done;
        for (auto log : trans.receipt.logs) {
            const CAccountName& name = options->tokenMap[log.address];
            if (name.address == log.address && !done[log.address]) {
                nums.reset();
                nums.asset = name.symbol.empty() ? name.name.substr(0, 4) : name.symbol;
                CMonitor m;
                m.address = log.address;
                string key = expContext().accountedFor + "_" + log.address;
                CReconciliation p = prev[key];
                nums.begBal = prev[key].endBal;
                nums.endBal = str_2_BigInt(getTokenBalanceOf(m, expContext().accountedFor, trans.blockNumber));
                if (nums.begBal > nums.endBal) {
                    nums.amountOut = (nums.begBal - nums.endBal);
                } else {
                    nums.amountIn = (nums.endBal - nums.begBal);
                }
                nums.amountNet = nums.amountIn - nums.amountOut;
                nums.reconciled = true;
                done[log.address] = true;
                if (nums.amountNet != 0)
                    trans.statements.push_back(nums);
                prev[key] = nums;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------
bool COptions::handle_accounting(void) {
    ENTER("handle_accounting");

    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    CReconciliationMap prev;
    if (apps.size() > 0 && first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = apps[0].blk - 1;
        eth.endBal = getBalanceAt(expContext().accountedFor, apps[0].blk - 1);
        prev[expContext().accountedFor + "_eth"] = eth;
    }

    bool first = true;
    blknum_t lastExportedBlock = NOPOS;
    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < freshen_max)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= expContext().tsCnt)
            break;

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
                block.timestamp = trans.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                // This data isn't stored, so we need to recreate it
                if (accounting) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    handle_reconciliation(this, trans, prev, next, tokens);
                }
                markNeighbors(trans);
                articulateAll(trans);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(nProcessed % 5)) {
                    blknum_t current = first_record + i;
                    ostringstream post;
                    post << " txs for address " << allMonitors[0].address;
                    post << " " << first_record << " " << nProcessed << " " << i << " " << nTransactions << "\r";
                    LOG_PROGRESS("Reading ", current, nTransactions, post.str());
                }

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
                block.timestamp = trans.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                // This data isn't stored, so we need to recreate it
                if (accounting) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    handle_reconciliation(this, trans, prev, next, tokens);
                }
                markNeighbors(trans);
                articulateAll(trans);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(nProcessed % 5)) {
                    blknum_t current = first_record + i;
                    ostringstream post;
                    post << " txs for address " << allMonitors[0].address;
                    post << " " << first_record << " " << nProcessed << " " << i << " " << nTransactions << "\r";
                    LOG_PROGRESS("Extracting ", current, nTransactions, post.str());
                }

                if (cache_txs)
                    writeTransToBinary(trans, txFilename);
            }

            nProcessed++;
            if (shouldDisplay) {
                cout << ((isJson() && !first) ? ", " : "");
                cout << trans.Format() << endl;
                first = false;
            }
        } else if (app->blk > exportRange.second) {
            break;
        }
    }

    if (!isTestMode()) {
        if ((first_record + nProcessed) == nTransactions)
            LOG_PROGRESS((freshen ? "Finished updating" : "Finished reporting on"), (first_record + nProcessed),
                         nTransactions, " txs for address " + allMonitors[0].address);
    }

    if (lastExportedBlock != NOPOS)
        for (auto monitor : allMonitors)
            monitor.writeLastExport(lastExportedBlock);

    reportNeighbors();

    EXIT_NOMSG(true);
}
