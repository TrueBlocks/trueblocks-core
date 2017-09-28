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
    CParams("~address_list", "One or more Ethereum addresses (starting with '0x') from which to retrieve balances"),
    CParams("~block_list",   "a list of one or more blocks at which to report balances, if empty defaults to 'latest'"),
    CParams("-noZero",       "suppress the display of zero balance accounts"),
    CParams("-data",         "render results as tab delimited data"),
    CParams("",              "Retrieve the balance(s) for one or more accounts at one or more blocks.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-n" || arg == "--noZero") {
            noZero = true;

        } else if (arg.startsWith("0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
            addrs += arg + "|";

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (toLong(arg) < 0)
                return usage(arg + " does not appear to be a valid block. Quitting...");
            blocks += arg + "|";
        }
    }

    if (addrs.empty())
        return usage("You must provide at least one Ethereum address.");

    if (blocks.empty())
        blocks = asStringU(getLatestBlockFromClient());

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    addrs = "";
    blocks = "";
    asData = false;
    noZero = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
