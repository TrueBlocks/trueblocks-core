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
#include "reconciliation.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
#define LOG_TRIAL_BALANCE()                                                                                            \
    LOG4("Trial balance: ", reconciliationType);                                                                       \
    LOG4("  hash: ", trans->hash);                                                                                     \
    LOG4("  ------------------------------");                                                                          \
    LOG4("  prevBal:       ", prevBal);                                                                                \
    LOG4("  begBal:        ", begBal);                                                                                 \
    LOG4("  begBalDiff:    ", begBalDiff_internal());                                                                  \
    LOG4("  ------------------------------");                                                                          \
    LOG8("  amountIn:      ", amountIn);                                                                               \
    LOG8("  internalIn:    ", internalIn);                                                                             \
    LOG8("  slfDstrctIn:   ", selfDestructIn);                                                                         \
    LOG8("  minBRwdIn:     ", minerBaseRewardIn);                                                                      \
    LOG8("  minNRwdIn:     ", minerNephewRewardIn);                                                                    \
    LOG8("  minTxFeeIn:    ", minerTxFeeIn);                                                                           \
    LOG8("  minURwdIn:     ", minerUncleRewardIn);                                                                     \
    LOG8("  prefundIn:     ", prefundIn);                                                                              \
    LOG4("  totalIn:       ", totalIn_internal());                                                                     \
    LOG8("  amountOut:     ", amountOut);                                                                              \
    LOG8("  internalOut:   ", internalOut);                                                                            \
    LOG8("  slfDstrctOt:   ", selfDestructOut);                                                                        \
    LOG8("  gasCostOut:    ", gasCostOut);                                                                             \
    LOG4("  totalOut:      ", totalOut_internal());                                                                    \
    LOG4("  amountNet:     ", amountNet_internal());                                                                   \
    LOG4("  endBal:        ", endBal);                                                                                 \
    LOG4("  ------------------------------");                                                                          \
    LOG4("  endBalCalc:    ", endBalCalc_internal());                                                                  \
    LOG4("  endBalDiff:    ", endBalDiff_internal());                                                                  \
    LOG4("  regular-recon: ", reconciled_internal() ? "true" : "false");

//-----------------------------------------------------------------------
bool CReconciliation::reconcileEth2(blknum_t nextBlock, const CTransaction* trans, const address_t& accountedFor) {
    CAccountName name;
    name.address = accountedFor;
    name.petname = addr_2_Petname(name.address, '-');
    findName(accountedFor, name);

    CReconciliation prev;
    prev.pTransaction = trans;
    prev.assetAddr = accountedFor;
    prev.endBal = trans->blockNumber == 0 ? 0 : getBalanceAt(accountedFor, trans->blockNumber - 1);
    bool ret = reconcileEth(prev, nextBlock, trans, name);
    spotPrice = getPriceInUsd(trans->blockNumber, priceSource);
    return ret;
}

