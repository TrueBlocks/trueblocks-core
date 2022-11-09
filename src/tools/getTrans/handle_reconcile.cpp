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

#define REC_TOP (1 << 0)
#define REC_TOKENS (1 << 2)
#define REC_ALL (REC_TOP | REC_TOKENS)
// #define REC_TRACES (1 << 1)
// #define REC_ALL (REC_TOP | REC_TRACES | REC_TOKENS)

size_t getStatements(CTransaction& trans, const address_t& accountedFor, int which);
//--------------------------------------------------------------
bool visitReconciliation(CTransaction& trans, void* data) {
    LOG4("blockNumber.txid: ", trans.blockNumber, "\t", trans.transactionIndex);

    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (getStatements(trans, opt->account_for, REC_ALL) > 0) {
        for (auto statement : trans.statements) {
            if (isText) {
                cout << trim(statement.Format(expContext().fmtMap["format"]), '\t') << endl;
            } else {
                if (!opt->firstOut)
                    cout << ",";
                cout << "  ";
                indent();
                statement.toJson(cout);
                unindent();
                opt->firstOut = false;
            }
        }
    } else {
        LOG_ERR("No material transactions found");
    }

    return true;
}

size_t getStatements(CTransaction& trans, const address_t& accountedFor, int which) {
    blknum_t prevBlock = trans.blockNumber == 0 ? 0 : trans.blockNumber - 1;
    blknum_t nextBlock = trans.blockNumber + 1;
    bigint_t prevBal = 0;
    if (trans.blockNumber > 0) {
        prevBal = getBalanceAt(accountedFor, prevBlock);
    }

    CReconciliation statement(accountedFor, &trans);
    if (!statement.reconcileFlows()) {
        statement.reconcileFlows_traces();
    }
    statement.reconcileBalances(prevBlock, nextBlock, prevBal);
    statement.reconcileLabel(prevBlock, nextBlock);
    statement.assetAddr = FAKE_ETH_ADDRESS;
    statement.assetSymbol = expContext().asEther ? "ETH" : "WEI";
    statement.encoding = trans.input.size() >= 10 ? trans.input.substr(0, 10) : "";
    statement.signature = trans.Format("[{COMPRESSEDTX}]");
    if (statement.reconciliationType == "regular") {
        statement.reconciliationType = "by-tx";
    }
    if (statement.amountNet() != 0) {
        trans.statements.push_back(statement);
    }

    for (size_t s = 0; s < trans.statements.size(); s++) {
        CReconciliation* st = &trans.statements[s];
        st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
    }

    return trans.statements.size();
}