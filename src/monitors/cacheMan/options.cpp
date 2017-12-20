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
    CParams("~filenames",      "path(s) of files to check, merge, fix or display (default=display)"),
    CParams("-check",          "check for duplicates and other problems in the cache"),
    CParams("-data",           "in 'list' mode, render results as data (i.e export mode)"),
    CParams("-fix",            "remove duplicates from the cache (if any)"),
    CParams("-list",           "list the contents of the cache (the default if no other option)"),
    CParams("-stats",          "report statistics on the cache"),
    CParams("-extract:<num>",  "extract transactions for the given contract :id"),
    CParams("-truncate:<num>", "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
    CParams("-merge",          "merge two or more caches into a single cache"),
    CParams("@s(k)ip",         "skip value for testing"),
    CParams("@spl(i)t",        "split the cache by address"),
    CParams("",                "Show the contents of an account cache and/or fix it by removing duplicate records.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-c" || arg == "--check") {
            mode = "check|" + mode;  // always do 'checks' first

        } else if (arg == "-f" || arg == "--fix") {
            if (!mode.Contains("fix"))
                mode += "fix|";
            mode.Replace("list|fix", "fix|list");  // always do 'fixes' first

        } else if (arg == "-l" || arg == "--list") {
            mode += "list|";  // do 'listing' in order found

        } else if (arg == "-s" || arg == "--stats") {
            wantsStats = true;

        } else if (arg.Contains("-t")) {
            arg = arg.Substitute("-t", "").Substitute("--truncate", "");
            if (!arg.startsWith(":"))
                return usage("You must supply a block number with the --truncate:n command.");
            nextTokenClear(arg, ':');
            trunc = toLong32u(arg);
            if (trunc < 1)
                return usage("You must supply a block number with the --truncate:n command.");
            if (trunc > getLatestBlockFromClient())
                return usage("You must supply a block number lower than the latest block.");
            if (!mode.Contains("fix"))
                mode += "fix|";
            mode.Replace("list|fix", "fix|list");  // do 'fixing' prior to 'listing'

        } else if (arg.Contains("-e")) {
            arg = arg.Substitute("-e", "").Substitute("--extract", "");
            if (!arg.startsWith(":"))
                return usage("You must supply a contract address to extract: --extract:id.");
            nextTokenClear(arg, ':');
            extractID = toLong32u(arg);
            isExtract = true;

        } else if (arg == "-m" || arg == "--merge") {
            isMerge = true;

        } else if (arg == "-i" || arg == "--split") {
            isSplit = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;
            colorsOff();

        } else if (arg.startsWith("-k:") || arg.startsWith("--skip:")) {
            SFString arg1 = arg.Substitute("-k:","").Substitute("--skip:","");
            skip = toUnsigned(arg1);
            if (!skip)
                skip = 1;

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            SFString path = arg;
            filenames[filenames.getCount()] = path;
            if (!fileExists(path))
                return usage("Cannot open file: " + path + ". Quitting.");
        }
    }

    if (filenames.getCount() == 0)
        return usage("You must provide at least one filename. Quitting.");
    if (mode.empty() && !wantsStats && !isMerge && !isExtract && !isSplit)
        return usage("You must provide some combination of --list, --check, or --fix. Quitting...");
    if (isMerge && filenames.getCount() < 2)
        return usage("Merge command needs at least two filenames. Quitting.");
    if (isSplit && filenames.getCount() != 1)
        return usage("Split command requires a single filename. Quitting.");

    // Merge and extract are handled in main, split is handled right here
    if (isSplit) {
        processSplit(*this);
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    filenames.Clear();
    mode = "";
    trunc = 0;
    isMerge = false;
    isSplit = false;
    isExtract = false;
    extractID = NOPOS;
    asData = false;
    wantsStats = false;
    skip = 1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
