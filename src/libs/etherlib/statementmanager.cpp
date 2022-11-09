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
    prevBal = 0;
    if (prevBlock > 0) {
        prevBal = getBalanceAt(accountedFor, prevBlock);
    }

    if (trans.readReconsFromCache(accountedFor)) {
        for (auto& statement : trans.statements) {
            string_q key = statementKey(statement.accountedFor, statement.assetAddr);
            previousBalances[key] = statement;
        }
        return !shouldQuit();
    }

    if (which & REC_TOP) {
        bool isTop = true;
        if (getTransfers(trans, isTop)) {
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

                    CReconciliation statement(accountedFor, transfer.assetAddr, &trans);
                    statement.assetSymbol = transfer.assetSymbol;
                    statement.decimals = transfer.decimals;
                    if (!statement.reconcileFlows(isTop, transfer)) {
                        statement.reconcileFlows_traces(isTop);
                    }
                    statement.reconcileBalances(isTop, prevBlock, nextBlock, previousBalances[tokenKey].balance);
                    statement.reconcileLabel(isTop, prevBlock, nextBlock);
                    if (statement.amountNet() != 0) {
                        trans.statements.push_back(statement);
                        previousBalances[tokenKey] = statement;
                    }
                }
            }
        }
    }

    if (which & REC_TOKENS) {
        bool isTop = false;
        if (getTransfers(trans, isTop)) {
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

                    CReconciliation statement(accountedFor, transfer.assetAddr, &trans);
                    statement.assetSymbol = transfer.assetSymbol;
                    statement.decimals = transfer.decimals;
                    if (!statement.reconcileFlows(isTop, transfer)) {
                        statement.reconcileFlows_traces(isTop);
                    }
                    statement.reconcileBalances(isTop, prevBlock, nextBlock, previousBalances[tokenKey].balance);
                    statement.reconcileLabel(isTop, prevBlock, nextBlock);
                    if (statement.amountNet() != 0) {
                        trans.statements.push_back(statement);
                        previousBalances[tokenKey] = statement;
                    }
                }
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
            statement.reconcileBalances(isTop, prevBlock, nextBlock, prevBal);
            statement.reconcileLabel(isTop, prevBlock, nextBlock);
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

    if (forExport) {
        trans.cacheIfReconciled(accountedFor);
        return !shouldQuit();
    }

    return trans.statements.size() > 0 && !shouldQuit();
}

//--------------------------------------------------------------
bool CStatementManager::getTransfers(const CTransaction& trans, bool isTop) {
    transfers.clear();
    if (isTop) {
        CTransfer transfer;
        transfer.assetAddr = FAKE_ETH_ADDRESS;
        transfer.assetSymbol = expContext().asEther ? "ETH" : "WEI";
        transfer.decimals = 18;
        transfers.push_back(transfer);
        return true;
    } else {
        return trans.getTransfers(transfers, accountedFor);
    }
}

}  // namespace qblocks
