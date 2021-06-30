/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (fourByteFilter(trav->trans.input, opt)) {
        if (opt->accounting) {
            blknum_t next = trav->index < opt->monApps.size() - 1 ? opt->monApps[trav->index + 1].blk : NOPOS;
            opt->process_reconciliation(trav, next);
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool COptions::process_reconciliation(CTraverser* trav, blknum_t next) {
    string_q path = getBinaryCachePath(CT_RECONS, accountedFor, trav->trans.blockNumber, trav->trans.transactionIndex);
    establishFolder(path);

    trav->trans.statements.clear();
    if (!isTestMode() && fileExists(path)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            archive >> trav->trans.statements;
            for (auto statement : trav->trans.statements)
                prevStatements[accountedFor + "_" + toLower(statement.assetAddr)] = statement;
            archive.Release();
            return !shouldQuit();
        }
    }

    trav->readStatus = "Reconciling";

    // We need to check to see if the export is starting after the
    // the first record so we can pick up the previous balance
    // We must do this for both ETH and any tokens
    if (prevStatements[accountedFor + "_eth"].assetAddr.empty()) {
        CReconciliation pEth;
        if (reversed) {
            if (first_record != 0) {
                pEth.blockNumber = monApps[monApps.size() - 1].blk - 1;
            } else if (exportRange.first != 0) {
                pEth.blockNumber = exportRange.second - 1;
            }
        } else {
            if (first_record != 0) {
                pEth.blockNumber = monApps[0].blk - 1;
            } else if (exportRange.first != 0) {
                pEth.blockNumber = exportRange.first - 1;
            }
        }
        pEth.endBal = getBalanceAt(accountedFor, pEth.blockNumber);
        prevStatements[accountedFor + "_eth"] = pEth;
    }

    CStringArray unused;  // future reference -- used for corrections
    CReconciliation eth(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
    eth.reconcileEth(unused, prevStatements, next, &trav->trans, accountedFor);
    trav->trans.statements.push_back(eth);
    prevStatements[accountedFor + "_eth"] = eth;

    CAccountNameMap tokenList;
    if (token_list_from_logs(tokenList, trav)) {
        for (auto item : tokenList) {
            CAccountName tokenName = item.second;
            CReconciliation tokStatement(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
            tokStatement.assetAddr = tokenName.address;
            tokStatement.assetSymbol = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
            if (tokStatement.assetSymbol.empty())
                tokStatement.assetSymbol = getTokenSymbol(tokenName.address, trav->trans.blockNumber);
            if (airdropMap[tokenName.address] && tokStatement.assetSymbol.empty()) {
                getNamedAccount(tokenName, tokenName.address);
                tokStatement.assetSymbol = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
            }
            if (tokStatement.assetSymbol.empty()) {
                tokStatement.assetSymbol = tokenName.address.substr(0, 4) + "...";
            }
            tokStatement.decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;

            string psKey = accountedFor + "_" + tokenName.address;

            if (prevStatements[psKey].assetAddr.empty()) {
                // first time we've seen this asset, so we need previous balance
                // which is frequently zero but may be non-zero if the command
                // started after the addresses's first transaction
                CReconciliation pBal = tokStatement;
                blknum_t bn = 0;
                if (trav->trans.blockNumber > 0)
                    bn = trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceOf2(tokenName.address, accountedFor, bn);
                prevStatements[psKey] = pBal;
            }

            tokStatement.begBal = prevStatements[psKey].endBal;
            tokStatement.endBal = getTokenBalanceOf2(tokenName.address, accountedFor, trav->trans.blockNumber);
            if (tokStatement.begBal > tokStatement.endBal) {
                tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
            } else {
                tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
            }
            tokStatement.amountNet = tokStatement.amountIn - tokStatement.amountOut;
            tokStatement.reconciled = true;
            tokStatement.reconciliationType = "";
            if (tokStatement.amountNet != 0)
                trav->trans.statements.push_back(tokStatement);

            prevStatements[psKey] = tokStatement;
        }
    }

    CArchive archive(WRITING_ARCHIVE);
    if (!isTestMode() && archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        archive << trav->trans.statements;
        archive.Release();
        return !shouldQuit();
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool COptions::token_list_from_logs(CAccountNameMap& tokenList, const CTraverser* trav) {
    for (auto log : trav->trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(tokenName, log.address);
        bool isAirdrop = airdropMap[log.address];
        if (tokenName.address.empty())
            tokenName.address = log.address;
        if ((isToken || isAirdrop || trav->trans.hasToken))
            tokenList[log.address] = tokenName;
    }
    return tokenList.size() > 0;
}
