/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~mode",            "one of 'check,' 'list,' 'freshen,' or 'refreshen'"),
    COption("-check:<blk>",     "check that empty blocks are empty, and visa versa. Optionally start at :block"),
    COption("-list",            "list all non-empty block numbers"),
    COption("-freshen",         "starting from the most recently visited block, append any new, non-empty block numbers to the index"),
    COption("-refreshen",       "remove the full-block index and re-create it"),
    COption("-maxBlocks:<val>", "number of blocks to process (defaults to 20000)"),
    COption("-deep",            "when doing 'check', do a deep check"),
    COption("@skip",            "export once every 5,000 lines (applies only to --verbose --list mode - used for testing)"),
    COption("",                 "Freshen or check the miniBlock database.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-l" || arg == "--list") {
            if (!fileExists(miniBlockCache) && !contains(mode, "freshen"))
                return usage("Could not open file '" + miniBlockCache + "' for listing. Quitting...\n");
            mode += "list|";

        } else if (arg == "-s" || arg == "--skip") {
            skip = 5000;
            verbose = true;

        } else if (arg == "-c" || arg == "--check" || startsWith(arg, "--check:")) {
            string_q orig = arg;
            if (!fileExists(miniBlockCache))
                return usage("Could not open file '" + miniBlockCache + "' for checking. Quitting...\n");
            replace(arg, "--check", "");
            replace(arg, "-c", "");
            if (!arg.empty() && !startsWith(arg, ':'))
                return usage("Invalid option: " + orig);
            arg = extract(arg, 1);
            startBlock = str_2_Uint(arg);
            if (startBlock > getLatestBlockFromCache())
                return usage("The --check:startBlock block value '" + uint_2_Str(startBlock) + "' is greater than the last block in the cache. Quitting...");
            // add to end
            mode += "check|";

        } else if (arg == "-d" || arg == "--deep") {
            deep = true;

        } else if (arg == "-f" || arg == "--freshen") {
            mode = "freshen|" + mode;

        } else if (arg == "-r" || arg == "--refreshen") {
            if (fileExists(miniBlockCache)
#ifdef MINI_TRANS
                || fileExists(miniTransCache)
#endif
                ) {
                cerr << "Are you sure you want to remove the mini-block index? (y=yes)\n> ";
                int ch = getchar();
                if (ch == 'y' || ch == 'Y') {
                    remove(miniBlockCache.c_str());
#ifdef MINI_TRANS
                    remove(miniTransCache.c_str());
#endif
                } else {
                    cerr << "Not removed. Quitting.\n";
                    cerr.flush();
                    exit(0);
                }
            }
            // freshen first so it goes first
            mode = "freshen|" + mode;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--maxBlocks:")) {
            nextTokenClear(arg,':');
            if (isUnsigned(arg))
                maxBlocks = str_2_Uint(arg);
            else
                return usage("Please provide an integer value of maxBlocks. Quitting...");

        } else if (startsWith(arg, '-')) {

            if (!contains(arg, "-h") && !contains(arg, "-v") && !contains(arg, "-t"))
                return usage("Invalid option: " + arg);

        } else {
            mode += (arg + "|");
        }
    }

    bool hasCheck = contains(mode, "check");
    bool hasFresh = contains(mode, "freshen");
    bool hasList  = contains(mode, "list");

    // fixup so things happen in a sensible order
    mode  = (hasFresh ? "freshen|" : "");
    mode += (hasCheck ? "check|"   : "");
    mode += (hasList  ? "list|"    : "");
    if (mode.empty())
        return usage("Mode must be one of --list, --check, --freshen, or --refreshen.");

    if (deep && !hasCheck)
        return usage("--deep only applies for mode 'check'");
    if (skip != 1 && (!verbose || mode != "list|"))
        return usage("Skip may only be used in --verbose --list mode");

    if (maxBlocks == 20000) // Pick up the config file if not on command line
        maxBlocks = getGlobalConfig("miniBlocks")->getConfigInt("settings", "maxBlocks", maxBlocks);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;

    mode = "";
    deep = false;
    skip = 1;
    startBlock = 0;
    maxBlocks = 20000;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // Cannot process anything if the files are locked.
    if (isFileLocked(miniBlockCache)) {
        cerr << "The miniBlockCache (" << miniBlockCache << ") is locked. The program cannot be run. Quitting...\n";
        cerr.flush();
        exit(0);
#ifdef MINI_TRANS
    } else if (isFileLocked(miniTransCache)) {
        cerr << "The miniTransCache (" << miniTransCache << ") is locked. The program cannot be run. Quitting...\n";
        cerr.flush();
        exit(0);
#endif
    }
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
