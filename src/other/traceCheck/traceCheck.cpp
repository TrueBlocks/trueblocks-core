/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "traceCheck.h"

extern bool visitBlock(uint64_t num, void *data);
extern bool visitTransaction(CTransaction& trans, void *data);
extern bool visitTrace(CTrace& trace, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHander);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    string_q checkResults;
    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        options.blocks.forEveryBlockNumber(visitBlock, NULL);
    }

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

