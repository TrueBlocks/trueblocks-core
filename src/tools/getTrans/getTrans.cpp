/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

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

        while (!options.queries.empty()) {
            SFString remains = nextTokenClear(options.queries, '|');
            SFString hash = nextTokenClear(remains, '.');
            SFUint32 txID = toLongU(remains);

            CTransaction trans;
            if (hash.startsWith("0x")) {
                if (txID) {
                    getTransaction(trans, hash, txID);  // blockHash.txID
                } else {
                    getTransaction(trans, hash);  // transHash
                }
            } else {
                getTransaction(trans, (uint32_t)toLongU(hash), txID);  // blockHash.txID
            }

            // We need to pick up some stuff from the block
            CBlock block;
            getBlock(block, trans.blockNumber);
            trans.pBlock = &block;

            SFString fmt;
            if (!verbose)
                fmt = "[{DATE}]\t[{TIMESTAMP}]\t[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t[{HASH}]";

            SFString ret = trans.Format(fmt);
            if (options.trace) {
                SFString trace;
                queryRawTrace(trace, trans.hash);
                ret.ReplaceReverse("}", ",\"trace\": " + trace + "}");
            }
            cout << ret << "\n";
        }

    }
    return 0;
}
