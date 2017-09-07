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
    CParams("~!hash",       "one or more hashes of Ethereum transactions, or"),
    CParams("~!bn.transID", "blockNumber.transactionID of one or more Ethereum transactions, or"),
    CParams("~!bh.transID", "blockHash.transactionID of one or more Ethereum transactions, or"),
    CParams("~!address",    "if --address, then an Ethereum address"),
    CParams("-fromNode",    "retrieve the transaction from the running node (from QuickBlocks otherwise)"),
    CParams("-address",     "retrieve all logs (from node) given a list of one or more Ethereum addresses"),
    CParams("",             "Retrieve logs from an Ethereum transaction using either QuickBlocks or a running node.\n"
                            " --note: 'hash' and 'blockHash' must start with '0x'."),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-a" || arg == "--address") {
            fromAddr = true;

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (arg.startsWith("0x") || arg.Contains(".")) {
                queries += (arg + "|");  // blockNum.transID

            } else {

                return usage("The argument '" + arg + "' is not properly formatted.");
            }
        }
    }

    if (queries.empty())
        return usage("Please provide at least one transaction or address. Quitting...");

    SFString check = queries;
    while (!check.empty()) {
        SFString arg = nextTokenClear(check, '|');
        SFString orig = arg;
        if (arg.startsWith("0x")) {
            if (arg.Contains(".")) {
                // blockHash.transID okay
                SFString hash = nextTokenClear(arg, '.');
                if (hash.length() != 66 || !isNumeral(arg))
                    return usage("The argument '" + orig + "' is not properly formatted.");
            } else if (arg.length() != 66 && arg.length() != 42) {
                // transaction hash or address okay
                return usage("The argument '" + orig + "' is not properly formatted.");
            }
        } else if (!arg.Contains(".")) {
            SFString blockNum = nextTokenClear(arg, '.');
            if (!isNumeral(blockNum) || !isNumeral(arg))
                return usage("The argument '" + orig + "' is not properly formatted.");
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    queries = "";
    fromAddr = false;
    verbose = true;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "options")
        return str.Substitute("hash bn.transID bh.transID address","< hash | bn.transID | bh.transID | address >"
                              "\n            -- note: This tool is incomplete.\n");
    return str;
}
