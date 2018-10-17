/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~mode",      "either 'list', 'at, or 'freshen'"),
    COption("-at:<key>",  "return the value stored at address 'val'"),
    COption("-list",      "list all accounts in the account database (list mode)"),
    COption("-freshen",   "starting with the last block visited, update the account tree (freshen mode)"),
    COption("-refreshen", "remove the existing account tree and re-create it from scratch with freshen mode"),
    COption("",           "Build a database of accounts with arbirtrary data attached.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    bool isRefreshen = false;
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-l" || arg == "--list") {
            mode += "list|";

        } else if (startsWith(arg, "-a") || startsWith(arg, "--at")) {
            arg = substitute(substitute(arg, "-a:", ""), "--at:", "");
            if (!isAddress(arg))
                return usage("--at requires an following address. Quitting...");
            address_list += (arg + "|");

        } else if (arg == "-f" || arg == "--freshen") {
            mode += "freshen|";

        } else if (arg == "-r" || arg == "--refreshen") {
            if (fileExists(accountIndex)) {
                cerr << "Are you sure you want to remove the account index? (y=yes)\n> ";
                int ch = getchar();
                if (ch == 'y' || ch == 'Y') {
                    remove(accountIndex.c_str());
                } else {
                    cerr << "Not removed. Quitting.\n";
                    cerr.flush();
                    exit(0);
                }
            }
            isRefreshen = true;
            mode += "freshen|";

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            mode += (arg + "|");
        }
    }

    bool hasFresh = contains(mode, "freshen");
    bool hasList  = contains(mode, "list");

    // fixup so things happen in a sensible order
    mode  = (hasFresh ? "freshen|" : "");
    mode += (hasList  ? "list|"    : "");
    if (mode.empty() && address_list.empty())
        return usage("Mode must be one of --list, --freshen, --at, or --refreshen.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    address_list = "";
    mode = "";
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

