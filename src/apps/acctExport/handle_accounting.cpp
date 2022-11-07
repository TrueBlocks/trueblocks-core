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
#include "options.h"

extern string_q getReconcilationPath(const address_t& address, const CTransaction* pT);
extern string_q statementKey(const address_t& accountedFor, const address_t& assetAddr);
//-----------------------------------------------------------------------
bool COptions::process_statements(CTraverser* trav) {
    trav->trans.statements.clear();

    // If we can get the reconciliations from the cache, do so...
    if (readReconsFromCache(trav)) {
        return !shouldQuit();
    }

    trav->searchOp = RECONCILE;

    string_q ethKey = statementKey(accountedFor.address, "");

    blknum_t prevAppBlk = 0;
    if (trav->index > 0) {
        prevAppBlk = monApps[trav->index - 1].blk;
    } else {
        if (trav->trans.blockNumber > 0) {
            prevAppBlk = trav->trans.blockNumber - 1;
        }
    }

    if (prevStatements[ethKey].assetAddr.empty()) {
        CReconciliation prevStatement(accountedFor.address, prevAppBlk, NOPOS, trav->trans.timestamp, &trav->trans);
        // Balance and price before genesis block is always zero
        if (prevAppBlk > 0) {
            prevStatement.endBal = getBalanceAt(accountedFor.address, prevAppBlk);
            prevStatement.spotPrice = getPriceInUsd(FAKE_ETH_ADDRESS, prevStatement.priceSource, prevAppBlk);
        }
        prevStatements[ethKey] = prevStatement;
    }

    CReconciliation ethStatement(accountedFor.address, &trav->trans);
    ethStatement.nextAppBlk = trav->index < monApps.size() - 1 ? monApps[trav->index + 1].blk : NOPOS;
    ethStatement.reconcileEth(prevStatements[ethKey]);
    ethStatement.spotPrice = getPriceInUsd(FAKE_ETH_ADDRESS, ethStatement.priceSource, trav->trans.blockNumber);
    if (ethStatement.amountNet() != 0) {
        trav->trans.statements.push_back(ethStatement);
    }
    prevStatements[ethKey] = ethStatement;

    CTransferArray transfers;
    if (trav->trans.getTransfers(transfers, accountedFor.address)) {
        for (auto transfer : transfers) {
            if (assetFilter.size() > 0 && !assetFilter[transfer.assetAddr]) {
                continue;
            }
            CReconciliation tokStatement(accountedFor.address, &trav->trans);
            tokStatement.assetAddr = transfer.assetAddr;
            tokStatement.decimals = transfer.decimals;
            tokStatement.assetSymbol = transfer.assetSymbol;
            tokStatement.sender = transfer.sender;
            tokStatement.recipient = transfer.recipient;

            string tokenKey = statementKey(accountedFor.address, tokStatement.assetAddr);
            if (prevStatements[tokenKey].assetAddr.empty()) {
                CReconciliation pBal = tokStatement;
                pBal.pTransaction = &trav->trans;
                pBal.blockNumber = trav->trans.blockNumber == 0 ? 0 : trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceAt(tokStatement.assetAddr, accountedFor.address, pBal.blockNumber);
                pBal.spotPrice = getPriceInUsd(tokStatement.assetAddr, pBal.priceSource, pBal.blockNumber);
                prevStatements[tokenKey] = pBal;
            }

            tokStatement.prevAppBlk = prevStatements[tokenKey].blockNumber;
            tokStatement.prevBal = prevStatements[tokenKey].endBal;
            // TODO: Note that this doesn't really query this token's balance at its last appearance,
            // TODO: it just picks up the previous balance
            tokStatement.begBal = prevStatements[tokenKey].endBal;
            tokStatement.endBal =
                getTokenBalanceAt(tokStatement.assetAddr, accountedFor.address, trav->trans.blockNumber);

            if (tokStatement.begBal != tokStatement.endBal) {
                if (tokStatement.begBal > tokStatement.endBal) {
                    tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
                } else {
                    tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
                }
                tokStatement.reconciliationType = "token";
                tokStatement.spotPrice =
                    getPriceInUsd(tokStatement.assetAddr, tokStatement.priceSource, trav->trans.blockNumber);
                trav->trans.statements.push_back(tokStatement);
                prevStatements[tokenKey] = tokStatement;
            }
        }
    }

    cacheIfReconciled(trav);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
string_q getReconcilationPath(const address_t& address, const CTransaction* pT) {
    string_q path = getPathToBinaryCache(CT_RECONS, address, pT->blockNumber, pT->transactionIndex);
    establishFolder(path);
    return path;
}

//-----------------------------------------------------------------------
bool isEtherAddr(const address_t& addr) {
    return toLower(addr) == FAKE_ETH_ADDRESS;
}

//-----------------------------------------------------------------------
// For each assset, we keep the last available balances in a map. When reconciling an
// asset for the current transaction, we need the asset's previous balance. We index
// into the previous balances using the accountedFor address and the asset's address.
string_q statementKey(const address_t& accountedFor, const address_t& assetAddr) {
    if (isZeroAddr(assetAddr) || isEtherAddr(assetAddr)) {
        return toLower(accountedFor + "-" + "_eth");
    }
    return toLower(accountedFor + "-" + assetAddr);
}

//-----------------------------------------------------------------------
bool COptions::readReconsFromCache(CTraverser* trav) {
    if (isTestMode()) {
        return false;
    }

    string_q path = getReconcilationPath(accountedFor.address, &trav->trans);
    if (!fileExists(path)) {
        return false;
    }

    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archive >> trav->trans.statements;
        archive.Release();
        for (auto& statement : trav->trans.statements) {
            // If this is an older versioned file, act as if it doesn't exist so it gets upgraded
            if (statement.accountedFor.empty()) {
                LOG_WARN("Cache for statements is back level. Updating....");
                return false;
            }

            // Freshen in case user has changed the names database since putting the statement in the cache
            CAccountName tokenName;
            if (findToken(statement.assetAddr, tokenName)) {
                statement.assetSymbol = tokenName.symbol;
                statement.decimals = tokenName.decimals;
            }

            statement.pTransaction = &trav->trans;

            string_q key = statementKey(accountedFor.address, statement.assetAddr);
            prevStatements[key] = statement;
        }
        return !shouldQuit();
    }

    return false;
}

