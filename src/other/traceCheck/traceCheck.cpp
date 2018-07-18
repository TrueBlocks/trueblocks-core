/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "traceCheck.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    string_q command = nextTokenClear(options.commandList, '\n');
    if (!options.parseArguments(command))
        return 0;

    options.blocks.forEveryBlockNumber(visitBlock, NULL);

    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t num, void *data) {
    CBlock block;
    getBlock(block, num);
    return forEveryTransactionInBlock(visitTransaction, &block, block);
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    cout << trans.Format("[{BLOCKNUMBER}]") << "." << trans.transactionIndex << " " << trans.from << " " << trans.to;
    cout << " (" << trans.receipt.gasUsed << ")\n";
    bool ret = forEveryTraceInTransaction(visitTrace, data, trans);
    cout << "\n";
    cout.flush();
    return ret;
}

//--------------------------------------------------------------
extern bool visitTrace(CTrace& trace, void *data) {
    string_q str = substitute(string_q(trace.traceAddress.size()+1, '.'), ".", "  ");
    replaceReverse(str, "  ", cYellow + "--- " + cOff);
    cout << str;
    cout << trace.action.from << cTeal << " [" << trace.action.callType << "] " << cOff;
    cout << trace.action.to << " (" << trace.result.gasUsed << ") " << trace.action.input.length() << ".\n";
    return true;
}
