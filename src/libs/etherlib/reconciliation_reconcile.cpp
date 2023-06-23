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

#define LOG_ONE(a, b, c)                                                                                               \
    {                                                                                                                  \
        string_q t = ":";                                                                                              \
        cerr << padRight("TEST[DATE|TIME]", 19) << padRight(((a) + t), 20) << " " << (b) << endl;                      \
    }

//---------------------------------------------------------------------------
#define LOG_TRIAL_BALANCE(msg)                                                                                         \
    if (isTestMode()) {                                                                                                \
        rCtx.DEBUG(msg);                                                                                               \
        cerr << "TEST[DATE|TIME] ---------------------------------------------------" << endl;                         \
        cerr << "TEST[DATE|TIME] Trial balance:" << endl;                                                              \
        LOG_ONE("reconciliationType", reconciliationType, "");                                                         \
        LOG_ONE("accountedFor", accountedFor, "");                                                                     \
        LOG_ONE("sender", sender, "");                                                                                 \
        LOG_ONE("recipient", recipient, "");                                                                           \
        LOG_ONE("assetAddr", assetAddr, "");                                                                           \
        LOG_ONE("assetSymbol", assetSymbol, "");                                                                       \
        LOG_ONE("decimals", decimals, "");                                                                             \
        LOG_ONE("prevAppBlk", prevAppBlk, "");                                                                         \
        LOG_ONE("hash", transactionHash, "");                                                                          \
        LOG_ONE("timestamp", timestamp, "");                                                                           \
        LOG_ONE("blockNumber", blockNumber, NOPOS);                                                                    \
        LOG_ONE("transactionIndex", transactionIndex, NOPOS);                                                          \
        LOG_ONE("logIndex", logIndex, NOPOS);                                                                          \
        LOG_ONE("priceSource", priceSource, 0);                                                                        \
        LOG_ONE("spotPrice", spotPrice, 0);                                                                            \
        LOG_ONE("prevBal", prevBal, 0);                                                                                \
        LOG_ONE("begBal", begBal, 0);                                                                                  \
        LOG_ONE("amountIn", amountIn, 0);                                                                              \
        LOG_ONE("internalIn", internalIn, 0);                                                                          \
        LOG_ONE("minerBaseRewardIn", minerBaseRewardIn, 0);                                                            \
        LOG_ONE("minerNephewRewardIn", minerNephewRewardIn, 0);                                                        \
        LOG_ONE("minerTxFeeIn", minerTxFeeIn, 0);                                                                      \
        LOG_ONE("minerUncleRewardIn", minerUncleRewardIn, 0);                                                          \
        LOG_ONE("prefundIn", prefundIn, 0);                                                                            \
        LOG_ONE("selfDestructIn", selfDestructIn, 0);                                                                  \
        LOG_ONE("totalIn", totalIn(), 0);                                                                              \
        LOG_ONE("amountOut", amountOut, 0);                                                                            \
        LOG_ONE("internalOut", internalOut, 0);                                                                        \
        LOG_ONE("selfDestructOut", selfDestructOut, 0);                                                                \
        LOG_ONE("gasOut", gasOut, 0);                                                                                  \
        LOG_ONE("totalOut", totalOut(), 0);                                                                            \
        LOG_ONE("amountNet", amountNet(), 0);                                                                          \
        LOG_ONE("endBal", endBal, 0);                                                                                  \
        LOG_ONE("begBalDiff", (trialBalance() ? 0 : begBalDiff()), 0);                                                 \
        LOG_ONE("endBalDiff", (trialBalance() ? 0 : endBalDiff()), 0);                                                 \
        LOG_ONE("endBalCalc", (trialBalance() ? 0 : endBalCalc()), 0);                                                 \
        LOG_ONE("trialBalance", (trialBalance() ? "true" : "false"), string_q(""));                                    \
        cerr << "TEST[DATE|TIME] ---------------------------------------------------" << endl;                         \
    }

//-----------------------------------------------------------------------
bool CReconciliation::reconcileFlows(const CTransfer& transfer, const CReconContext& rCtx) {
    sender = transfer.sender;
    recipient = transfer.recipient;

    bool isEth = isEtherAddr(assetAddr);
    reconciliationType = rCtx.reconType(isEth, pTransaction->blockNumber == 0, "");
    if (isEth) {
        if (pTransaction->from == accountedFor) {
            gasOut = str_2_BigInt(pTransaction->getValueByName("gasCost"));
        }

        // Do not collapse. Both may be true.
        if (sender == accountedFor) {
            amountOut = pTransaction->isError ? 0 : pTransaction->value;
        }

        // Do not collapse. Both may be true.
        if (recipient == accountedFor) {
            if (sender == "0xPrefund") {
                prefundIn = pTransaction->value;

            } else if (sender == "0xBlockReward") {
                minerBaseRewardIn = pTransaction->value;
                minerNephewRewardIn = pTransaction->extraValue1;
                minerTxFeeIn = pTransaction->extraValue2;

            } else if (sender == "0xUncleReward") {
                minerUncleRewardIn = pTransaction->value;

            } else {
                amountIn = pTransaction->isError ? 0 : pTransaction->value;
            }
        }

    } else {
        // Do not collapse. Both may be true.
        if (sender == accountedFor) {
            amountOut = transfer.amount;
        }

        // Do not collapse. Both may be true.
        if (recipient == accountedFor) {
            amountIn = transfer.amount;
        }

        if (sender != accountedFor && recipient != accountedFor) {
            LOG_ERR("Invalid state in reconcileFlows. Quitting...");
            exit(0);
        }
    }

    // For trialBalance only
    begBal = blockNumber == 0 ? 0 : getTokenBalanceAt(assetAddr, accountedFor, blockNumber - 1);
    endBal = getTokenBalanceAt(assetAddr, accountedFor, blockNumber);

    ((CReconciliation*)this)->priceSource = (priceSource.empty() ? "uniswap" : priceSource);

    ostringstream os;
    LOG_TRIAL_BALANCE((isEth ? "ETH" : "TOKENS"));
    if (trialBalance()) {
        return true;
    }

    return reconcileFlows_traces(rCtx);
}

