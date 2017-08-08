/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~!hash",        "one or more hashes of Ethereum transactions, or"),
    CParams("~!bn.transID",  "blockNumber.transactionID of one or more Ethereum transactions, or"),
    CParams("~!bh.transID",  "blockHash.transactionID of one or more Ethereum transactions"),
    CParams("-fromNode",    "retrieve the transaction from the running node (from QuickBlocks otherwise)"),
    CParams("-trace",       "return the trace of the transaction in addition to regular details"),
    CParams("",             "Retrieve an Ethereum transaction from either QuickBlocks or a running node.\n"
                            " --note: 'hash' and 'blockHash' must start with '0x'."),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-t" || arg == "--trace") {

            trace = true;

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (arg.startsWith("0x")) {

                if (arg.Contains(".")) {

                    SFString hash = nextTokenClear(arg, '.');
                    if (hash.length() != 66 || toLong(arg) < 1)
                        return usage("The argument '" + arg + "' is not properly formatted.");
                    queries += (hash + "." + arg + "|");  // blockHash.transID

                } else if (arg.length() == 66) {

                    queries += (arg + "|");  // transHash
                }

            } else if (arg.Contains(".")) {

                queries += (arg + "|");  // blockNum.transID

            } else {

                return usage("The argument '" + arg + "' is not properly formatted.");
            }
        }
    }

    if (queries.empty())
        return usage("Invalid input.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    queries = "";
    trace = false;
    verbose = true;

    useVerbose = true;
    useTesting = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
