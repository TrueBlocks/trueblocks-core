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
bool traces_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    for (auto trace : trav->trans.traces) {
        CTrace copy = trace;
        if (fourByteFilter(trace.action.input, opt)) {
            // Do not collapse with the following code block...both (create and (suicide|regular)) can be true in a
            // single trace
            if (!opt->factory) {
                bool isSuicide = trace.action.selfDestructed != "";
                if (isSuicide) {
                    copy.action.from = trace.action.selfDestructed;
                    copy.action.to = trace.action.refundAddress;
                    copy.action.callType = "suicide";
                    copy.action.value = trace.action.balance;
                    copy.traceAddress.push_back("s");
                    copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                    copy.action.input = "0x";
                }
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy;
                opt->firstOut = false;
            }

            bool isCreation = trace.result.newContract != "";
            if (isCreation) {
                copy.action.from = "0x0";
                copy.action.to = trace.result.newContract;
                copy.action.callType = "creation";
                copy.action.value = trace.action.value;
                if (copy.traceAddress.size() == 0)
                    copy.traceAddress.push_back("null");
                copy.traceAddress.push_back("s");
                copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                copy.action.input = trace.action.input;
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy;
                opt->firstOut = false;
            }
        }
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
size_t traces_Count(CTraverser* trav, void* data) {
    return trav->trans.traces.size();
}
