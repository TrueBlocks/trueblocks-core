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

    if (!opt->fourByteFilter(trav->trans.input)) {
        return true;
    }

    if (opt->accounting) {
        opt->ledgerManager.getPrevNext(trav->index, trav->trans);
        if (!opt->ledgerManager.getStatements(trav->trans)) {
            return false;  // user quit
        }
        if (!opt->relevant && !opt->ledgerManager.isFilterOn()) {
            // only cache the reconciliation if it's a full set
            trav->trans.cacheIfReconciled(opt->ledgerManager.accountedFor);
        }
    }

    if (opt->statements) {
        // only show the statments...
        for (auto statement : trav->trans.statements) {
            // ... only if they're of the right type
            bool checkFlow = !opt->flow.empty();
            bool in = opt->flow == "in" && statement.amountNet() > 0;
            bool out = opt->flow == "out" && statement.amountNet() < 0;
            bool zero = opt->flow == "zero" && statement.amountNet() == 0;
            if (!checkFlow || in || out || zero) {
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << statement;
                opt->firstOut = false;
            }
        }

    } else {
        if (opt->relevant) {
            for (auto& log : trav->trans.receipt.logs) {
                log.m_showing = opt->isRelevant(log);
            }
        }
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
