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
#include "statementmanager.h"

namespace qblocks {

//--------------------------------------------------------------
bool CStatementManager::getStatements(CTransaction& trans) {
    if (trans.readReconsFromCache(accountedFor)) {
        for (auto& statement : trans.statements) {
            string_q key = statementKey(statement.accountedFor, statement.assetAddr);
            previousBalances[key] = statement;
        }
        return !shouldQuit();
    }

    prevBal = 0;
    if (prevBlock > 0) {
        prevBal = getBalanceAt(accountedFor, prevBlock);
    }

    getTransfers(trans);
    for (auto transfer : transfers) {
        if (assetFilter.size() == 0 || assetFilter[transfer.assetAddr]) {
            string tokenKey = statementKey(accountedFor, transfer.assetAddr);
            if (previousBalances[tokenKey] == CPreviousBalance()) {
                CReconciliation prevStatement(accountedFor, transfer.assetAddr, &trans);
                if (trans.blockNumber > 0) {
                    prevStatement.blockNumber = prevBlock;
                    prevStatement.endBal =
                        prevBlock == 0 ? 0 : getTokenBalanceAt(transfer.assetAddr, accountedFor, prevBlock);
                }
                previousBalances[tokenKey] = prevStatement;
            }

            bool isTop = transfer.type == TT_TOP;
            CReconciliation statement(accountedFor, transfer.assetAddr, &trans);
            statement.assetSymbol = transfer.assetSymbol;
            statement.decimals = transfer.decimals;
            if (!statement.reconcileFlows(isTop, transfer)) {
                statement.reconcileFlows_traces(isTop);
            }
            statement.reconcileBalances(isTop, transfer, prevBlock, nextBlock, previousBalances[tokenKey].balance);
            statement.reconcileLabel(prevBlock, nextBlock);
            if (statement.amountNet() != 0) {
                trans.statements.push_back(statement);
                previousBalances[tokenKey] = statement;
            }
        }
    }

    if (which & REC_TRACES) {
        bool isTop = true;
        CTraceArray traceArray;
        getTraces(traceArray, trans.hash, &trans);
        for (auto trace : traceArray) {
            CReconciliation statement(accountedFor, FAKE_ETH_ADDRESS, &trans);
            statement.reconcileFlows_traces(isTop);
            CTransfer transfer;
            statement.reconcileBalances(isTop, transfer, prevBlock, nextBlock, prevBal);
            statement.reconcileLabel(prevBlock, nextBlock);
            statement.assetSymbol = expContext().asEther ? "ETH" : "WEI";
            statement.encoding = trace.action.input.size() >= 10 ? trace.action.input.substr(0, 10) : "";
            statement.signature = trans.Format("[{COMPRESSEDTRACE}]");
            if (statement.reconciliationType == "regular") {
                statement.reconciliationType = "by-trace";
            }
            if (statement.amountNet() != 0) {
                trans.statements.push_back(statement);
            }
        }
    }

    for (size_t s = 0; s < trans.statements.size(); s++) {
        CReconciliation* st = &trans.statements[s];
        st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
    }

    return !shouldQuit();
}

//--------------------------------------------------------------
bool CStatementManager::getTransfers(const CTransaction& trans) {
    transfers.clear();

    CTransfer transfer;
    transfer.assetAddr = "";  // so it sorts right
    transfer.assetSymbol = expContext().asEther ? "ETH" : "WEI";
    transfer.decimals = 18;
    transfer.blockNumber = trans.blockNumber;
    transfer.transactionIndex = trans.transactionIndex;
    transfer.logIndex = 0;
    transfer.type = TT_TOP;
    transfers.push_back(transfer);

    for (auto& log : trans.receipt.logs) {
        CAccountName tokenName;
        findToken(log.address, tokenName);
        if (tokenName.address.empty()) {
            tokenName.address = log.address;
            tokenName.petname = addr_2_Petname(tokenName.address, '-');
        }

        if (isTokenTransfer(log.topics[0]) && log.topics.size() > 2) {
            CTransfer transfer;
            transfer.sender = topic_2_Addr(log.topics[1]);
            transfer.recipient = topic_2_Addr(log.topics[2]);
            if (transfer.sender != accountedFor && transfer.recipient != accountedFor) {
                continue;
            }

            transfer.log = (CLogEntry*)&log;  // TODO: for debugging only, can be removed

            transfer.assetAddr = log.address;

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

            if (transfer.sender != accountedFor && transfer.recipient != accountedFor)
                continue;

            transfer.amount = str_2_Wei(log.data);
            if (transfer.amount == 0) {
                continue;
            }

            transfer.blockNumber = trans.blockNumber;
            transfer.transactionIndex = trans.transactionIndex;
            transfer.logIndex = log.logIndex;
            transfer.type = TT_LOG;
            transfer.timestamp = str_2_Ts(trans.Format("[{TIMESTAMP}]"));
            transfer.date = ts_2_Date(transfer.timestamp);
            transfer.transactionHash = trans.hash;
            transfer.encoding = trans.input.substr(0, 10);

            transfers.push_back(transfer);
        }
    }

    sort(transfers.begin(), transfers.end());
    transfers[0].assetAddr = FAKE_ETH_ADDRESS;  // so it sorts right
    return transfers.size() > 0;
}

}  // namespace qblocks
