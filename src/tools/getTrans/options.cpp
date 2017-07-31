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

    // queries = "";
    trace = false;
    verbose = true;

    useVerbose = true;
    useTesting = false;
}

const char *STR_README_HEADER =
"The `getTrans` tool retrieves transactions from the running Ethereum node (using the `--fromNode` option) "
"or from QuickBlocks (the default). You may provide a transaction `hash`, a `blockNumber.transactionID` "
"pair, or a `blockHash.transactionID` pair (or any combination) to specify the transaction(s).\n"
"\n"
"Use the `--trace` option to display a trace of the transaction as well.\n";

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    SFString url = "https://github.com/paritytech/parity/wiki/JSONRPC-eth-module";
    header = STR_README_HEADER;
    // footer = "";
    seeAlso = "See Also: This command-line tool implements these RPC interfaces: "
        "[eth_getTransactionByHash](" + url + "#eth_gettransactionbyhash), "
        "[eth_getTransactionByBlockNumberAndIndex](" + url + "#eth_gettransactionbyblocknumberandindex), "
        "[eth_getTransactionByBlockHashAndIndex](" + url + "#eth_gettransactionbyblockhashandindex).\n";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
