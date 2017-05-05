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
    CParams("~addr(s)",     "Ethereum address (starting with '0x') from which to retrieve the balance"),
    CParams("~block(s)",    "the block at which to retrieve the balance (defaults to 'latest')"),
    CParams("-data",        "render results as tab delimited data"),
    CParams("-ether",       "return the balance in Ether instead of Wei"),
    CParams("",             "Retrieve the balance for an account at a block.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-e" || arg == "--ether") {
            asEther = true;

        } else if (arg == "-d" || arg == "--data") {
                asData = true;

        } else if (arg.startsWith("0x")) {
            if (arg.length() != 42)
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
            addrs += arg + "|";

        } else {
            if (toLong(arg)<1)
                return usage(arg + " does not appear to be a valid parameter. Quitting...");
            blocks += arg + "|";
        }
    }

    if (addrs.empty())
        return usage("You must provide at least one Ethereum address.");

    if (blocks.empty())
        blocks = asString(getClientLatestBlk());

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    // addrs = "";
    // blocks = "";
    asEther = false;
    asData = false;

    outScreen.setOutput(stdout);  // so we know where it is at the start of each run
    useVerbose = true;
    useTesting = false;
}

//---------------------------------------------------------------------------------------------------
const char *STR_README_HEAD =
"Retrieve the balance of an address (or more than one) at the given block(s). You may specify multiple "
"addresses or multiple blocks on the line, but not both. `block`, if not specified, defaults to `latest`.\n"
"\n"
"`getBalance` retrieves the balance from the local Ethereum node (not QuickBlocks). Use the `--accounting` "
"option of an account monitor to retrieve the balance from QuickBlocks (both balances should always agree, obviously).\n";

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    header = STR_README_HEAD;
    // footer = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    outScreen.setOutput(stdout);  // flushes and clears archive file if any
}
