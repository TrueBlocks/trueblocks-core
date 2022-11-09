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
    if (trav->index == 0) {
        statementManager.prevBlock = max(trav->trans.blockNumber, blknum_t(1)) - 1;
    } else {
        statementManager.prevBlock = monApps[trav->index - 1].blk;
    }

    if (trav->index < monApps.size() - 1) {
        statementManager.nextBlock = monApps[trav->index + 1].blk;
    } else {
        statementManager.nextBlock = NOPOS;
    }

    return statementManager.getStatements(trav->trans);
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
