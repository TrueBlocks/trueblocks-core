/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
// TODO(tjayrush): NO!
CReconciliationArray statements;
CReconciliation current;
bool CAccumulator::accumulate(const CTransaction* trans, CTransaction& summary) {
    if (isSamePeriod(sum_type, endOfPeriod, ts_2_Date(trans->timestamp))) {
        for (auto statement : trans->statements) {
            statements.push_back(statement);
            current += statement;
        }
        return false;
    }

    ((CTransaction*)trans)->statements.clear();
    summary = *trans;
    summary.from = "0xSummary";
    summary.setValueByName("timestamp", ts_2_Str(date_2_Ts(endOfPeriod)));
    summary.timestamp = date_2_Ts(endOfPeriod);
    summary.statements.push_back(current);
    current = CReconciliation();
    endOfPeriod = EOP(sum_type, ts_2_Date(trans->timestamp));
    return true;
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

        if (opt->summarize_by.empty()) {
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
        } else {
            trav->trans.from = "0xSummary";
            CTransactionTraverser* tt = (CTransactionTraverser*)trav;
            CTransaction summary;
            if (tt->pl.endOfPeriod == earliestDate)
                tt->pl.endOfPeriod = EOP(tt->pl.sum_type, ts_2_Date(tt->trans.timestamp));
            if (tt->pl.accumulate(&tt->trans, summary)) {
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << summary;
                opt->firstOut = false;
            }
        }
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool COptions::process_reconciliation(CTraverser* trav) {
    string_q path =
        getBinaryCachePath(CT_RECONS, accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex);
    establishFolder(path);

    trav->trans.statements.clear();
    if (!isTestMode() && fileExists(path)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            archive >> trav->trans.statements;
            archive.Release();
            if (isReconciled(trav)) {
                for (auto& statement : trav->trans.statements) {
                    CAccountName tokenName;
                    if (contains(statement.assetSymbol, "reverted"))
                        statement.assetSymbol = "";
                    if (statement.assetSymbol != "ETH" && statement.assetSymbol != "WEI" &&
                        findToken(tokenName, statement.assetAddr)) {
                        // We always freshen these in case user has changed names database
                        statement.assetSymbol = tokenName.symbol;
                        statement.decimals = tokenName.decimals;
                    }
                    prevStatements[accountedFor.address + "_" + toLower(statement.assetAddr)] = statement;
                }
                return !shouldQuit();
            } else {
                trav->trans.statements.clear();
            }
        }
    }

    trav->readStatus = "Reconciling";
    slowQuery = true;

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
        // TODO(tjayrush): Also, we used to do something different for reversed mode
        pEth.endBal =
            trav->trans.blockNumber == 0 ? 0 : getBalanceAt(accountedFor.address, trav->trans.blockNumber - 1);
        prevStatements[accountedFor.address + "_eth"] = pEth;
    }

    CReconciliation eth(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
    eth.reconcileEth(prevStatements[accountedFor.address + "_eth"], nextAppBlk, &trav->trans, accountedFor);
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
            if (tokStatement.amountNet() != 0)
                trav->trans.statements.push_back(tokStatement);
            prevStatements[psKey] = tokStatement;
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
        LOG_WARN("Transaction ", trav->trans.hash, " did not reconciled.");
        return;
    }

    lockSection();
    CArchive archive(WRITING_ARCHIVE);
    string_q path =
        getBinaryCachePath(CT_RECONS, accountedFor.address, trav->trans.blockNumber, trav->trans.transactionIndex);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        LOG4("Writing to cache for ", path);
        archive << trav->trans.statements;
        archive.Release();
    }
    unlockSection();
}

//-----------------------------------------------------------------------
bool acct_PreFunc(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;
    if (opt->summarize_by.empty())
        return true;
    HIDE_FIELD(CTransaction, "to");
    HIDE_FIELD(CTransaction, "value");
    HIDE_FIELD(CTransaction, "gas");
    HIDE_FIELD(CTransaction, "gasPrice");
    HIDE_FIELD(CTransaction, "input");
    HIDE_FIELD(CTransaction, "receipt");
    HIDE_FIELD(CTransaction, "traces");
    HIDE_FIELD(CTransaction, "compressedTx");
    HIDE_FIELD(CTransaction, "gasCost");
    HIDE_FIELD(CTransaction, "etherGasCost");
    HIDE_FIELD(CTransaction, "function");
    HIDE_FIELD(CTransaction, "gasUsed");
    HIDE_FIELD(CTransaction, "ether");
    HIDE_FIELD(CTransaction, "encoding");
    HIDE_FIELD(CTransaction, "articulatedTx");
    HIDE_FIELD(CTransaction, "isError");
    CTransactionTraverser* tt = (CTransactionTraverser*)trav;
    tt->pl.sum_type = getSummaryType(opt->summarize_by);
    tt->pl.endOfPeriod = earliestDate;
    return true;
}

//-----------------------------------------------------------------------
bool COptions::token_list_from_logs(CAccountNameMap& tokenList, const CTraverser* trav) {
    for (auto log : trav->trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(tokenName, log.address);
        if (tokenName.address.empty())
            tokenName.address = log.address;
        if ((isToken || trav->trans.hasToken))
            tokenList[log.address] = tokenName;
    }
    return tokenList.size() > 0;
}
