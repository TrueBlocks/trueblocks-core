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
#define LOG_ONE(a, b, d)                                                                                               \
    if (b != d) {                                                                                                      \
        LOG_INFO("   ", a, b);                                                                                         \
    }

//---------------------------------------------------------------------------
#define LOG_TRIAL_BALANCE_INSIDE()                                                                                     \
    if (isTestMode()) {                                                                                                \
        LOG_INFO("--------------------------------------------------------");                                          \
        LOG_INFO("Trial balance: ", reconciliationType);                                                               \
        LOG_ONE("hash:                ", pTransaction->hash, "");                                                      \
        LOG_ONE("blockNumber:         ", pTransaction->blockNumber, NOPOS);                                            \
        LOG_ONE("transactionIndex:    ", pTransaction->transactionIndex, NOPOS);                                       \
        LOG_ONE("begBal:              ", begBal, 0);                                                                   \
        LOG_ONE("amountIn:            ", amountIn, 0);                                                                 \
        LOG_ONE("internalIn:          ", internalIn, 0);                                                               \
        LOG_ONE("selfDestructIn:      ", selfDestructIn, 0);                                                           \
        LOG_ONE("minerBaseRewardIn:   ", minerBaseRewardIn, 0);                                                        \
        LOG_ONE("minerNephewRewardIn: ", minerNephewRewardIn, 0);                                                      \
        LOG_ONE("minerTxFeeIn:        ", minerTxFeeIn, 0);                                                             \
        LOG_ONE("minerUncleRewardIn:  ", minerUncleRewardIn, 0);                                                       \
        LOG_ONE("prefundIn:           ", prefundIn, 0);                                                                \
        LOG_ONE("totalIn:             ", totalIn(), 0);                                                                \
        LOG_ONE("amountOut:           ", amountOut, 0);                                                                \
        LOG_ONE("internalOut:         ", internalOut, 0);                                                              \
        LOG_ONE("selfDestructOut:     ", selfDestructOut, 0);                                                          \
        LOG_ONE("gasOut:              ", gasOut, 0);                                                                   \
        LOG_ONE("totalOut:            ", totalOut(), 0);                                                               \
        LOG_ONE("amountNet:           ", amountNet(), 0);                                                              \
        LOG_ONE("endBal:              ", endBal, 0);                                                                   \
        LOG_INFO("--------------------------------------------------------");                                          \
    }

//-----------------------------------------------------------------------
bool CReconciliation::reconcileInside(void) {
    assetSymbol = "ETH";
    assetAddr = FAKE_ETH_ADDRESS;

    begBal = 0;
    if (blockNumber > 0) {
        begBal = getBalanceAt(accountedFor, blockNumber - 1);
    }
    endBal = getBalanceAt(accountedFor, blockNumber);

    if (pTransaction->from == accountedFor) {
        sender = pTransaction->from;
        recipient = pTransaction->to;
        amountOut = pTransaction->isError ? 0 : pTransaction->value;
        gasOut = str_2_BigInt(pTransaction->getValueByName("gasCost"));
    }

    if (pTransaction->to == accountedFor) {
        sender = pTransaction->from;
        recipient = pTransaction->to;
        if (pTransaction->from == "0xPrefund") {
            prefundIn = pTransaction->value;
        } else if (pTransaction->from == "0xBlockReward") {
            minerBaseRewardIn = pTransaction->value;
            minerNephewRewardIn = pTransaction->extraValue1;
            minerTxFeeIn = pTransaction->extraValue2;
        } else if (pTransaction->from == "0xUncleReward") {
            minerUncleRewardIn = pTransaction->value;
        } else {
            amountIn = pTransaction->isError ? 0 : pTransaction->value;
        }
    }

    LOG_TRIAL_BALANCE_INSIDE();
    if (begBal + totalIn() - totalOut() == endBal) {
        return true;
    }

    if (reconcileUsingTraces()) {
        return true;
    }

    return true;
}

