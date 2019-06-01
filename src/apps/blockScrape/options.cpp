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

    // Establish the folders that hold the data...
    establishFolder(indexFolder_sorted);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_pending);
    establishFolder(configPath("cache/tmp"));
    if (writeBlocks)
        establishFolder(blockFolder);

    CBlock latest;
    getBlock_light(latest, "latest");
    latestBlockTs = latest.timestamp;

    blknum_t pending, staging, finalized, client;
    getLastBlocks(pending, staging, finalized, client);

    // Find out where to start and stop
    if (startBlock == NOPOS && endBlock != NOPOS) {

        startBlock = staging + 1;
        if (startBlock >= endBlock)
            return usage("--start must be before --end. Quitting...");

    } else if (startBlock != NOPOS && endBlock != NOPOS) {

        // The user told us where to start and stop. This is okay, but check for bogus data...
        if (startBlock >= endBlock)
            return usage("--start must be before --end. Quitting...");

        if (startBlock > staging)
            return usage("--start must be later than or equal to the last block already in the cache. Quitting...");

    } else if (startBlock != NOPOS) {

        endBlock = max(startBlock + 1, client);

     } else {

         startBlock = staging + 1;
         endBlock = max(startBlock + 1, client);

    }

    // The first time we ever run, the user has a chance to tell us where to start
    uint64_t forceStart = getGlobalConfig("blockScrape")->getConfigInt("settings", "forceStartBlock", NOPOS);
    if (startBlock == 1 && forceStart != NOPOS) {
        // TODO(tjayrush): This should snap to a legit index file start point
        startBlock = forceStart;
        endBlock = max(startBlock + 1, client);
    }

    // No more than maxBlocks after start
    endBlock = min(endBlock, startBlock + maxBlocks);

    if (!isParity() || !nodeHasTraces())
        return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    maxIndexRows = getGlobalConfig("blockScrape")->getConfigInt("settings", "maxIndexRows", maxIndexRows);

    string_q zeroBin = indexFolder_finalized + padNum9(0) + "-" + padNum9(0) + ".bin";
    if (!fileExists(zeroBin)) {
        ASSERT(prefunds.size() == 8893);  // This is a known value
        CStringArray appearances;
        for (auto prefund : prefunds) {
            CStringArray parts;
            explode(parts, prefund, '\t');
            // The prefund transactions have a zero for thier block numbers and an index
            // into thier location in the list of presale addresses. We need to do this so we
            // can distringuish them when they are exported.
            ostringstream os;
            os << parts[0] << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances.size()) << endl;
            appearances.push_back(os.str());
        }
        writeIndexAsBinary(zeroBin, appearances); // also writes the bloom file
    }

    if (startBlock > client)
        cerr << cTeal << "INFO: " << cOff << "The scraper is at the front of the chain." << endl;
    return startBlock <= client;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RUNONCE | OPT_PREFUND);

    startBlock   = NOPOS;
    endBlock     = NOPOS;
    maxBlocks    = NOPOS;
    writeBlocks  = false;
    minArgs      = 0;
    maxIndexRows = 350000;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
