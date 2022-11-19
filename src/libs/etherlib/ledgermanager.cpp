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
#include "etherlib.h"
#include "ledgermanager.h"

namespace qblocks {

//--------------------------------------------------------------
bool CLedgerManager::getStatements(CTransaction& trans) {
    if (trans.readReconsFromCache(accountedFor)) {
        searchOp = READ;
        for (auto& statement : trans.statements) {
            string_q key = statementKey(statement.accountedFor, statement.assetAddr);
            ledgers[key] = statement;
        }
        return !shouldQuit();
    }

    searchOp = RECONCILE;
    getTransfers(trans);

    for (size_t i = 0; i < transfers.size(); i++) {
        CTransfer& transfer = transfers[i];
        if (filterByAsset(transfer.assetAddr)) {
            string tokenKey = statementKey(accountedFor, transfer.assetAddr);
            if (ledgers[tokenKey] == CLedgerEntry()) {
                CReconciliation prevStatement(accountedFor, transfer.assetAddr, &trans);
                if (trans.blockNumber > 0) {
                    prevStatement.blockNumber = max(trans.blockNumber, blknum_t(1)) - 1;
                    prevStatement.endBal = trans.blockNumber == 0 ? 0
                                                                  : getTokenBalanceAt(transfer.assetAddr, accountedFor,
                                                                                      prevStatement.blockNumber);
                }
                ledgers[tokenKey] = prevStatement;
            }

            CReconciliation statement(accountedFor, transfer.assetAddr, &trans);
            statement.logIndex = transfer.logIndex;
            statement.assetSymbol = transfer.assetSymbol;
            statement.decimals = transfer.decimals;
            if (!statement.reconcileFlows(transfer)) {
                statement.reconcileFlows_traces();
            }

            bool isPrevDiff = transfer.blockNumber == 0 || (ledgers[tokenKey].blockNumber != transfer.blockNumber);
            bool isNextDiff = nextBlock != transfer.blockNumber && i == transfers.size() - 1;
            if (isTestMode()) {
                LOG_INFO("isPrevDiff: ", isPrevDiff, " isNextDiff: ", isNextDiff);
                LOG_INFO("ledger.blockNumber: ", ledgers[tokenKey].blockNumber,
                         " prevBlock: ", ledgers[tokenKey].blockNumber, " transfer.blockNumber: ", transfer.blockNumber,
                         " nextBlock: ", nextBlock);
            }

            statement.prevAppBlk = ledgers[tokenKey].blockNumber;
            statement.prevBal = ledgers[tokenKey].balance;
            statement.reconcileBalances(isPrevDiff, isNextDiff);
            if (statement.amountNet() != 0) {
                trans.statements.push_back(statement);
                ledgers[tokenKey] = statement;
            }
        }
    }

    for (size_t s = 0; s < trans.statements.size(); s++) {
        CReconciliation* st = &trans.statements[s];
        st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
static bool isTokenTransfer(const string_q& logNeedle) {
    return logNeedle == str_2_Topic("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
}

// //-----------------------------------------------------------------------
// static bool isDepositTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0xe1fffcc4923d04b559f4d29a8bfc6cda04eb5b0d3c460751c2402c5c5cc9109c");
// }

// //-----------------------------------------------------------------------
// static bool isWithdrawalTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x7fcf532c15f0a6db0bd6d0e038bea71d30d808c7d98cb3bf7268a95bf5081b65");
// }

// //--------------------------------------------------------------
// static bool isMintTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885");
// }

// //--------------------------------------------------------------
// static bool isBurnTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5");
// }

// //--------------------------------------------------------------
// static bool isStakedTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x9e71bc8eea02a63969f509818f2dafb9254532904319f9dbda79b67bd34a5f3d");
// }

//--------------------------------------------------------------
bool CLedgerManager::getTransfers(const CTransaction& trans) {
    timestamp_t ts = str_2_Ts(trans.Format("[{TIMESTAMP}]"));
    string_q encoding = trans.input.substr(0, 10);
    time_q dt = ts_2_Date(ts);

    CTransferArray tmp;
    for (auto& log : trans.receipt.logs) {
        CAccountName tokenName;
        findToken(log.address, tokenName);
        if (tokenName.address.empty()) {
            tokenName.address = log.address;
            tokenName.petname = addr_2_Petname(tokenName.address, '-');
        }

        CTransfer transfer;
        if (isTokenTransfer(log.topics[0]) && log.topics.size() > 2) {
            transfer.sender = topic_2_Addr(log.topics[1]);
            transfer.recipient = topic_2_Addr(log.topics[2]);
            transfer.amount = str_2_Wei(log.data);
            // } else if (log.topics.size() > 1) {
            //     if (isWithdrawalTransfer(log.topics[0])) {
            //         transfer.sender = topic_2_Addr(log.topics[1]);
            //         transfer.recipient = topic_2_Addr(trans.from);
            //         transfer.amount = str_2_Wei(log.data);
            //     } else if (isDepositTransfer(log.topics[0]) || isMintTransfer(log.topics[0]) ||
            //                isBurnTransfer(log.topics[0]) || isStakedTransfer(log.topics[0])) {
            //         transfer.sender = topic_2_Addr(trans.from);
            //         transfer.recipient = topic_2_Addr(log.topics[1]);
            //         transfer.amount = str_2_Wei(log.data);
            //     }
        }

        if (transfer.amount != 0 && (transfer.sender == accountedFor || transfer.recipient == accountedFor)) {
            transfer.blockNumber = trans.blockNumber;
            transfer.transactionIndex = trans.transactionIndex;
            transfer.logIndex = log.logIndex;
            transfer.timestamp = ts;
            transfer.date = dt;
            transfer.transactionHash = trans.hash;
            transfer.encoding = encoding;
            transfer.assetAddr = log.address;
            transfer.log = (CLogEntry*)&log;  // TODO: for debugging only, can be removed

            transfer.assetSymbol = tokenName.symbol;
            if (transfer.assetSymbol.empty()) {
                transfer.assetSymbol = getTokenSymbol(transfer.assetAddr, trans.blockNumber);
                if (transfer.assetSymbol.empty()) {
                    transfer.assetSymbol = transfer.assetAddr.substr(0, 4);
                }
            }

            transfer.decimals = tokenName.decimals;
            if (transfer.decimals == 0) {
                transfer.decimals = getTokenDecimals(transfer.assetAddr, trans.blockNumber);
                if (transfer.decimals == 0) {
                    transfer.decimals = 18;
                }
            }

            tmp.push_back(transfer);
        }
    }

    sort(tmp.begin(), tmp.end());

    transfers.clear();
    transfers.reserve(tmp.size() + 2);

    CTransfer transfer;
    transfer.sender = trans.from;
    transfer.recipient = trans.to;
    transfer.blockNumber = trans.blockNumber;
    transfer.transactionIndex = trans.transactionIndex;
    transfer.logIndex = 0;
    transfer.timestamp = str_2_Ts(trans.Format("[{TIMESTAMP}]"));
    transfer.date = ts_2_Date(transfer.timestamp);
    transfer.transactionHash = trans.hash;
    transfer.encoding = trans.input.substr(0, 10);
    transfer.assetAddr = FAKE_ETH_ADDRESS;
    transfer.log = nullptr;
    transfer.assetSymbol = expContext().asEther ? "ETH" : "WEI";
    transfer.decimals = 18;
    transfer.amount = trans.value;

    transfers.push_back(transfer);
    transfers.insert(transfers.end(), tmp.begin(), tmp.end());

    return transfers.size() > 0;
}

//--------------------------------------------------------------------------------
void CLedgerManager::getPrevNext(size_t index, const CTransaction& trans) {
    nextBlock = NOPOS;
    if (index < appArray.size() - 1) {
        nextBlock = appArray[index + 1].blk;
    }
}

}  // namespace qblocks
