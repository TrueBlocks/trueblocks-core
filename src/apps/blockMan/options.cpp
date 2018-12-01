//check

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-stats",       "show statistics about the blocks"),
    COption("-check",       "check the block cache"),
    COption("-empty",       "check empty blocks (assumes --check)"),
    COption("-full",        "check full blocks (assumes --check)"),
    COption("-start:<num>", "optionally start at block 'num'"),
    COption("@skip",        "export once every 5,000 lines"),
    COption("",             "Show stats or check the block cache.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();

    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-s" || arg == "--stats") {
            showStats = true; // last in wins

        } else if (arg == "-e" || arg == "--empty") {
            full = false;
            showStats = false; // last in wins

        } else if (arg == "-f" || arg == "--full") {
            empty = false;
            showStats = false; // last in wins

        } else if (arg == "-c" || arg == "--check") {
            showStats = false; // last in wins

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--start:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--start:", "");
            if (!isUnsigned(arg))
                return usage(arg + " does not appear to be a valid number. Quitting...\n");
            startBlock = str_2_Uint(arg);

        } else {
            if (!builtInCmd(arg))
                return usage("Invalid option: " + arg);
        }
    }

    if (!fileExists(fullBlockIndex))
        return usage("Could not open file '" + fullBlockIndex + "'. Quitting...\n");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;

    showStats = true;
    empty     = true;
    full      = true;
    startBlock = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
