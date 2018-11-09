/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~filenames",        "path(s) of files to check, merge, fix or display (default=display)"),
    COption("-check",            "check for duplicates and other problems in the cache"),
    COption("-data",             "in 'list' mode, render results as data (i.e export mode)"),
    COption("-sort",             "sort the list of transactions and re-write (precludes other modes, other than --dedup)"),
    COption("-fix",              "remove duplicates from the cache (if any)"),
    COption("-list",             "list the contents of the cache (the default if no other option)"),
    COption("-cacheB(a)ls",      "cache per block account balances for each account"),
    COption("-balances",         "export account balances for each account"),
    COption("-import",           "import transactions if import.txt file exists in current folder"),
    COption("-remove",           "remove transactions if remove.txt file exists in current folder"),
    COption("-truncate:<num>",   "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
    COption("-maxBloc(k):<num>", "for testing, max block to visit"),
    COption("-merge",            "merge two or more caches into a single cache"),
    COption("@s(k)ip",           "skip value for testing"),
    COption("",                  "Show the contents of an account cache and/or fix it by removing duplicate records.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isMerge = false, isSort = false, isCacheBal = false, isBals = false;
    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-c" || arg == "--check") {
            mode = "check|" + mode;  // always do 'checks' first

        } else if (arg == "-f" || arg == "--fix") {
            if (!contains(mode, "fix"))
                mode += "fix|";
            replace(mode, "list|fix", "fix|list");  // always do 'fixes' first

        } else if (arg == "-l" || arg == "--list") {
            mode += "list|";  // do 'listing' in order found

        } else if (startsWith(arg, "-t:") || startsWith(arg, "--truncate:")) {
            arg = substitute(substitute(arg, "-t:", ""), "--truncate:", "");
            if (!isNumeral(arg))
                return usage("You must supply a block number with the --truncate:n command.");
            trunc = str_2_Uint(arg);
            if (trunc > getLatestBlockFromClient())
                return usage("You must supply a block number lower than the latest block.");
            if (!contains(mode, "fix"))
                mode += "fix|";
            replace(mode, "list|fix", "fix|list");  // do 'fixing' prior to 'listing'

        } else if (startsWith(arg, "-k:") || startsWith(arg, "--maxBlock:")) {
            arg = substitute(substitute(arg, "-k:", ""), "--maxBlock:", "");
            if (!isNumeral(arg))
                return usage("You must supply a block number with the --maxBlock:n command.");
            maxBlock = str_2_Uint(arg);
            if (maxBlock == 0)
                maxBlock = NOPOS;

        } else if (arg == "-m" || arg == "--merge") {
            isMerge = true;

        } else if (arg == "-b" || arg == "--balances") {
            isBals = true;

        } else if (arg == "-a" || arg == "--cacheBals") {
            isCacheBal = true;

        } else if (arg == "-i" || arg == "--import") {
            if (!fileExists("./import.txt"))
                return usage("File ./import.txt not found. Quitting...");
            isImport = true;

        } else if (arg == "-r" || arg == "--remove") {
            if (!fileExists("./remove.txt"))
                return usage("File ./remove.txt not found. Quitting...");
            cerr << cGreen << "Found removal file...\n";
            string_q contents;
            asciiFileToString("./remove.txt", contents);
            while (!contents.empty()) {
                string_q line  = nextTokenClear(contents,'\n');
                CAcctCacheItem item(line);
                removals.push_back(item);
                cerr << cYellow << "\tremoval instruction: " << cTeal << removals.size() << "-" << item << cOff << "\r";
                cerr.flush();
            }
            cerr << "                                                                \n";
            cerr.flush();
            if (!isTestMode()) {
                copyFile("./remove.txt", "remove.bak");
                remove("./remove.txt");
            }
            sort(removals.begin(), removals.end());
            isRemove = (removals.size() > 0);

        } else if (arg == "-s" || arg == "--sort") {
            isSort = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;
            colorsOff();

        } else if (startsWith(arg, "-k:") || startsWith(arg, "--skip:")) {
            string_q arg1 = substitute(substitute(arg, "-k:", ""), "--skip:", "");
            skip = str_2_Uint(arg1);
            if (!skip)
                skip = 1;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            string_q path = arg;
            filenames.push_back(path);
            if (!fileExists(path))
                return usage("Cannot open file: " + path + ". Quitting.");
            if (!contains(path, ".acct.bin"))
                return usage("cacheMan app only processes .acct.bin files. Quitting.");
        }
    }

    if (!isBals && filenames.size() == 0 && !isImport)
        return usage("You must provide at least one filename. Quitting.");
    if (mode.empty())
        mode = "list|";
    if (isMerge && filenames.size() < 2)
        return usage("Merge command needs at least two filenames. Quitting.");
    if ((isSort || isRemove || isCacheBal) && filenames.size() != 1)
        return usage("Command requires a single filename. Quitting.");

    if (isBals) {
        listBalances(*this);
        return false;

    } else if (isCacheBal) {
        handleCacheBals(*this);
        return false;

    } else if (isSort) {
        handleSort();
        return false;

    } else if (isMerge) {
        handleMerge();
        return false;

    } else if (isImport) {
        if (!fileExists("./config.toml"))
            return usage("Could not open file: ./config.toml. Quitting.");
        CToml toml("./config.toml");
        loadWatches(toml);
        if (filenames.empty())
            filenames.push_back("cache/" + watches[0].address + ".acct.bin");
        handleImport();
        return false;

    } else if (isRemove) {
        handleRemove();
        return false;

    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    filenames.clear();
    mode = "";
    trunc = 0;
    asData = false;
    maxBlock = NOPOS;
    skip = 1;
    isRemove = false;
    isImport = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool COptions::loadWatches(const CToml& toml) {

    // Note: this call does no checks on the data
    loadWatchList(toml, watches, "list");

    if (watches.size() == 0)
        return usage("Empty list of watches. Quitting...\n");

    // Check the watches for validity and pick up balance if possible
    for (uint32_t i = 0 ; i < watches.size() ; i++) {

        const CAccountWatch *watch = &watches[i];
        if (!isAddress(watch->address))
            return usage("Invalid watch address " + watch->address + "\n");

        if (watch->name.empty())
            return usage("Empty watch name " + watch->name + "\n");
    }
    return true;
}
