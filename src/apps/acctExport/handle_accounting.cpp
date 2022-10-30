// TODO: Reverse mode
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
bool COptions::process_reconciliation(CTraverser* trav) {
    trav->trans.statements.clear();

    // If we can get the reconciliations from the cache, do so...
    // if (readReconsFromCache(trav)) {
    //     return !shouldQuit();
    // }

    trav->searchOp = RECONCILE;

    string_q ethKey = statementKey(accountedFor.address, "");

    // The block of the previous appearance, unless we're at the start of the list, in which case
    // it's one less than the current block, unless we're at the zero block, then zero
    blknum_t prevAppBlk = 0;
    if (trav->index > 0) {
        prevAppBlk = monApps[trav->index - 1].blk;
    } else {
        if (trav->trans.blockNumber > 0) {
            prevAppBlk = trav->trans.blockNumber - 1;
        }
    }

    if (prevStatements[ethKey].assetAddr.empty()) {
        // TODO(tjayrush): Incorrect code follows
        // This code is wrong. We ask for the balance at the current block minus one, but we should ask
        // at the previous block in this address's appearance list. When we're called with first_record
        // or start_block not zero we don't have this (since we only load those appearances we're asked
        // for) To fix this, we need to be able to get the previous appearance's block. Note, we
        // only need the balance for the previous reconcilation, so using NOPOS for transactionIndex is okay.
        CReconciliation prevStatement(accountedFor.address, prevAppBlk, NOPOS, trav->trans.timestamp, &trav->trans);
        prevStatement.endBal = getBalanceAt(accountedFor.address, prevAppBlk);
        prevStatement.spotPrice = getPriceInUsd(prevAppBlk, prevStatement.priceSource);
        prevStatements[ethKey] = prevStatement;
    }

    CReconciliation ethStatement(accountedFor.address, &trav->trans);
    blknum_t nextAppBlk = trav->index < monApps.size() - 1 ? monApps[trav->index + 1].blk : NOPOS;
    ethStatement.reconcileEth(prevStatements[ethKey], nextAppBlk, &trav->trans, accountedFor);
    ethStatement.spotPrice = getPriceInUsd(trav->trans.blockNumber, ethStatement.priceSource);
    trav->trans.statements.push_back(ethStatement);
    prevStatements[ethKey] = ethStatement;

    CTransferArray transfers;
    CAccountNameMap tokenList;
    if (getListOfTransfers(transfers, tokenList, trav)) {
        for (auto transfer : transfers) {
            cerr << transfer << endl;
        }

        for (auto item : tokenList) {
            CReconciliation tokStatement(accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex,
                                         trav->trans.timestamp, &trav->trans);

            CAccountName tokenName = item.second;
            tokStatement.assetAddr = tokenName.address;
            tokStatement.decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;
            tokStatement.assetSymbol = tokenName.symbol;
            if (tokStatement.assetSymbol.empty()) {
                tokStatement.assetSymbol = getTokenSymbol(tokenName.address, tokStatement.blockNumber);
                if (contains(tokStatement.assetSymbol, "reverted"))
                    tokStatement.assetSymbol = "";
            }
            if (tokStatement.assetSymbol.empty())
                tokStatement.assetSymbol = tokenName.address.substr(0, 4);

            string tokenKey = accountedFor.address + "_" + tokenName.address;
            if (prevStatements[tokenKey].assetAddr.empty()) {
                CReconciliation pBal = tokStatement;
                pBal.sender = trav->trans.from;
                pBal.pTransaction = &trav->trans;
                pBal.blockNumber = trav->trans.blockNumber == 0 ? 0 : trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceOf2(tokenName.address, accountedFor.address, pBal.blockNumber);
                pBal.spotPrice = getPriceInUsd(pBal.blockNumber, pBal.priceSource, tokenName.address);
                prevStatements[tokenKey] = pBal;
            }

            tokStatement.prevAppBlk = prevStatements[tokenKey].blockNumber;
            tokStatement.prevBal = prevStatements[tokenKey].endBal;
            tokStatement.begBal =
                trav->trans.blockNumber == 0
                    ? 0
                    : getTokenBalanceOf2(tokenName.address, accountedFor.address, trav->trans.blockNumber - 1);
            tokStatement.endBal = getTokenBalanceOf2(tokenName.address, accountedFor.address, trav->trans.blockNumber);
            if (tokStatement.begBal > tokStatement.endBal) {
                tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
            } else {
                tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
            }
            tokStatement.reconciliationType = "token";
            if (tokStatement.amountNet_internal() != 0) {
                tokStatement.sender = trav->trans.from;
                tokStatement.spotPrice =
                    getPriceInUsd(trav->trans.blockNumber, tokStatement.priceSource, tokenName.address);
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
bool COptions::getListOfTransfers(CTransferArray& transfers, CAccountNameMap& tokenList, const CTraverser* trav) {
    for (auto log : trav->trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(log.address, tokenName);
        if (tokenName.address.empty()) {
            tokenName.address = log.address;
            tokenName.petname = addr_2_Petname(tokenName.address, '-');
        }
        if (isToken || (log.topics.size() > 0 && isTokenRelated(log.topics[0]))) {
            tokenList[log.address] = tokenName;
            bool passes = assetFilter.size() == 0 || assetFilter[tokenName.address];
            if (!passes)
                continue;
            CTransfer transfer;
            transfer.blockNumber = trav->trans.blockNumber;
            transfer.transactionIndex = trav->trans.transactionIndex;
            transfer.logIndex = log.logIndex;
            transfer.timestamp = trav->trans.pBlock ? trav->trans.pBlock->timestamp : trav->trans.timestamp;
            transfer.date = ts_2_Date(transfer.timestamp);
            transfer.transactionHash = trav->trans.hash;
            transfer.sender = log.topics.size() > 1 ? log.topics[1] : "";
            transfer.recipient = log.topics.size() > 2 ? log.topics[2] : "";
            transfer.amount = str_2_Wei(log.data);
            transfer.topic0 = log.topics.size() > 0 ? log.topics[0] : "";
            transfer.topic1 = log.topics.size() > 1 ? log.topics[1] : "";
            transfer.topic2 = log.topics.size() > 2 ? log.topics[2] : "";
            transfer.topic3 = log.topics.size() > 3 ? log.topics[3] : "";
            transfer.data = log.data;
            transfer.encoding = trav->trans.input.substr(0, 10);
            transfer.assetAddr = log.address;
            transfer.assetSymbol = tokenName.symbol;
            transfers.push_back(transfer);
        }
    }
    return tokenList.size() > 0;
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
            // Migrations will have made sure any reconciliation statements are at least this version
            ASSERT(statement.m_scheme >= getVersionNum(0, 42, 6));

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
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------
bool COptions::isReconciled(CTraverser* trav, CReconciliation& which) const {
    for (auto recon : trav->trans.statements) {
        if (!recon.reconciled_internal()) {
            which = recon;
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
        if (opt->accounting)
            opt->process_reconciliation(trav);

        if (opt->relevant) {
            for (auto& log : trav->trans.receipt.logs) {
                log.m_showing = opt->isRelevant(log);
            }
        }

        if (opt->statements) {
            for (auto recon : trav->trans.statements) {
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << recon.Format() << endl;
                opt->firstOut = false;
            }
        } else {
            cout << ((isJson() && !opt->firstOut) ? ", " : "");
            cout << trav->trans;
            opt->firstOut = false;
        }
    }

    return prog_Log(trav, data);
}

//-----------------------------------------------------------------------
bool acct_PreFunc(CTraverser* trav, void* data) {
    return true;
}
