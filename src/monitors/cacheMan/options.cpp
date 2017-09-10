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
    CParams("-blockOnly",      "in 'list' mode, render block number only (used for testing)"),
    CParams("-check",          "check for duplicates and other problems in the cache"),
    CParams("-fix",            "remove duplicates from the cache (if any)"),
    CParams("-list",           "list the contents of the cache (the default if no other option)"),
    CParams("-stats",          "report statistics on the cache"),
    CParams("-extract:<num>",  "extract transactions for the given contract :id"),
    CParams("-truncate:<num>", "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
    CParams("-merge",          "merge two or more caches into a single cache"),
//    CParams("-renumber",     "renumber contract is from :old-new. (You may specify multiple pairs separated by commas.)"),
    CParams("@s(k)ip",         "skip value for testing"),
//  CParams("-remove:addr",    "remove any transaction included because of this address (disabled)"),
    CParams("",                "Show the contents of an account cache and/or fix it by removing duplicate records.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

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
            mode = "extract";  // 'extract' must be stand alone

        } else if (arg.startsWith("-r") || arg.startsWith("-renum")) {
            arg = arg.Substitute("-r", "").Substitute("--renum", "");
            if (!arg.startsWith(":"))
                return usage("You must supply both an old and new contract id: --renum:old-new.");
            if (!arg.Contains("-"))
                return usage("Use '-' to separate old and new in --renum:old-new.");

            // User may specify either -r:old-new,old-new or multiple -r:old-new pairs
            // At this point 'arg' starts with ':' and may contain any number of old-new pairs
            // of renumbers separated by commas.
            nextTokenClear(arg, ':');
            while (!arg.empty()) {
                if (!arg.Contains("-"))
                    return usage("You must supply both an old and new contract id: --renum:old-new.");
                renums[renums.getCount()] = toLong32u(nextTokenClear(arg, '-'));
                renums[renums.getCount()] = toLong32u(nextTokenClear(arg, ','));
            }
            mode = "renum";  // 'renum' must be stand alone

        } else if (arg == "-m" || arg == "--merge") {
            isMerge = true;

        } else if (arg == "-b" || arg == "--blockOnly") {
            blockOnly = true;

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
    if (mode.empty())
        mode = "list";

    if (isMerge && filenames.getCount() < 2)
        return usage("Merge command needs at least two filenames. Quitting.");
    // The actual merge is handled in caller

    if (mode.Contains("extract"))
        mode = "extract";  // if extract is used at all, use it alone

    if (mode.Contains("renum")) {
        for (uint32_t i = 0 ; i < renums.getCount() ; i++)
            cout << renums[i] << ":";
        cout << "\n";
        mode = "renum|list";  // if renum is used at all, use it alone
        if (renums.getCount() % 2)
            return usage("With --renum, you must specify both old and new ids. Use --renum:old:new. Quitting.");
        for (uint32_t i = 0 ; i < renums.getCount() ; i++) {
            if (renums[i] < 0)
                return usage("Renum ids must be greater than zero. Quitting.");
            for (uint32_t j = 0 ; j < renums.getCount() ; j++) {
                if (i != j && renums[i] == renums[j])
                    return usage("Use a contract id only once in a renumbering scheme. Run cacheMan twice instead.");
            }
        }
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
    extractID = NOPOS;
    isMerge = false;
    blockOnly = false;
    wantsStats = false;
    renums.Clear();
    skip = 1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
