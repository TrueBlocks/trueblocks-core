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
    CParams("~block[s]", "a space-separated list of one or more blocks to search for"),
    CParams("-alone",    "display the path(s) of the blocks if found in the cache"),
    CParams("",          "Reports if a block was found in the cache or at a local or remote node.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;

        if (arg == "-a" || arg == "--alone") {
            alone = true;

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {

            if (isUnsigned(arg)) {
                if (toUnsigned(arg) > getLatestBlockFromClient()) {
                    cout << "The block number you requested (";
                    cout << cTeal << orig << cOff;
                    cout << ") is after the latest block (";
                    cout << cTeal << (isTestMode() ? "TESTING" : asStringU(getLatestBlockFromClient())) << cOff;
                    cout << "). Quitting...\n";
                    return false;
                }
                blocks[blocks.getCount()] = toUnsigned(arg);
            }
        }
    }

    if (!blocks.getCount())
        return usage("You must enter a valid block number. Quitting...");
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    blocks.Clear();
    alone = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
