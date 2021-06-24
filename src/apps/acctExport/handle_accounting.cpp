/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern bool process_reconciliation(COptions* options, CReconciliation& nums, CTransaction& trans,
                                   CReconciliationMap& prev, blknum_t next, bool tokens);

//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (fourByteFilter(trav->trans.input, opt)) {
        if (opt->accounting) {
            CReconciliation nums(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
            blknum_t next = trav->index < opt->monApps.size() - 1 ? opt->monApps[trav->index + 1].blk : NOPOS;
            process_reconciliation(opt, nums, trav->trans, opt->prevStatements, next, opt->tokens);
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool acct_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (opt->monApps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->monApps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->monApps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}

//-----------------------------------------------------------------------
bool process_reconciliation(COptions* options, CReconciliation& nums, CTransaction& trans,
                            CReconciliationMap& prevStatements, blknum_t next, bool tokens) {
    CStringArray corrections;
    nums.reconcileEth(corrections, prevStatements, next, &trans, options->accountedFor);
    trans.statements.clear();
    trans.statements.push_back(nums);
    prevStatements[options->accountedFor + "_eth"] = nums;
    if (tokens) {
        CAddressBoolMap done;
        for (auto log : trans.receipt.logs) {
            CAccountName tokenName;
            bool isToken = options->findToken(tokenName, log.address);
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
                if (nums.asset.empty()) {
                    nums.asset = log.address.substr(0, 4) + "...";
                }
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
