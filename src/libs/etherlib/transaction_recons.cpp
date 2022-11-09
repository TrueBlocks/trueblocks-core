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
#include "transaction.h"
#include "etherlib.h"

namespace qblocks {

//--------------------------------------------------------------
bool CStatementManager::getStatements(CTransaction& trans, const address_t& accountedFor) {
    if (trans.readReconsFromCache(accountedFor)) {
        for (auto& statement : trans.statements) {
            string_q key = statementKey(statement.accountedFor, statement.assetAddr);
            previousBalances[key] = statement;
        }
        return !shouldQuit();
    }

    if (which & REC_TOP) {
        CTransfer transfer;
        transfer.assetAddr = FAKE_ETH_ADDRESS;
        transfer.assetSymbol = expContext().asEther ? "ETH" : "WEI";
        transfer.decimals = 18;

        string tokenKey = statementKey(accountedFor, transfer.assetAddr);
        if (previousBalances[tokenKey] == CPreviousBalance()) {
            CReconciliation prevStatement(accountedFor, transfer.assetAddr, &trans);
            prevStatement.blockNumber = prevBlock;
            prevStatement.endBal = prevBlock == 0 ? 0 : getTokenBalanceAt(transfer.assetAddr, accountedFor, prevBlock);
            previousBalances[tokenKey] = prevStatement;
        }

        CReconciliation statement(accountedFor, transfer.assetAddr, &trans);
        statement.assetSymbol = transfer.assetSymbol;
        statement.decimals = transfer.decimals;
        if (!statement.reconcileFlows()) {
            statement.reconcileFlows_traces();
        }
        statement.reconcileBalances(prevBlock, nextBlock, previousBalances[tokenKey].balance);
        statement.reconcileLabel(prevBlock, nextBlock);

        if (!forExport) {
            if (statement.reconciliationType == "regular") {
                statement.reconciliationType = "by-tx";
            }
            statement.encoding = trans.input.size() >= 10 ? trans.input.substr(0, 10) : "";
            statement.signature = trans.Format("[{COMPRESSEDTX}]");
        }

        if (statement.amountNet() != 0) {
            trans.statements.push_back(statement);
        }

        previousBalances[tokenKey] = statement;
    }

    CTransferArray transfers;
    if (which & REC_TOKENS) {
        if (trans.getTransfers(transfers, accountedFor)) {
            for (auto transfer : transfers) {
                if (assetFilter.size() > 0 && !assetFilter[transfer.assetAddr]) {
                    continue;
                }

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

                CReconciliation aStatement(accountedFor, transfer.assetAddr, &trans);
                aStatement.assetSymbol = transfer.assetSymbol;
                aStatement.decimals = transfer.decimals;
                aStatement.sender = transfer.sender;
                aStatement.recipient = transfer.recipient;
                aStatement.prevAppBlk = previousBalances[tokenKey].blockNumber;
                aStatement.prevBal = previousBalances[tokenKey].balance;
                aStatement.begBal = previousBalances[tokenKey].balance;
                aStatement.endBal =
                    getTokenBalanceAt(aStatement.assetAddr, aStatement.accountedFor, aStatement.blockNumber);

                if (aStatement.begBal != aStatement.endBal) {
                    if (aStatement.begBal > aStatement.endBal) {
                        aStatement.amountOut = (aStatement.begBal - aStatement.endBal);
                    } else {
                        aStatement.amountIn = (aStatement.endBal - aStatement.begBal);
                    }
                    aStatement.reconciliationType = "token";
                    trans.statements.push_back(aStatement);
                    previousBalances[tokenKey] = aStatement;
                }
            }
        }
    }

    if (which & REC_TRACES) {
        CTraceArray traceArray;
        getTraces(traceArray, trans.hash, &trans);
        for (auto trace : traceArray) {
            CReconciliation statement(accountedFor, FAKE_ETH_ADDRESS, &trans);
            statement.reconcileFlows_traces();
            statement.reconcileBalances(prevBlock, nextBlock, prevBal);
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

    if (forExport) {
        trans.cacheIfReconciled(accountedFor);
        return !shouldQuit();
    }

    return trans.statements.size() > 0 && !shouldQuit();
}

//-------------------------------------------------------------------------
bool CTransaction::getTransfers(CTransferArray& transfers, const address_t& accountedFor) const {
    for (auto log : receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(log.address, tokenName);
        if (tokenName.address.empty()) {
            tokenName.address = log.address;
            tokenName.petname = addr_2_Petname(tokenName.address, '-');
        }

        if (isToken || (log.topics.size() > 2 && isTokenTransfer(log.topics[0]))) {
            CTransfer transfer;

            transfer.assetAddr = log.address;

            transfer.assetSymbol = tokenName.symbol;
            if (transfer.assetSymbol.empty()) {
                transfer.assetSymbol = getTokenSymbol(transfer.assetAddr, blockNumber);
                if (transfer.assetSymbol.empty()) {
                    transfer.assetSymbol = transfer.assetAddr.substr(0, 4);
                }
            }

            transfer.decimals = tokenName.decimals;
            if (transfer.decimals == 0) {
                transfer.decimals = getTokenDecimals(transfer.assetAddr, blockNumber);
                if (transfer.decimals == 0) {
                    transfer.decimals = 18;
                }
            }

            transfer.sender = topic_2_Addr(log.topics[1]);
            transfer.recipient = topic_2_Addr(log.topics[2]);
            if (transfer.sender != accountedFor && transfer.recipient != accountedFor)
                continue;

            transfer.amount = str_2_Wei(log.data);
            if (transfer.amount == 0) {
                continue;
            }

            transfer.blockNumber = blockNumber;
            transfer.transactionIndex = transactionIndex;
            transfer.logIndex = log.logIndex;
            transfer.timestamp = str_2_Ts(Format("[{TIMESTAMP}]"));
            transfer.date = ts_2_Date(transfer.timestamp);
            transfer.transactionHash = hash;
            transfer.encoding = input.substr(0, 10);

            transfers.push_back(transfer);
        }
    }

    sort(transfers.begin(), transfers.end());
    return transfers.size() > 0;
}

//-----------------------------------------------------------------------
string_q CTransaction::getReconcilationPath(const address_t& address) const {
    string_q path = getPathToBinaryCache(CT_RECONS, address, blockNumber, transactionIndex);
    establishFolder(path);
    return path;
}

//-----------------------------------------------------------------------
bool CTransaction::readReconsFromCache(const address_t& accountedFor) {
    statements.clear();
    if (isTestMode()) {
        return false;
    }

    string_q path = getReconcilationPath(accountedFor);
    if (!fileExists(path)) {
        return false;
    }

    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archive >> statements;
        archive.Release();
        for (auto& statement : statements) {
            if (statement.accountedFor.empty()) {
                // This is an old version of the reconciliation, delete it and return false
                LOG_WARN("Back-level cache for statements found. Removing....");
                ::remove(path.c_str());
                return false;
            }

            // Freshen in case user has changed the names database since putting the statement in the cache
            CAccountName tokenName;
            if (findToken(statement.assetAddr, tokenName)) {
                statement.assetSymbol = tokenName.symbol;
                statement.decimals = tokenName.decimals;
            }

            statement.pTransaction = this;
        }
        return !shouldQuit();
    }

    return false;
}

//-----------------------------------------------------------------------
void CTransaction::cacheIfReconciled(const address_t& accountedFor) const {
    if (isTestMode())
        return;

    CReconciliation whichFailed;
    if (!isReconciled(whichFailed)) {
        if (whichFailed.assetSymbol == "ETH") {
            LOG_WARN("Transaction at ", blockNumber, ".", padNum4(transactionIndex), " (", hash,
                     ") did not reconcile for account ", accountedFor);
        } else {
            LOG_WARN("Token transfer at ", blockNumber, ".", padNum4(transactionIndex), " (", hash,
                     ") did not reconcile for account ", accountedFor);
        }
        return;
    }

    string_q path = getReconcilationPath(accountedFor);

    lockSection();

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        LOG4("Writing to cache for ", path);
        archive << statements;
        archive.Release();
    }

    unlockSection();
}

//-----------------------------------------------------------------------
bool CTransaction::isReconciled(CReconciliation& which) const {
    for (auto statement : statements) {
        if (!statement.reconciled()) {
            which = statement;
            return false;
        }
    }
    return true;
}

}  // namespace qblocks
