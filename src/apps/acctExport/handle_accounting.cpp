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
bool acct_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (opt->monApps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->monApps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->monApps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}

//-----------------------------------------------------------------------
bool COptions::process_reconciliation(CTraverser *trav, blknum_t next) {
    string_q path = getBinaryCachePath(CT_RECONS, accountedFor, trav->trans.blockNumber, trav->trans.transactionIndex);
    establishFolder(path);

    if (fileExists(path)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            archive >> trav->trans.statements;
            for (auto statement : trav->trans.statements)
                prevStatements[accountedFor + "_" + toLower(statement.asset)] = statement;
            archive.Release();
            return true;
        }
    }

    trav->readStatus = "Reconciling";
    CAddressBoolMap done;
    CStringArray corrections;
    CReconciliation theStatement(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
    theStatement.reconcileEth(corrections, prevStatements, next, &trav->trans, accountedFor);
    trav->trans.statements.clear();
    trav->trans.statements.push_back(theStatement);
    prevStatements[accountedFor + "_eth"] = theStatement;
    for (auto log : trav->trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = findToken(tokenName, log.address);
        bool isAirdrop = airdropMap[log.address];
        bool isDone = done[log.address];
        if ((isToken || trav->trans.hasToken || isAirdrop) && !isDone) {
            CMonitor m;
            m.address = log.address;
            theStatement.reset();
            theStatement.asset = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
            if (theStatement.asset.empty())
                theStatement.asset = getTokenSymbol(m, trav->trans.blockNumber);
            if (isAirdrop && theStatement.asset.empty()) {
                getNamedAccount(tokenName, log.address);
                theStatement.asset = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
            }
            if (theStatement.asset.empty()) {
                theStatement.asset = log.address.substr(0, 4) + "...";
            }
            theStatement.decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;
            string key = accountedFor + "_" + log.address;
            theStatement.begBal = prevStatements[key].endBal;
            theStatement.endBal = str_2_BigInt(getTokenBalanceOf(m, accountedFor, trav->trans.blockNumber));
            if (theStatement.begBal > theStatement.endBal) {
                theStatement.amountOut = (theStatement.begBal - theStatement.endBal);
            } else {
                theStatement.amountIn = (theStatement.endBal - theStatement.begBal);
            }
            theStatement.amountNet = theStatement.amountIn - theStatement.amountOut;
            theStatement.reconciled = true;
            theStatement.reconciliationType = "";
            done[log.address] = true;
            if (theStatement.amountNet != 0)
                trav->trans.statements.push_back(theStatement);
            prevStatements[key] = theStatement;
        }
    }

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        archive << trav->trans.statements;
        archive.Release();
        return true;
    }

    return true;
}
