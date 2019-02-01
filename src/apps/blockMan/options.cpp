//check

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-list",          "list all blocks in the index (non-empty blocks only by default)"),
    COption("-check",         "verify the block index (both empty and non-empty by default)"),
    COption("-stats",         "show statistics for the block index (the default)"),
    COption("-empty",         "check or list only empty blocks"),
    COption("-full",          "check or list only full blocks"),
    COption("-sta(r)t:<num>", "optionally start at block 'num'"),
    COption("-sto(p):<num>",  "optionally stop at block 'num'"),
    COption("@s(k)ip:<num>",  "export once every 5,000 lines"),
//    COption("@fix",           "fix the database by removing duplicates (if any)"),
    COption("",               "Show stats or check the block cache.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t stopBlock = getLatestBlockFromClient();
    if (isTestMode())
        stopBlock = 4000000;

    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-l" || arg == "--list") {
            modes |= OP_LIST;

        } else if (arg == "-c" || arg == "--check") {
            modes |= OP_CHECK;

        } else if (arg == "-s" || arg == "--stats") {
            modes |= OP_STATS;

        } else if (arg == "-e" || arg == "--empty") {
            incEmpty = true;

        } else if (arg == "-f" || arg == "--full") {
            incFull = true;

        } else if (startsWith(arg, "-r:") || startsWith(arg, "--start:")) {
            arg = substitute(substitute(arg, "-r:", ""), "--start:", "");
            if (!isUnsigned(arg))
                return usage(arg + " does not appear to be a valid number. Quitting...\n");
            startBlock = str_2_Uint(arg);

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--stop:")) {
            arg = substitute(substitute(arg, "-p:", ""), "--stop:", "");
            if (!isUnsigned(arg))
                return usage(arg + " does not appear to be a valid number. Quitting...\n");
            stopBlock = str_2_Uint(arg);

        } else if (startsWith(arg, "-k:") || startsWith(arg, "--skip:")) {
            arg = substitute(substitute(arg, "-k:", ""), "--skip:", "");
            if (!isUnsigned(arg))
                return usage(arg + " does not appear to be a valid number. Quitting...\n");
            skip = str_2_Uint(arg);

        } else if (arg == "--fix") {

            return usage("The --fix option is disabled.\n");
#if 0
            if (fileExists(fullBlockIndex + ".lck"))
                return usage("fullBlockIndex is locked. Quitting...");

            if (fileExists(fullBlockIndex)) {
                cerr << "Are you sure you want to fix the fullBlock index? (y=yes)\n> ";
                int ch = getchar();
                if (ch == 'y' || ch == 'Y') {
                    handle_remove_dups(*this);
                } else {
                    cerr << "Not removed. Quitting.\n";
                }
            }
            return false;
#endif

        } else {
            if (!builtInCmd(arg))
                return usage("Invalid option: " + arg);
        }
    }

    if (!fileExists(fullBlockIndex))
        return usage("Could not open file '" + fullBlockIndex + "'. Quitting...");

    if (stopBlock <= startBlock)
        return usage("--startBlock must preceed --stopBlock. Quitting...");
    nBlocks = stopBlock - startBlock;

    if (verbose)
        modes |= OP_LIST;

    if (!modes)
        modes = OP_STATS;

    if (!incEmpty && !incFull)
        incEmpty = incFull = true;

    if (skip < 1)
        skip = 1;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    modes      = 0;
    incEmpty   = false;
    incFull    = false;
    startBlock = 0;
    nBlocks    = 0;
    skip       = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
