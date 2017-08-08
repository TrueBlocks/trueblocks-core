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
    CParams("~token",     "token contract (starting with '0x') from which to retrieve the balance"),
    CParams("~holder(s)", "holder address(es) (starting with '0x') for which to retrieve the balance(s)"),
    CParams("~block(s)",  "the block at which to retrieve the token balance(s) (defaults to 'latest')"),
    CParams("-data",      "render results as tab delimited data"),
    CParams("-ether",     "return the token balance in Ether instead of Wei"),
    CParams("",           "Retrieve the token balance from a token contract for an account at a block (or more).\n"),
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
            if (token.empty())
                token = arg;
            else
                holders += arg + "|";

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (toLong(arg) < 0)
                return usage(arg + " does not appear to be a valid address. Quitting...");
            blocks += arg + "|";
        }
    }

    if (token.empty())
        return usage("You must provide a token contract address.");

    if (holders.empty())
        return usage("You must provide at least one holder address.");

    if (blocks.empty())
        blocks = asString(getLatestBlockFromClient());

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    token = "";
    holders = "";
    blocks = "";
    asEther = false;
    asData = false;

    useVerbose = true;
    useTesting = false;
}

//---------------------------------------------------------------------------------------------------
const char *STR_README_HEAD =
"For the given list of token contracts, retrieve the holders' token balance at the given block(s). You may specify "
"multiple contracts, holder addresses, or blocks on the line, but not more than one multiple. `block`, if not "
"specified, defaults to `latest`.\n"
"\n"
"If the token contract(s) you're requesting are not ERC20 token standard contracts, the return values are "
"undefined.\n"
"\n"
"`getTokenBal` retrieves a token's balance from the local running Ethereum node (not QuickBlocks). Use the "
"`--accounting` option of an account monitor to retrieve the balances from QuickBlocks.\n";

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    header = STR_README_HEAD;
    // footer = "";
    // seeAlso = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
