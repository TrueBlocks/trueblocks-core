// TODO: Reverse mode
// TODO: collapse eth reconciliation into token reconconcilation by adding a CTransfer
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

// #define NEW_CODE

extern string_q getReconcilationPath(const address_t& address, const CTransaction* pT);
extern string_q statementKey(const address_t& accountedFor, const address_t& assetAddr);
//-----------------------------------------------------------------------
bool COptions::process_reconciliation(CTraverser* trav) {
    trav->trans.statements.clear();

    // If we can get the reconciliations from the cache, do so...
    if (readReconsFromCache(trav)) {
        return !shouldQuit();
    }

    trav->searchOp = RECONCILE;

    string_q ethKey = statementKey(accountedFor.address, "");

#ifdef NEW_CODE
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
    ethStatement.nextAppBlk = trav->index < monApps.size() - 1 ? monApps[trav->index + 1].blk : NOPOS;
    ethStatement.reconcileEth(prevStatements[ethKey]);
    ethStatement.spotPrice = getPriceInUsd(trav->trans.blockNumber, ethStatement.priceSource);
    if (ethStatement.amountNet_internal() != 0) {
        trav->trans.statements.push_back(ethStatement);
    }
    prevStatements[ethKey] = ethStatement;

    CTransferArray transfers;
    CAccountNameMap tokenList;
    if (getTokenTransfers(transfers, tokenList, accountedFor.address, trav)) {
        for (auto transfer : transfers) {
            if (assetFilter.size() > 0 && !assetFilter[transfer.assetAddr]) {
                continue;
            }

            CReconciliation tokStatement(accountedFor.address, &trav->trans);
            tokStatement.assetAddr = transfer.assetAddr;
            tokStatement.decimals = transfer.decimals;
            tokStatement.assetSymbol = transfer.assetSymbol;
            string tokenKey = statementKey(accountedFor.address, tokStatement.assetAddr);

            if (prevStatements[tokenKey].assetAddr.empty()) {
                CReconciliation pBal = tokStatement;
                // pBal.sender = transfer.sender;
                // pBal.recipient = transfer.recipient;
                // pBal.pTransaction = &trav->trans;
                pBal.blockNumber = trav->trans.blockNumber == 0 ? 0 : trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceOf2(tokStatement.assetAddr, accountedFor.address, pBal.blockNumber);
                pBal.spotPrice = getPriceInUsd(pBal.blockNumber, pBal.priceSource, tokStatement.assetAddr);
                prevStatements[tokenKey] = pBal;
            }
            tokStatement.prevAppBlk = prevStatements[tokenKey].blockNumber;
            tokStatement.prevBal = prevStatements[tokenKey].endBal;
            tokStatement.begBal =
                trav->trans.blockNumber == 0
                    ? 0
                    : getTokenBalanceOf2(tokStatement.assetAddr, accountedFor.address, trav->trans.blockNumber - 1);
            tokStatement.endBal =
                getTokenBalanceOf2(tokStatement.assetAddr, accountedFor.address, trav->trans.blockNumber);
            if (tokStatement.begBal > tokStatement.endBal) {
                tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
            } else {
                tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
            }
            tokStatement.reconciliationType = "token";
            if (tokStatement.amountNet_internal() != 0) {
                tokStatement.sender = transfer.sender;
                tokStatement.recipient = transfer.recipient;
                tokStatement.spotPrice =
                    getPriceInUsd(trav->trans.blockNumber, tokStatement.priceSource, tokStatement.assetAddr);
                trav->trans.statements.push_back(tokStatement);
                prevStatements[tokenKey] = tokStatement;
            }
        }
    }
#else
    blknum_t prevAppBlk = 0;
    if (trav->index > 0) {
        prevAppBlk = monApps[trav->index - 1].blk;
    } else {
        if (trav->trans.blockNumber > 0) {
            prevAppBlk = trav->trans.blockNumber - 1;
        }
    }
    blknum_t prevAppTxid = trav->index > 0 ? monApps[trav->index - 1].txid : 0;

    if (prevStatements[ethKey].assetAddr.empty()) {
        CReconciliation pEth(accountedFor.address, prevAppBlk, prevAppTxid, trav->trans.timestamp, &trav->trans);
        // TODO(tjayrush): Incorrect code follows
        // This code is wrong. We ask for the balance at the current block minus one, but we should ask
        // at the previous block in this address's appearance list. When we're called with first_record
        // or start_block not zero we don't have this (since we only load those appearances we're asked
        // for) To fix this, we need to be able to get the previous appearance's block. Note, we
        // only need the balance for the previous reconcilation, so using NOPOS for transactionIndex is okay.
        pEth.endBal =
            trav->trans.blockNumber == 0 ? 0 : getBalanceAt(accountedFor.address, trav->trans.blockNumber - 1);
        pEth.spotPrice = getPriceInUsd(trav->trans.blockNumber - 1, pEth.priceSource);
        prevStatements[ethKey] = pEth;
    }

    CReconciliation eth(accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex,
                        trav->trans.timestamp, &trav->trans);
    eth.reconcileEth(prevStatements[ethKey]);
    eth.spotPrice = getPriceInUsd(trav->trans.blockNumber, eth.priceSource);
    trav->trans.statements.push_back(eth);
    prevStatements[ethKey] = eth;

    CTransferArray transfers;
    CAccountNameMap tokenList;
    if (getTokenTransfers(transfers, tokenList, accountedFor.address, trav)) {
        for (auto item : tokenList) {
            CAccountName tokenName = item.second;

            CReconciliation tokStatement(accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex,
                                         trav->trans.timestamp, &trav->trans);
            tokStatement.assetAddr = tokenName.address;
            tokStatement.assetSymbol = tokenName.symbol;
            if (tokStatement.assetSymbol.empty()) {
                tokStatement.assetSymbol = getTokenSymbol(tokenName.address, tokStatement.blockNumber);
                if (contains(tokStatement.assetSymbol, "reverted"))
                    tokStatement.assetSymbol = "";
            }
            if (tokStatement.assetSymbol.empty())
                tokStatement.assetSymbol = tokenName.address.substr(0, 4);
            tokStatement.decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;

            string psKey = accountedFor.address + "_" + tokenName.address;
            if (prevStatements[psKey].assetAddr.empty()) {
                // first time we've seen this asset, so we need previous balance
                // which is frequently zero but may be non-zero if the command
                // started after the addresses's first transaction
                CReconciliation pBal = tokStatement;
                pBal.pTransaction = &trav->trans;
                pBal.blockNumber = 0;
                if (trav->trans.blockNumber > 0)
                    pBal.blockNumber = trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceOf2(tokenName.address, accountedFor.address, pBal.blockNumber);
                pBal.spotPrice = getPriceInUsd(pBal.blockNumber, pBal.priceSource, tokenName.address);
                prevStatements[psKey] = pBal;
            }

            tokStatement.prevAppBlk = prevStatements[psKey].blockNumber;
            tokStatement.prevBal = prevStatements[psKey].endBal;
            tokStatement.begBal = prevStatements[psKey].endBal;
            tokStatement.endBal = getTokenBalanceOf2(tokenName.address, accountedFor.address, trav->trans.blockNumber);
            if (tokStatement.begBal > tokStatement.endBal) {
                tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
            } else {
                tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
            }
            tokStatement.reconciliationType = "";
            if (tokStatement.amountNet_internal() != 0) {
                tokStatement.spotPrice =
                    getPriceInUsd(trav->trans.blockNumber, tokStatement.priceSource, tokenName.address);
                trav->trans.statements.push_back(tokStatement);
                prevStatements[psKey] = tokStatement;
            }
        }
    }
#endif

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
address_t topic_2_Addr(const string_q& topic) {
    if (topic.length() != 66)
        return "";
    return "0x" + padLeft(topic.substr(26, 66), 40, '0');
}

// //-----------------------------------------------------------------------
// bool COptions::token_list_from_logs(CAccountNameMap& tokenList, const CTraverser* trav) {
//     for (auto log : trav->trans.receipt.logs) {
//         CAccountName tokenName;
//         bool isToken = findToken(log.address, tokenName);
//         if (tokenName.address.empty()) {
//             tokenName.address = log.address;
//             tokenName.petname = addr_2_Petname(tokenName.address, '-');
//         }
//         if (isToken || (log.topics.size() > 0 && isTokenRelated(log.topics[0])))
//             tokenList[log.address] = tokenName;
//     }
//     return tokenList.size() > 0;
// }

//-----------------------------------------------------------------------
bool getTokenTransfers(CTransferArray& transfers, CAccountNameMap& tokenList, const address_t& aF,
                       const CTraverser* trav) {
    // if (trav->trans.value != 0 || trav->trans.from == accountedFor) {
    //     CTransfer transfer;
    //     transfer.blockNumber = trav->trans.blockNumber;
    //     transfer.transactionIndex = trav->trans.transactionIndex;
    //     transfer.logIndex = NOPOS;
    //     transfer.timestamp = trav->trans.pBlock ? trav->trans.pBlock->timestamp : trav->trans.timestamp;
    //     transfer.date = ts_2_Date(transfer.timestamp);
    //     transfer.transactionHash = trav->trans.hash;
    //     transfer.sender = trav->trans.from;
    //     transfer.recipient = trav->trans.to;
    //     transfer.amount = trav->trans.value;
    //     // transfer.topic0 = log.topics.size() > 0 ? log.topics[0] : "";
    //     // transfer.topic1 = log.topics.size() > 1 ? log.topics[1] : "";
    //     // transfer.topic2 = log.topics.size() > 2 ? log.topics[2] : "";
    //     // transfer.topic3 = log.topics.size() > 3 ? log.topics[3] : "";
    //     // transfer.data = log.data;
    //     transfer.encoding = trav->trans.input.substr(0, 10);
    //     transfer.assetAddr = FAKE_ETH_ADDRESS;
    //     transfer.assetSymbol = "ETH";
    //     transfers.push_back(transfer);
    // }

    for (auto log : trav->trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(log.address, tokenName);
        if (tokenName.address.empty()) {
            tokenName.address = log.address;
            tokenName.petname = addr_2_Petname(tokenName.address, '-');
        }

#ifdef NEW_CODE
        if (isToken || (log.topics.size() > 2 && isTokenRelated(log.topics[0]))) {
            CTransfer transfer;

            transfer.assetAddr = log.address;

            transfer.sender = topic_2_Addr(log.topics[1]);
            transfer.recipient = topic_2_Addr(log.topics[2]);
            if (transfer.sender != accountedFor && transfer.recipient != accountedFor)
                continue;

            transfer.amount = str_2_Wei(log.data);
            if (transfer.amount == 0) {
                continue;
            }

            transfer.assetSymbol = tokenName.symbol;
            if (transfer.assetSymbol.empty()) {
                transfer.assetSymbol = getTokenSymbol(transfer.assetAddr, trav->trans.blockNumber);
                if (transfer.assetSymbol.empty()) {
                    transfer.assetSymbol = transfer.assetAddr.substr(0, 4);
                }
            }

            transfer.decimals = tokenName.decimals;
            if (transfer.decimals == 0) {
                transfer.decimals = getTokenDecimals(transfer.assetAddr, trav->trans.blockNumber);
                if (transfer.decimals == 0) {
                    transfer.decimals = 18;
                }
            }

            transfer.blockNumber = trav->trans.blockNumber;
            transfer.transactionIndex = trav->trans.transactionIndex;
            transfer.logIndex = log.logIndex;
            transfer.timestamp = str_2_Ts(trav->trans.Format("[{TIMESTAMP}]"));
            transfer.date = ts_2_Date(transfer.timestamp);
            transfer.transactionHash = trav->trans.hash;
            transfer.encoding = trav->trans.input.substr(0, 10);

            transfer.topic0 = log.topics[0];
            transfer.topic1 = log.topics[1];
            transfer.topic2 = log.topics[2];
            transfer.topic3 = log.topics.size() > 3 ? log.topics[3] : "";
            transfer.data = log.data;

            transfers.push_back(transfer);
        }
    }

    return transfers.size() > 0;
#else
        if (isToken || (log.topics.size() > 0 && isTokenRelated(log.topics[0])))
            tokenList[log.address] = tokenName;
    }
    return tokenList.size() > 0;
#endif
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
        bool backLevel = false;
        for (auto& statement : trav->trans.statements) {
            // If this is an older versioned file, act as if it doesn't exist so it gets upgraded
            if (statement.accountedFor.empty()) {
                LOG_WARN("Cache for reconciliation is back level. Updating....");
                return false;
            }

            // At version 0.11.8, we finally got pricing of reconcilations correct. We didn't
            // want to add an upgrade of reconcilations to the migration, so we do it here
            // but only when the user reads an older file
            backLevel = backLevel || (statement.m_schema < getVersionNum(0, 11, 8));
            CAccountName tokenName;
            if (contains(statement.assetSymbol, "reverted"))
                statement.assetSymbol = "";
            if (statement.assetSymbol != "ETH" && statement.assetSymbol != "WEI" &&
                findToken(statement.assetAddr, tokenName)) {
                // We always freshen these in case user has changed names database
                statement.assetSymbol = tokenName.symbol;
                statement.decimals = tokenName.decimals;
            }

            statement.pTransaction = &trav->trans;

            string_q key = statementKey(accountedFor.address, statement.assetAddr);
            prevStatements[key] = statement;
        }
        if (backLevel) {
            trav->searchOp = UPDATE;
            cacheIfReconciled(trav);
        }
        return !shouldQuit();
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
