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
bool statements_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;
    opt->process_reconciliation(trav);

    for (auto statement : trav->trans.statements) {
        if (opt->assetFilter.size() == 0 || opt->assetFilter[statement.assetAddr]) {
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
    }

    return prog_Log(trav, data);
}
