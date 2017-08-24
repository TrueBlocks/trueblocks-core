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
    CParams("-all",          "process all transactions from start of chain to latest block"),
    CParams("-start:<uint>", "the first block to process"),
    CParams("",              "Build an account tree listing first transaction, latest transaction, and "
                             "node balance for each account.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg.startsWith("-s:") || arg.startsWith("--start:")) {
            arg = orig.Substitute("-s:","").Substitute("--start:","");
            startBlock = newUnsigned32(arg);
            if (!isUnsigned(arg))
                return usage("Positive block number expected: " + orig);

        } else {
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    all = false;
    startBlock = 0;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
