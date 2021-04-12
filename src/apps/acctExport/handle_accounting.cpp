/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool acct_Pre(const CTraverser* trav, void* data);
extern bool acct_Display(const CTraverser* trav, void* data);
extern bool acct_Post(const CTraverser* trav, void* data);
extern void acct_Log(const CTraverser* trav, void* data, TraverserLog mode);
//-----------------------------------------------------------------------
bool COptions::handle_accounting(void) {
    CTraverser trav(this, cout, "txs");
    acct_Pre(&trav, nullptr);

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
                // Order matters -- this data isn't stored, so we need to recreate it
                if (accounting) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    handle_reconciliation(this, trans, prevStatements, next, tokens);
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

                markNeighbors(trans);
                articulateAll(trans);
                // Order matters -- this data isn't stored, so we need to recreate it
                if (accounting) {
                    blknum_t next = i < apps.size() - 1 ? apps[i + 1].blk : NOPOS;
                    handle_reconciliation(this, trans, prevStatements, next, tokens);
                }

                if (cache_txs)
                    writeTransToBinary(trans, txFilename);
            }

            nProcessed++;
            if (!freshen) {
                cout << ((isJson() && !firstOut) ? ", " : "");
                cout << trans.Format() << endl;
                firstOut = false;
            }

            if (!isTestMode() && !(nProcessed % 5)) {
                // Use inCache to distinguish between Reading and Fetching
                acct_Log(&trav, nullptr, TR_PROGRESS);
            }

        } else if (app->blk > scanRange.second) {
            break;
        }
    }

    acct_Post(&trav, nullptr);
    return true;
}

//-----------------------------------------------------------------------
bool acct_Pre(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    if (opt->apps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->apps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}

//-----------------------------------------------------------------------
bool acct_Post(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->lastExpBlock != NOPOS)
        for (auto monitor : opt->allMonitors)
            monitor.writeLastExport(trav->lastExpBlock);

    opt->reportNeighbors();

    if (!isTestMode())
        acct_Log(trav, data, TR_END);

    return true;
}

//-----------------------------------------------------------------------
void acct_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (mode == TR_END) {
        end_Log(trav, data, mode);

    } else if (mode == TR_PROGRESS) {
        prog_Log(trav, data, mode);
    }
}

//-----------------------------------------------------------------------
bool handle_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prevStatements, blknum_t next,
                           bool tokens) {
    CReconciliation nums;
    nums.blockNumber = trans.blockNumber;
    nums.transactionIndex = trans.transactionIndex;
    nums.timestamp = trans.timestamp;
    CStringArray corrections;
    nums.reconcileEth(corrections, prevStatements, next, &trans, options->accountedFor);
    trans.statements.clear();
    trans.statements.push_back(nums);
    prevStatements[options->accountedFor + "_eth"] = nums;
    if (tokens) {
        CAddressBoolMap done;
        for (auto log : trans.receipt.logs) {
            CAccountName tokenName = options->tokenMap[log.address];
            bool isToken = tokenName.address == log.address;
            bool isAirdrop = options->airdropMap[log.address];
            bool isDone = done[log.address];
            if ((isToken || trans.hasToken || isAirdrop) && !isDone) {
                CMonitor m;
                m.address = log.address;
                nums.reset();
                nums.asset = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
                if (nums.asset.empty())
                    nums.asset = getTokenSymbol(m, trans.blockNumber);
                if (isAirdrop && nums.asset.empty()) {
                    options->getNamedAccount(tokenName, log.address);
                    nums.asset = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
                }
                if (nums.asset.empty())
                    nums.asset = "---";
                nums.decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;
                string key = options->accountedFor + "_" + log.address;
                nums.begBal = prevStatements[key].endBal;
                nums.endBal = str_2_BigInt(getTokenBalanceOf(m, options->accountedFor, trans.blockNumber));
                if (nums.begBal > nums.endBal) {
                    nums.amountOut = (nums.begBal - nums.endBal);
                } else {
                    nums.amountIn = (nums.endBal - nums.begBal);
                }
                nums.amountNet = nums.amountIn - nums.amountOut;
                nums.reconciled = true;
                nums.reconciliationType = "";
                done[log.address] = true;
                if (nums.amountNet != 0)
                    trans.statements.push_back(nums);
                prevStatements[key] = nums;
            }
        }
    }
    return true;
}
