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
    if (trav->trans.readReconsFromCache(accountedFor.address)) {
        for (auto& statement : trav->trans.statements) {
            string_q key = statementKey(statement.accountedFor, statement.assetAddr);
            previousBalances[key] = statement;
        }
        return !shouldQuit();
    }

    trav->searchOp = RECONCILE;

    CTransfer transfer;
    transfer.assetAddr = FAKE_ETH_ADDRESS;
    transfer.assetSymbol = expContext().asEther ? "ETH" : "WEI";
    transfer.decimals = 18;

    blknum_t prevBlock = trav->index == 0 ? trav->trans.blockNumber == 0 ? 0 : trav->trans.blockNumber - 1
                                          : monApps[trav->index - 1].blk;
    blknum_t nextBlock = trav->index < monApps.size() - 1 ? monApps[trav->index + 1].blk : NOPOS;

    string tokenKey = statementKey(accountedFor.address, transfer.assetAddr);
    if (previousBalances[tokenKey] == CPreviousBalance()) {
        CReconciliation prevStatement(accountedFor.address, &trav->trans);
        prevStatement.blockNumber = prevBlock;
        prevStatement.endBal =
            prevBlock == 0 ? 0 : getTokenBalanceAt(transfer.assetAddr, accountedFor.address, prevBlock);
        previousBalances[tokenKey] = prevStatement;
    }

    CReconciliation aStatement(accountedFor.address, &trav->trans);
    aStatement.assetAddr = transfer.assetAddr;
    aStatement.assetSymbol = transfer.assetSymbol;
    aStatement.decimals = transfer.decimals;
    aStatement.reconcileFlows();
    aStatement.reconcileBalances(prevBlock, nextBlock, previousBalances[tokenKey].balance);
    aStatement.reconcileLabel(prevBlock, nextBlock);
    if (aStatement.amountNet() != 0) {
        trav->trans.statements.push_back(aStatement);
    }
    previousBalances[tokenKey] = aStatement;

    CTransferArray transfers;
    if (trav->trans.getTransfers(transfers, accountedFor.address)) {
        for (auto transfer : transfers) {
            if (assetFilter.size() > 0 && !assetFilter[transfer.assetAddr]) {
                continue;
            }

            string tokenKey = statementKey(accountedFor.address, transfer.assetAddr);
            if (previousBalances[tokenKey] == CPreviousBalance()) {
                CReconciliation prevStatement(accountedFor.address, &trav->trans);
                if (trav->trans.blockNumber > 0) {
                    prevStatement.blockNumber = prevBlock;
                    prevStatement.endBal =
                        prevBlock == 0 ? 0 : getTokenBalanceAt(transfer.assetAddr, accountedFor.address, prevBlock);
                }
                previousBalances[tokenKey] = prevStatement;
            }

            CReconciliation aStatement(accountedFor.address, &trav->trans);
            aStatement.assetAddr = transfer.assetAddr;
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
                trav->trans.statements.push_back(aStatement);
                previousBalances[tokenKey] = aStatement;
            }
        }
    }

    for (size_t s = 0; s < trav->trans.statements.size(); s++) {
        CReconciliation* st = &trav->trans.statements[s];
        st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
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
