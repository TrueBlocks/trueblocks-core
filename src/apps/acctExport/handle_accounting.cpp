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
            if (opt->relevant) {
                for (auto& log : trav->trans.receipt.logs) {
                    log.m_showing = opt->isRelevant(log);
                }
            }
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

const char* STR_DEBUG =
    "[{BLOCKNUMBER}] [{TRANSACTIONINDEX}] [{BEGBAL}] [{AMOUNTIN}] [{AMOUNTOUT}] [{ENDBAL}] [{RECONCILED}]";
//-----------------------------------------------------------------------
bool COptions::process_reconciliation(CTraverser* trav, blknum_t next) {
    string_q path = getBinaryCachePath(CT_RECONS, accountedFor, trav->trans.blockNumber, trav->trans.transactionIndex);
    establishFolder(path);

    trav->trans.statements.clear();
    if (!isTestMode() && fileExists(path)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            archive >> trav->trans.statements;
            archive.Release();
            LOG4("Reading from cache for ", path);
            for (auto& statement : trav->trans.statements) {
                CAccountName tokenName;
                if (findToken(tokenName, statement.assetAddr)) {
                    // We always freshen these in case user has changed names database
                    statement.assetSymbol = tokenName.symbol;
                    statement.decimals = tokenName.decimals;
                }
                LOG4(statement.Format(STR_DEBUG));
                prevStatements[accountedFor + "_" + toLower(statement.assetAddr)] = statement;
            }
            return !shouldQuit();
        }
    }

    trav->readStatus = "Reconciling";

    // We need to check to see if the export is starting after the
    // the first record so we can pick up the previous balance
    // We must do this for both ETH and any tokens
    if (prevStatements[accountedFor + "_eth"].assetAddr.empty()) {
        CReconciliation pEth(trav->trans.blockNumber == 0 ? 0 : trav->trans.blockNumber - 1, 0, trav->trans.timestamp);
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
        pEth.endBal = getBalanceAt(accountedFor, pEth.blockNumber == 0 ? 0 : pEth.blockNumber - 1);
        LOG4("Adding previous statement: ", pEth.Format(STR_DEBUG));
        prevStatements[accountedFor + "_eth"] = pEth;
    }

    CReconciliation eth(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
    eth.reconcileEth(prevStatements[accountedFor + "_eth"].endBal, prevStatements[accountedFor + "_eth"].blockNumber,
                     next, &trav->trans, accountedFor);
    trav->trans.statements.push_back(eth);
    prevStatements[accountedFor + "_eth"] = eth;
    LOG4("pushed: ", eth.Format(STR_DEBUG));

    CAccountNameMap tokenList;
    if (token_list_from_logs(tokenList, trav)) {
        for (auto item : tokenList) {
            CAccountName tokenName = item.second;

            CReconciliation tokStatement(trav->trans.blockNumber, trav->trans.transactionIndex, trav->trans.timestamp);
            tokStatement.initForToken(tokenName);

            string psKey = accountedFor + "_" + tokenName.address;
            if (prevStatements[psKey].assetAddr.empty()) {
                // first time we've seen this asset, so we need previous balance
                // which is frequently zero but may be non-zero if the command
                // started after the addresses's first transaction
                CReconciliation pBal = tokStatement;
                pBal.blockNumber = 0;
                if (trav->trans.blockNumber > 0)
                    pBal.blockNumber = trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceOf2(tokenName.address, accountedFor, pBal.blockNumber);
                prevStatements[psKey] = pBal;
            }

            tokStatement.prevBlk = prevStatements[psKey].blockNumber;
            tokStatement.prevBlkBal = prevStatements[psKey].endBal;
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

    bool allReconciled = !reversed;
    for (auto recon : trav->trans.statements) {
        if (!allReconciled)
            break;
        allReconciled = recon.reconciled;
    }

    if (allReconciled) {
        lockSection();
        CArchive archive(WRITING_ARCHIVE);
        if (!isTestMode() && archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
            LOG4("Writing to cache for ", path);
            archive << trav->trans.statements;
            archive.Release();
            unlockSection();
            return !shouldQuit();
        }
        unlockSection();
    }

    return !shouldQuit();
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
