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
bool COptions::process_statements(CTraverser* trav) {
    if (trav->trans.readReconsFromCache(accountedFor.address, prevStatements)) {
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
        CReconciliation prevStatement(accountedFor.address, &trav->trans);
        prevStatement.blockNumber = prevAppBlk;
        if (prevAppBlk > 0) {
            prevStatement.endBal = getBalanceAt(accountedFor.address, prevAppBlk);
        }
        prevStatements[ethKey] = prevStatement;
    }

    CReconciliation ethStatement(accountedFor.address, &trav->trans);
    ethStatement.reconcileInside();
    ethStatement.nextAppBlk = trav->index < monApps.size() - 1 ? monApps[trav->index + 1].blk : NOPOS;
    ethStatement.reconcileAcross(prevStatements[ethKey].endBal, prevStatements[ethKey].blockNumber);
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
                CReconciliation pBal;
                pBal.pTransaction = &trav->trans;
                pBal.blockNumber = trav->trans.blockNumber == 0 ? 0 : trav->trans.blockNumber - 1;
                pBal.endBal = getTokenBalanceAt(tokStatement.assetAddr, accountedFor.address, pBal.blockNumber);
                prevStatements[tokenKey] = pBal;
            }

            tokStatement.prevAppBlk = prevStatements[tokenKey].blockNumber;
            tokStatement.prevBal = prevStatements[tokenKey].endBal;
            tokStatement.begBal = prevStatements[tokenKey].endBal;
            tokStatement.endBal =
                getTokenBalanceAt(tokStatement.assetAddr, tokStatement.accountedFor, tokStatement.blockNumber);

            if (tokStatement.begBal != tokStatement.endBal) {
                if (tokStatement.begBal > tokStatement.endBal) {
                    tokStatement.amountOut = (tokStatement.begBal - tokStatement.endBal);
                } else {
                    tokStatement.amountIn = (tokStatement.endBal - tokStatement.begBal);
                }
                tokStatement.reconciliationType = "token";
                trav->trans.statements.push_back(tokStatement);
                prevStatements[tokenKey] = tokStatement;
            }
        }
    }

    if (reversed) {
        for (size_t s = trav->trans.statements.size() - 1; s >= 0; s--) {
            CReconciliation* st = &trav->trans.statements[s];
            st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
        }
    } else {
        for (int s = 0; s < trav->trans.statements.size(); s++) {
            CReconciliation* st = &trav->trans.statements[size_t(s)];
            st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
        }
    }

    trav->trans.cacheIfReconciled(accountedFor.address);

    return !shouldQuit();
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