//-----------------------------------------------------------------------
bool CReconciliation::reconcileFlows_traces(const CReconContext& rCtx) {
    bool isEth = isEtherAddr(assetAddr);
    if (!isEth) {
        return true;
    }
    reconciliationType = rCtx.reconType(isEth, pTransaction->blockNumber == 0, "traces-");

    amountIn = 0;
    internalIn = selfDestructIn = prefundIn = 0;
    minerBaseRewardIn = minerNephewRewardIn = minerTxFeeIn + minerUncleRewardIn = 0;
    amountOut = 0;
    internalOut = selfDestructOut = 0;  // don't reset gasOut as it's always top level

    if (pTransaction->blockNumber == 0) {
        sender = pTransaction->from;
        recipient = pTransaction->to;
        begBal = 0;
        prefundIn = pTransaction->value;

    } else {
        bool tracesAllocated = false;
        if (pTransaction->traces.size() == 0) {
            blknum_t bn = pTransaction->blockNumber;
            blknum_t txid = pTransaction->transactionIndex;
            loadTraces(*((CTransaction*)pTransaction), bn, txid, false, false);  // NOLINT
            tracesAllocated = true;
        }

        size_t index = 0;
        for (auto trace : pTransaction->traces) {
            if (!trace.action.selfDestructed.empty()) {
                // do not collapse, both may happen
                if (trace.action.refundAddress == accountedFor) {
                    sender = trace.action.from;
                    recipient = trace.action.to;
                    selfDestructIn += trace.action.balance;
                }

                // do not collapse, both may happen
                if (trace.action.selfDestructed == accountedFor) {
                    sender = trace.action.from;
                    recipient = trace.action.to;
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
                        sender = trace.action.from;
                        recipient = trace.action.to;
                        if (index == 0) {
                            amountOut += pTransaction->isError ? 0 : trace.action.value;
                        } else {
                            internalOut += pTransaction->isError ? 0 : trace.action.value;
                        }
                    }
                }

                if (trace.action.to == accountedFor && !trace.isDelegateCall()) {
                    sender = trace.action.from;
                    recipient = trace.action.to;
                    if (pTransaction->from == "0xPrefund") {
                        prefundIn = pTransaction->value;

                    } else if (pTransaction->from == "0xBlockReward") {
                        minerBaseRewardIn = pTransaction->value;
                        minerNephewRewardIn = pTransaction->extraValue1;
                        minerTxFeeIn = pTransaction->extraValue2;

                    } else if (pTransaction->from == "0xUncleReward") {
                        minerUncleRewardIn = pTransaction->value;

                    } else {
                        if (index == 0) {
                            amountIn += pTransaction->isError ? 0 : trace.action.value;
                        } else {
                            internalIn += pTransaction->isError ? 0 : trace.action.value;
                        }
                    }
                }
            }
            index++;
        }

        if (tracesAllocated) {
            ((CTransaction*)pTransaction)->traces.clear();
        }
    }

    priceSource = (priceSource.empty() ? "uniswap" : priceSource);

    ostringstream os;
    LOG_TRIAL_BALANCE("traces");

    return trialBalance();
}

//-----------------------------------------------------------------------
string_q CReconContext::reconType(bool isEth, bool isGenesis, const string_q& in) const {
    string_q ret;
    if (isGenesis) {
        ret = "genesis";

    } else {
        if (isPrevDiff && isNextDiff) {
            ret = "regular";

        } else if (isPrevDiff) {
            ret = "prevDiff-same";

        } else if (isNextDiff) {
            ret = "same-nextDiff";

        } else {
            ret = "same-same";
        }
    }

    return in + ret + (isEth ? "-eth" : "-token");
}

//-----------------------------------------------------------------------
bool CReconciliation::reconcileBalances(bigint_t& begBalOut, bigint_t& endBalOut, const CReconContext& rCtx) {
    bigint_t balEOLB = blockNumber == 0 ? 0 : getTokenBalanceAt(assetAddr, accountedFor, blockNumber - 1);
    bigint_t balEOB = getTokenBalanceAt(assetAddr, accountedFor, blockNumber);
    begBalOut = balEOLB;
    endBalOut = balEOB;

    if (blockNumber == 0) {
        // balances are zero

    } else if (rCtx.isPrevDiff && rCtx.isNextDiff) {
        // The trace reconcile may have changed values
        begBal = balEOLB;
        endBal = balEOB;

    } else if (rCtx.isPrevDiff) {
        // This tx has a tx after it in the same block but none before it
        begBal = balEOLB;
        endBal = endBalCalc();

    } else if (rCtx.isNextDiff) {
        // This tx has a tx before it in the block but none after it
        begBal = prevBal;
        endBal = balEOB;

    } else {
        // this tx has both a tx before it and one after it in the same block
        begBal = prevBal;
        endBal = endBalCalc();
    }

    ((CReconciliation*)this)->priceSource = (priceSource.empty() ? "uniswap" : priceSource);
    ostringstream os;
    os << "[" << blockNumber << "] " << rCtx.isPrevDiff << " " << rCtx.isNextDiff;
    LOG_TRIAL_BALANCE((isEtherAddr(assetAddr) ? "BALANCE ETH" : "BALANCE TOKEN"));

    return trialBalance();
}

}  // namespace qblocks