//---------------------------------------------------------------------------
bool CReconciliation::reconcileUsingTraces(void) {
    amountOut = amountIn = 0;
    prefundIn = minerBaseRewardIn = minerNephewRewardIn = minerTxFeeIn + minerUncleRewardIn = 0;

    if (pTransaction->blockNumber == 0) {
        sender = pTransaction->from;
        recipient = pTransaction->to;
        begBal = 0;
        prefundIn = pTransaction->value;

    } else {
        bool allocated = false;
        if (pTransaction->traces.size() == 0) {
            blknum_t bn = pTransaction->blockNumber;
            blknum_t txid = pTransaction->transactionIndex;
            loadTraces(*((CTransaction*)pTransaction), bn, txid, false, false);  // NOLINT
            allocated = true;
        }

        for (auto trace : pTransaction->traces) {
            if (!trace.action.selfDestructed.empty()) {
                // do not collapse, both may happen
                if (trace.action.refundAddress == accountedFor) {
                    sender = pTransaction->from;
                    recipient = pTransaction->to;
                    selfDestructIn += trace.action.balance;
                }
                // do not collapse, both may happen
                if (trace.action.selfDestructed == accountedFor) {
                    sender = pTransaction->from;
                    recipient = pTransaction->to;
                    selfDestructOut += trace.action.balance;
                }

            } else {
                if (trace.action.from == accountedFor && !trace.isDelegateCall()) {
                    // Sometimes, EOAs appear here, but there is no way
                    // that a trace can initiate an expenditure on an EOA
                    // TODO(tjayrush): unless it's the first trace?
                    // unless the EOA initiated the top level tx. I think
                    // this might be a bug in a smart contract or something.
                    if (isContractAt(accountedFor, blockNumber) || pTransaction->from == accountedFor) {
                        sender = pTransaction->from;
                        recipient = pTransaction->to;
                        internalOut += pTransaction->isError ? 0 : trace.action.value;
                    }
                }

                if (trace.action.to == accountedFor && !trace.isDelegateCall()) {
                    sender = pTransaction->from;
                    recipient = pTransaction->to;
                    if (pTransaction->from == "0xPrefund") {
                        prefundIn = pTransaction->value;

                    } else if (pTransaction->from == "0xBlockReward") {
                        minerBaseRewardIn = pTransaction->value;
                        minerNephewRewardIn = pTransaction->extraValue1;
                        minerTxFeeIn = pTransaction->extraValue2;

                    } else if (pTransaction->from == "0xUncleReward") {
                        minerUncleRewardIn = pTransaction->value;

                    } else {
                        internalIn += pTransaction->isError ? 0 : trace.action.value;
                    }
                }
            }
        }

        if (allocated) {
            ((CTransaction*)pTransaction)->traces.clear();
        }
    }

    reconciliationType = "by-trace";
    LOG_TRIAL_BALANCE_INSIDE();

    return (begBal + totalIn() - totalOut() == endBal);
}

//-----------------------------------------------------------------------
bool CReconciliation::reconcileAcross(bigint_t pBal, blknum_t pBn) {
    prevBal = pBal;
    prevAppBlk = pBn;

    bool prevDifferent = prevAppBlk != blockNumber;
    bool nextDifferent = blockNumber != nextAppBlk || nextAppBlk == NOPOS;

    bigint_t balEOLB = getBalanceAt(accountedFor, blockNumber == 0 ? 0 : blockNumber - 1);
    bigint_t balEOB = getBalanceAt(accountedFor, blockNumber);

    if (pTransaction->blockNumber == 0) {
    } else if (prevDifferent && nextDifferent) {
        // The trace reconcile may have changed values
        begBal = balEOLB;
        endBal = balEOB;

    } else if (prevDifferent) {
        // This tx has a tx after it in the same block but none before it
        begBal = balEOLB;
        endBal = endBalCalc();

    } else if (nextDifferent) {
        // This tx has a tx before it in the block but none after it
        begBal = pBal;
        endBal = balEOB;

    } else {
        // this tx has both a tx before it and one after it in the same block
        begBal = pBal;
        endBal = endBalCalc();
    }

    return reconciled();
}

//-----------------------------------------------------------------------
bool CReconciliation::reconcileLabel(blknum_t pBn) {
    bool prevDifferent = prevAppBlk != blockNumber;
    bool nextDifferent = blockNumber != nextAppBlk || nextAppBlk == NOPOS;
    if (pTransaction->blockNumber == 0) {
        reconciliationType = "genesis";

    } else if (prevDifferent && nextDifferent) {
        reconciliationType = "regular";

    } else if (prevDifferent) {
        reconciliationType = "prevdiff-partial";

    } else if (nextDifferent) {
        reconciliationType = "partial-nextdiff";

    } else {
        reconciliationType = "partial-partial";
    }

    LOG_TRIAL_BALANCE_INSIDE();
    return reconciled();
}

}  // namespace qblocks