//-----------------------------------------------------------------------
bool CReconciliation::reconcileEth(const CReconciliation& prevRecon, blknum_t nextBlock, const CTransaction* trans,
                                   const CAccountName& accountedFor) {
    address_t acctFor = accountedFor.address;
    prevBal = prevRecon.endBal;
    prevAppBlk = prevRecon.blockNumber;
    assetSymbol = "ETH";
    assetAddr = FAKE_ETH_ADDRESS;

    bigint_t balEOLB = getBalanceAt(acctFor, blockNumber == 0 ? 0 : blockNumber - 1);
    bigint_t balEOB = getBalanceAt(acctFor, blockNumber);

    begBal = balEOLB;
    endBal = balEOB;

    if (trans->from == acctFor) {
        sender = trans->from;
        recipient = trans->to;
        amountOut = trans->isError ? 0 : trans->value;
        gasCostOut = str_2_BigInt(trans->getValueByName("gasCost"));
    }

    if (trans->to == acctFor) {
        sender = trans->from;
        recipient = trans->to;
        if (trans->from == "0xPrefund") {
            prefundIn = trans->value;
        } else if (trans->from == "0xBlockReward") {
            minerBaseRewardIn = trans->value;
            minerNephewRewardIn = trans->extraValue1;
            minerTxFeeIn = trans->extraValue2;
        } else if (trans->from == "0xUncleReward") {
            minerUncleRewardIn = trans->value;
        } else {
            amountIn = trans->isError ? 0 : trans->value;
        }
    }

    bool prevDifferent = prevRecon.blockNumber != blockNumber;
    bool nextDifferent = blockNumber != nextBlock;
    if (trans->blockNumber == 0) {
        reconciliationType = "genesis";

    } else {
        if (prevDifferent && nextDifferent) {
            reconciliationType = "regular";

        } else if (prevDifferent) {
            reconciliationType = "prevdiff-partial";

        } else if (nextDifferent) {
            reconciliationType = "partial-nextdiff";

        } else {
            reconciliationType = "partial-partial";
        }
    }

    LOG_TRIAL_BALANCE();
    if (reconciled_internal())
        return true;

    // Reconciliation failed, let's try to reconcile by traces
    if (reconcileUsingTraces(prevRecon.endBal, trans, accountedFor))
        return true;

    // Reconciliation by traces failed, we want to correct for that and try
    // one more method - intra block.
    if (prevDifferent && nextDifferent) {
        // The trace reconcile may have changed values
        begBal = balEOLB;
        endBal = balEOB;
        reconciliationType = "regular";

    } else if (prevDifferent) {
        // This tx has a tx after it in the same block but none before it
        begBal = balEOLB;
        endBal = endBalCalc_internal();
        reconciliationType = "prevdiff-partial";

    } else if (nextDifferent) {
        // This tx has a tx before it in the block but none after it
        begBal = prevRecon.endBal;
        endBal = balEOB;
        reconciliationType = "partial-nextdiff";

    } else {
        // this tx has both a tx before it and one after it in the same block
        begBal = prevRecon.endBal;
        endBal = endBalCalc_internal();
        reconciliationType = "partial-partial";
    }

    LOG_TRIAL_BALANCE();
    return reconciled_internal();
}

//---------------------------------------------------------------------------
bool CReconciliation::reconcileUsingTraces(bigint_t prevEndBal, const CTransaction* trans,
                                           const CAccountName& accountedFor) {
    address_t acctFor = accountedFor.address;
    amountOut = amountIn = 0;
    prefundIn = minerBaseRewardIn = minerNephewRewardIn = minerTxFeeIn + minerUncleRewardIn = 0;

    if (trans->blockNumber == 0) {
        sender = trans->from;
        recipient = trans->to;
        begBal = 0;
        prefundIn = trans->value;
    } else {
        if (trans->traces.size() == 0) {
            blknum_t bn = trans->blockNumber;
            blknum_t txid = trans->transactionIndex;
            loadTraces(*((CTransaction*)trans), bn, txid, false, false);  // NOLINT
        }
        for (auto trace : trans->traces) {
            if (!trace.action.selfDestructed.empty()) {
                // do not collapse
                if (trace.action.refundAddress == acctFor) {
                    sender = trans->from;
                    recipient = trans->to;
                    selfDestructIn += trace.action.balance;
                }
                // do not collapse, both may happen
                if (trace.action.selfDestructed == acctFor) {
                    sender = trans->from;
                    recipient = trans->to;
                    selfDestructOut += trace.action.balance;
                }
            } else {
                if (trace.action.from == acctFor && !trace.isDelegateCall()) {
                    // Sometimes, EOAs appear here, but there is no way
                    // that a trace can initiate an expenditure on an EOA
                    // TODO(tjayrush): unless it's the first trace?
                    // unless the EOA initiated the top level tx. I think
                    // this might be a bug in a smart contract or something.
                    if (accountedFor.isContract || trans->from == acctFor) {
                        sender = trans->from;
                        recipient = trans->to;
                        internalOut += trans->isError ? 0 : trace.action.value;
                    }
                }

                if (trace.action.to == acctFor && !trace.isDelegateCall()) {
                    sender = trans->from;
                    recipient = trans->to;
                    if (trans->from == "0xPrefund") {
                        prefundIn = trans->value;
                    } else if (trans->from == "0xBlockReward") {
                        minerBaseRewardIn = trans->value;
                        minerNephewRewardIn = trans->extraValue1;
                        minerTxFeeIn = trans->extraValue2;
                    } else if (trans->from == "0xUncleReward") {
                        minerUncleRewardIn = trans->value;
                    } else {
                        internalIn += trans->isError ? 0 : trace.action.value;
                    }
                }
            }
        }
    }

    reconciliationType = "by-trace";
    LOG_TRIAL_BALANCE();
    if (!reconciled_internal())
        ((CTransaction*)trans)->traces.clear();  // NOLINT
    return reconciled_internal();
}

}  // namespace qblocks
