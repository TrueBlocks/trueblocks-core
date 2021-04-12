/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool process_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prevStatements, blknum_t next,
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