//-----------------------------------------------------------------------
bool COptions::isReconciled(CTraverser* trav, CReconciliation& which) const {
    for (auto statement : trav->trans.statements) {
        if (!statement.reconciled()) {
            which = statement;
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------
void COptions::cacheIfReconciled(CTraverser* trav) const {
    if (isTestMode())
        return;

    CReconciliation whichFailed;
    if (!isReconciled(trav, whichFailed)) {
        if (whichFailed.assetSymbol == "ETH") {
            LOG_WARN("Transaction at ", trav->trans.blockNumber, ".", padNum4(trav->trans.transactionIndex), " (",
                     trav->trans.hash, ") did not reconcile for account ", accountedFor.address);
        } else {
            LOG_WARN("Token transfer at ", trav->trans.blockNumber, ".", padNum4(trav->trans.transactionIndex), " (",
                     trav->trans.hash, ") did not reconcile for account ", accountedFor.address);
        }
        return;
    }

    string_q path = getReconcilationPath(accountedFor.address, &trav->trans);

    lockSection();

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        LOG4("Writing to cache for ", path);
        archive << trav->trans.statements;
        archive.Release();
    }

    unlockSection();
}

//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (fourByteFilter(trav->trans.input, opt)) {
        if (opt->relevant) {
            for (auto& log : trav->trans.receipt.logs) {
                log.m_showing = opt->isRelevant(log);
            }
        }

        if (opt->accounting)
            opt->process_statements(trav);

        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    return prog_Log(trav, data);
}

//-----------------------------------------------------------------------
bool acct_PreFunc(CTraverser* trav, void* data) {
    return true;
}
