/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool visitTransaction(CTransaction& trans, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;
        forEveryTransaction(visitTransaction, &options, options.transList.queries);
    }
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    const COptions *opt = (const COptions*)data;

    if (opt->isRaw) {
        SFString results;
        queryRawTransaction(results, trans.getValueByName("hash"));
        cout << results << "\n";
        return true;
    }

    if (verbose) {
        trans.doExport(cout);
        cout << "\n";
    } else {
        cout << trans.Format("[{DATE}][\t{TIMESTAMP}][\t{BLOCKNUMBER}][\t{TRANSACTIONINDEX}][\t{HASH}]\n");
    }

    return true;
}
