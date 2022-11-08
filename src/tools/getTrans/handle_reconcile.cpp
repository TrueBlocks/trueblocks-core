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

//--------------------------------------------------------------
bool visitReconciliation(CTransaction& trans, void* data) {
    LOG4("blockNumber.txid: ", trans.blockNumber, "\t", trans.transactionIndex);

    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    bigint_t prevBal = 0;
    if (trans.blockNumber > 0) {
        prevBal = getBalanceAt(opt->account_for, trans.blockNumber - 1);
    }

    CReconciliation statement(opt->account_for, &trans);
    statement.nextAppBlk = trans.blockNumber + 1;
    statement.reconcileInside();
    statement.reconcileAcross(prevBal, trans.blockNumber);
    statement.spotPrice = getPriceInUsd(FAKE_ETH_ADDRESS, statement.priceSource, trans.blockNumber);

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

    return true;
}
