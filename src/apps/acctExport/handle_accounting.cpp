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
bool COptions::process_reconciliation(CTraverser* trav) {
    string_q path =
        getPathToBinaryCache(CT_RECONS, accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex);
    establishFolder(path);

    trav->trans.statements.clear();
    if (!isTestMode() && fileExists(path)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            archive >> trav->trans.statements;
            archive.Release();
            if (true) {
                bool backLevel = false;
                for (auto& statement : trav->trans.statements) {
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
                    prevStatements[accountedFor.address + "_" + toLower(statement.assetAddr)] = statement;
                }
                if (backLevel) {
                    // LOG_WARN(cYellow, "Updating statements", cOff);
                    trav->searchOp = UPDATE;
                    cacheIfReconciled(trav, true /* isNew */);
                    slowQueries++;
                    reportFreq = 1;
                }
                return !shouldQuit();
                //             } else {
                //               trav->trans.statements.clear();
            }
        }
    }

    trav->searchOp = RECONCILE;
    slowQueries++;
    reportFreq = 1;

    blknum_t nextAppBlk = trav->index < monApps.size() - 1 ? monApps[trav->index + 1].blk : NOPOS;
    blknum_t prevAppBlk = trav->index > 0 ? monApps[trav->index - 1].blk : 0;
    blknum_t prevAppTxid = trav->index > 0 ? monApps[trav->index - 1].txid : 0;

    // We need to check to see if the export is starting after the
    // the first record so we can pick up the previous balance
    // We must do this for both ETH and any tokens
    if (prevStatements[accountedFor.address + "_eth"].assetAddr.empty()) {
        CReconciliation pEth(prevAppBlk, prevAppTxid, trav->trans.timestamp);
        // TODO(tjayrush): This code is wrong. We ask for the balance at the current
        // TODO(tjayrush): block minus one, but we should ask at the previous block in this address's
        // TODO(tjayrush): appearance list. When we're called with first_record or start_block not zero
        // TODO(tjayrush): we don't have this (since we only load those appearances we're asked for)
        // TODO(tjayrush): To fix this, we need to be able to get the previous appearance's block.
        // TODO(tjayrush):
        // TODO(tjayrush): Also, we used to handle reversed mode here, that code was removed
        pEth.endBal =
            trav->trans.blockNumber == 0 ? 0 : getBalanceAt(accountedFor.address, trav->trans.blockNumber - 1);
        pEth.spotPrice = getPriceInUsd(trav->trans.blockNumber - 1, pEth.priceSource);
        prevStatements[accountedFor.address + "_eth"] = pEth;
    }

    CReconciliation eth(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
    eth.reconcileEth(prevStatements[accountedFor.address + "_eth"], nextAppBlk, &trav->trans, accountedFor);
    eth.spotPrice = getPriceInUsd(trav->trans.blockNumber, eth.priceSource);
    trav->trans.statements.push_back(eth);
    prevStatements[accountedFor.address + "_eth"] = eth;

    CAccountNameMap tokenList;
    if (token_list_from_logs(tokenList, trav)) {
        for (auto item : tokenList) {
            CAccountName tokenName = item.second;

            CReconciliation tokStatement(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
            tokStatement.initForToken(tokenName);

            string psKey = accountedFor.address + "_" + tokenName.address;
            if (prevStatements[psKey].assetAddr.empty()) {
                // first time we've seen this asset, so we need previous balance
                // which is frequently zero but may be non-zero if the command
                // started after the addresses's first transaction
                CReconciliation pBal = tokStatement;
                pBal.blockNumber = 0;
                if (trav->trans.blockNumber > 0)
                    pBal.blockNumber = trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceOf2(tokenName.address, accountedFor.address, pBal.blockNumber);
                pBal.spotPrice = getPriceInUsd(pBal.blockNumber, pBal.priceSource, tokenName.address);
                prevStatements[psKey] = pBal;
            }

            tokStatement.prevBlk = prevStatements[psKey].blockNumber;
            tokStatement.prevBlkBal = prevStatements[psKey].endBal;
            tokStatement.begBal = prevStatements[psKey].endBal;
            tokStatement.endBal = getTokenBalanceOf2(tokenName.address, accountedFor.address, trav->trans.blockNumber);
            if (tokStatement.begBal > tokStatement.endBal) {
                tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
            } else {
                tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
            }
            tokStatement.reconciliationType = "";
            if (tokStatement.amountNet() != 0) {
                tokStatement.spotPrice =
                    getPriceInUsd(trav->trans.blockNumber, tokStatement.priceSource, tokenName.address);
                trav->trans.statements.push_back(tokStatement);
                prevStatements[psKey] = tokStatement;
            }
        }
    }

    cacheIfReconciled(trav, true /* isNew */);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool COptions::isReconciled(CTraverser* trav) const {
    for (auto recon : trav->trans.statements) {
        if (!recon.reconciled())
            return false;
    }
    return true;
}

//-----------------------------------------------------------------------
void COptions::cacheIfReconciled(CTraverser* trav, bool isNew) const {
    if (isTestMode())
        return;
    if (!isReconciled(trav)) {
        LOG_WARN("Transaction ", trav->trans.hash, " did not reconcile.");
    }

    lockSection();
    CArchive archive(WRITING_ARCHIVE);
    string_q path =
        getPathToBinaryCache(CT_RECONS, accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        LOG4("Writing to cache for ", path);
        archive << trav->trans.statements;
        archive.Release();
    }
    unlockSection();
}

//-----------------------------------------------------------------------
bool acct_PreFunc(CTraverser* trav, void* data) {
    return true;
}

//-----------------------------------------------------------------------
bool COptions::token_list_from_logs(CAccountNameMap& tokenList, const CTraverser* trav) {
    for (auto log : trav->trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(log.address, tokenName);
        if (tokenName.address.empty())
            tokenName.address = log.address;
        if ((isToken || trav->trans.hasToken))
            tokenList[log.address] = tokenName;
    }
    return tokenList.size() > 0;
}
