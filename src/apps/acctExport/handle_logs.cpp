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

    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    CReconciliation lastStatement;
    if (items.size() > 0 && first_record != 0)
        lastStatement.endBal = getBalanceAt(expContext().accountedFor, items[0].blk - 1);

    bool first = true;
    for (size_t i = 0; i < items.size() && (!freshen || (nExported < freshen_max)); i++) {
        const CAppearance_base* item = &items[i];
        if (shouldQuit() || item->blk >= ts_cnt)
            break;
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = item->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
            if (item->blk != 0 && fileExists(txFilename)) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                // This data isn't stored, so we need to recreate it
                if (accounting || statements) {
                    blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
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

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(nExported % FREQ)) {
                    blknum_t current = first_record + nExported;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << monitors[0].address << "\r";
                    LOG_PROGRESS1("Reading", current, nTransactions, post.str());
                }

            } else {
                if (item->blk == 0) {
                    address_t addr = prefundAddrMap[item->txid];
                    trans.loadTransAsPrefund(item->blk, item->txid, addr, prefundWeiMap[addr]);

                } else if (item->txid == 99997 || item->txid == 99999) {
                    trans.loadTransAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

                } else if (item->txid == 99998) {
                    uint64_t nUncles = getUncleCount(item->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, item->blk, u);
                        if (uncle.miner == blkRewardMap[item->blk]) {
                            trans.loadTransAsUncleReward(item->blk, uncle.blockNumber, uncle.miner);
                        }
                    }

                } else {
                    getTransaction(trans, item->blk, item->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                if (accounting || statements) {
                    blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
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

                if (articulate) {
                    abiMap[trans.to]++;
                    if (abiMap[trans.to] == 1 || fileExists(getAbiPath(trans.to))) {
                        CStringArray unused;
                        loadAbiAndCache(abis, trans.to, false, unused);
                    }
                    abis.articulateTransaction(&trans);

                    for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
                        CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];
                        string_q str = log->Format();
                        if (contains(str, bytesOnly)) {
                            abiMap[log->address]++;
                            if (abiMap[log->address] == 1 || fileExists(getAbiPath(log->address))) {
                                CStringArray unused;
                                loadAbiAndCache(abis, log->address, false, unused);
                            }
                            abis.articulateLog(log);
                        }
                    }

                    for (size_t j = 0; j < trans.traces.size(); j++) {
                        CTrace* trace = (CTrace*)&trans.traces[j];
                        abiMap[trace->action.to]++;
                        if (abiMap[trace->action.to] == 1 || fileExists(getAbiPath(trace->action.to))) {
                            CStringArray unused;
                            loadAbiAndCache(abis, trace->action.to, false, unused);
                        }
                        abis.articulateTrace(trace);
                    }
                }
                if ((write_opt & CACHE_TXS))
                    writeTransToBinary(trans, txFilename);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(nExported % FREQ)) {
                    blknum_t current = first_record + nExported;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << monitors[0].address << "\r";
                    LOG_PROGRESS1("Extracting", current, nTransactions, post.str());
                }
            }

            for (auto log : trans.receipt.logs) {
                if (!emitter || log.address == monitors[0].address) {
                    nExported++;
                    if (shouldDisplay) {
                        cout << ((isJson() && !first) ? ", " : "");
                        cout << log.Format() << endl;
                        first = false;
                    }
                }
            }
        }
    }

    if (!isTestMode())
        LOG_PROGRESS1("Reported", (first_record + nExported), nTransactions,
                      " transactions for address " + monitors[0].address + "\r");

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportNeighbors();

    EXIT_NOMSG(true);
}
