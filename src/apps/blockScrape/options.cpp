/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("-start:<num>",     "first block to visit (default: last visited block + 1)"),
    COption("-end:<num>",       "last block to visit (required if --start supplied)"),
    COption("-maxBlocks:<num>", "maximum number of blocks to process (defaults to 5000)"),
    COption("",                 "Decentralized blockchain scraper and block cache.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, "-s:") || startsWith(arg, "--start:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--start:","");
            if (!isUnsigned(arg))
                return usage("--start must be a non-negative number. Quitting...");
            startBlock = str_2_Uint(arg);

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--end:")) {
            arg = substitute(substitute(arg, "-e:", ""), "--end:", "");
            if (!isUnsigned(arg))
                return usage("--end must be a non-negative number. Quitting...");
            endBlock = str_2_Uint(arg);

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--maxBlocks:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--maxBlocks:", "");
            if (!isUnsigned(arg))
                return usage("--maxBlocks must be a non-negative number. Quitting...");
            maxBlocks = str_2_Uint(arg);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    // The maximum number of blocks to process, only pick up from config if user has not told us the value
    if (maxBlocks == NOPOS)
        maxBlocks = getGlobalConfig("blockScrape")->getConfigInt("settings", "maxBlocks", 500);

    // Allow the user to tell us that they want to write the block cache
    writeBlocks = getGlobalConfig("blockScrape")->getConfigBool("settings", "writeBlocks", writeBlocks);

    // 'to' addresses (if any) to ignore (helps exclude dDos transactions)
    if (getGlobalConfig("blockScrape")->getConfigBool("exclusions", "enable", false))
        exclusionList = toLower(getGlobalConfig("blockScrape")->getConfigStr("exclusions", "list", ""));

    // Establish the folders that hold the data...
    establishFolder(indexFolder_sorted_v2);
    establishFolder(indexFolder_finalized_v2);
    establishFolder(indexFolder_staging_v2);
    establishFolder(indexFolder_v2 + "tmp/");

    CBlock latest;
    getBlock(latest, "latest");
    latestBlockTs = latest.timestamp;

    blknum_t staging, finalized, client;
    getLastBlocks(staging, finalized, client);

    // Find out where to start and stop
    if (startBlock == NOPOS && endBlock != NOPOS) {

        // User tried to tell us where to stop, but didn't tell us where to start. Not okay.
        return usage("You may only specify an --end block if you've given a --start block. Quitting...");

    } else if (startBlock != NOPOS && endBlock != NOPOS) {

        // The user told us where to start and stop. This is okay, but check for bogus data...
        if (startBlock >= endBlock)
            return usage("--start must be before --end. Quitting...");

        if (startBlock > finalized)
            return usage("--start must be later than or equal to the last block already in the cache. Quitting...");

    } else if (startBlock != NOPOS) {

        endBlock = max(startBlock + 1, client);

     } else {

         startBlock = finalized + 1;
         endBlock = max(startBlock + 1, client);

    }

    // If we're at the beginning, and we've been told explicitly where to start, start there...
    uint64_t forceStart = getGlobalConfig("blockScrape")->getConfigInt("settings", "forceStartBlock", NOPOS);
    if (startBlock == 1 && forceStart != NOPOS) {
        startBlock = forceStart;
        endBlock = max(startBlock + 1, client);
    }

    // No more than maxBlocks after start
    endBlock = min(endBlock, startBlock + maxBlocks);

    if (!isParity() || !nodeHasTraces())
        return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    maxIndexBytes = getGlobalConfig("blockScrape")->getConfigInt("settings", "maxIndexBytes", maxIndexBytes);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RUNONCE);

    startBlock    = NOPOS;
    endBlock      = NOPOS;
    maxBlocks     = NOPOS;
    writeBlocks   = false;
    minArgs       = 0;
    maxIndexBytes = 50000000;  // 50 MB
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
