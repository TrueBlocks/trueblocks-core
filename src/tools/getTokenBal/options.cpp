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
    CParams("~address_list", "two or more address (0x...), the first is an ERC20 token, balances for the remaining accounts are reported"),
    CParams("~block_list",   "a list of one or more blocks at which to report balances, if empty defaults to 'latest'"),
    CParams("-byAcct",       "if enabled, all but the last address is considered an ERC20 token, balances for each for the last address are reported."),
    CParams("-data",         "render results as tab delimited data (for example, to build a cap table)"),
    CParams("",              "Retrieve token balances from a series of ERC20 token contracts for an account or visa versa.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    SFString address_list;
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-b" || arg == "--byAcct") {
            byAccount = true;

        } else if (arg.startsWith("0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
            address_list += arg + "|";

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

    address_list = Strip(address_list, '|');
    if (countOf('|', address_list) < 1)
        return usage("You must provide both a token contract and an account. Quitting...");

    if (!byAccount) {
        // first item is ERC20 contract, remainder are accounts
        // token holder1 holder2
        holders = address_list;
        tokens = nextTokenClear(holders, '|');
        
    } else {
        // last item is account, preceeding are ERC20 contracts
        address_list.Reverse();
        // holder token2 token1 - reversed
        tokens = address_list;
        holders = nextTokenClear(tokens,'|');
        tokens.Reverse(); holders.Reverse();
    }

    if (blocks.empty())
        blocks = asStringU(getLatestBlockFromClient());

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    tokens = "";
    holders = "";
    blocks = "";
    asData = false;
    byAccount = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
