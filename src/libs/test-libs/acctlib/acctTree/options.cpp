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
    CParams("-all",            "process all transactions from start of chain to latest block"),
    CParams("-start:<uint>",   "the first block to process"),
    CParams("-end:<uint>",     "the last block (less one) to process"),
    CParams("-nblocks:<uint>", "the number of blocks to visit (ignored for -a)"),
    CParams("",                "Build an account tree listing first transaction, latest transaction, and "
                               "node balance for each account.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    bool hasN = false;
    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg.startsWith("-n:") || arg.startsWith("--nblocks:")) {
            arg = orig.Substitute("-n:","").Substitute("--nblocks:","");
            nBlocks = newUnsigned32(arg);
            hasN = true;

        } else if (arg.startsWith("-s:") || arg.startsWith("--start:")) {
            arg = orig.Substitute("-s:","").Substitute("--start:","");
            startBlock = newUnsigned32(arg);
            if (!isUnsigned(arg))
                return usage("Positive start block number expected: " + orig);

        } else if (arg.startsWith("-e:") || arg.startsWith("--end:")) {
            arg = orig.Substitute("-e:","").Substitute("--end:","");
            if (arg == "latest") {
                endBlock = getLatestBlockFromClient();
            } else {
                endBlock = newUnsigned32(arg);
                if (!isUnsigned(arg))
                    return usage("Positive end block number expected: " + orig);
            }

        } else {
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (hasN && endBlock != NOPOS)
        return usage("You may use either -n or -e, not both. Quitting...");
    if (endBlock != NOPOS)
        nBlocks = (endBlock - startBlock);
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    all = false;
    startBlock = 0;
    endBlock = NOPOS;
    nBlocks = 500;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
