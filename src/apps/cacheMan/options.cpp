/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    COption("filenames", "", "list<string>", OPT_REQUIRED | OPT_POSITIONAL, "path(s) of files to check, merge, fix or display (default=display)"),
    COption("check", "c", "", OPT_SWITCH, "check for duplicates and other problems in the cache"),
    COption("data", "d", "", OPT_SWITCH, "in 'list' mode, render results as data (i.e export mode)"),
    COption("sort", "s", "", OPT_SWITCH, "sort the list of transactions and re-write (precludes other modes, other than --dedup)"),
    COption("fix", "f", "", OPT_SWITCH, "remove duplicates from the cache (if any)"),
    COption("list", "l", "", OPT_SWITCH, "list the contents of the cache (the default if no other option)"),
    COption("import", "i", "", OPT_SWITCH, "import transactions if import.txt file exists in current folder"),
    COption("remove", "r", "", OPT_SWITCH, "remove transactions if remove.txt file exists in current folder"),
    COption("truncate", "u", "<blknum>", OPT_FLAG, "truncate the cache at block :n (keeps block 'n' and before, implies --fix)"),
    COption("merge", "m", "", OPT_SWITCH, "merge two or more caches into a single cache"),
    COption("skip", "k", "<uint32>", OPT_HIDDEN | OPT_FLAG, "skip value for testing"),
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Show the contents of an account cache and/or fix it by removing duplicate records."),
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER8("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG8(false);

    // BEG_CODE_LOCAL_INIT
    CStringArray strings;
    bool check = false;
    bool sort = false;
    bool fix = false;
    bool list = false;
    bool import = false;
    bool remove = false;
    bool merge = false;
    // END_CODE_LOCAL_INIT

    blknum_t latest = getLastBlock_client();

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (arg == "-d" || arg == "--data") {
            data = true;

        } else if (arg == "-s" || arg == "--sort") {
            sort = true;

        } else if (arg == "-f" || arg == "--fix") {
            fix = true;

        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-i" || arg == "--import") {
            import = true;

        } else if (arg == "-r" || arg == "--remove") {
            remove = true;

        } else if (startsWith(arg, "-u:") || startsWith(arg, "--truncate:")) {
            if (!confirmBlockNum("truncate", truncate, arg, latest))
                return false;

        } else if (arg == "-m" || arg == "--merge") {
            merge = true;

        } else if (startsWith(arg, "-k:") || startsWith(arg, "--skip:")) {
            if (!confirmUint("skip", skip, arg))
                return false;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmBlockNum("start", start, arg, latest))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmBlockNum("end", end, arg, latest))
                return false;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseStringList2(this, strings, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    for (auto string : strings) {
        address_t addr = toLower(substitute(string, ".acct.bin", ""));
        if (!isAddress(addr))
            EXIT_USAGE("Option '" + string + "' does not appear to be an address.");

        // Command line and chifra send in straight addresses, some test cases may send a local file...
        string_q path = (fileExists(string + ".acct.bin") ? (string + ".acct.bin") : getMonitorPath(addr));
        if (!fileExists(path)) // Hack alert: some weird test cases send in 'merged' as the address.
            EXIT_USAGE("Monitor file for '" + string + "' does not exist.");

        monitors.push_back(CAccountWatch(addr, path));
    }

    if (list) {
        mode += "list|";  // do 'listing' in order found
        exportFmt = JSON1;
    }

    if (data)
        colorsOff();

    if (fix) {
        if (!contains(mode, "fix"))
            mode += "fix|";
        replace(mode, "list|fix", "fix|list");  // always do 'fixes' first
    }

    if (truncate) {
        if (!contains(mode, "fix"))
            mode += "fix|";
        replace(mode, "list|fix", "fix|list");  // do 'fixing' prior to 'listing'
    }

    if (check)
        mode = "check|" + mode;  // always do 'checks' first

    if (monitors.size() == 0 && !import)
        return usage("You must provide at least one filename. Quitting...");
    if (mode.empty())
        mode = "list|";
    if (merge && monitors.size() < 2)
        return usage("Merge command needs at least two filenames. Quitting...");
    if ((sort || remove) && monitors.size() != 1)
        return usage("Command requires a single filename. Quitting...");

    if (sort) {
        handleSort();
        return false;

    } else if (merge) {
        handleMerge();
        return false;

    } else if (import) {
        if (!fileExists("./import.txt"))
            return usage("File ./import.txt not found. Quitting...");
        if (monitors.empty() || monitors.size() > 1)
            return usage("Please provide a single address for this import.");
        handleImport();
        return false;

    } else if (remove) {
        if (!fileExists("./remove.txt"))
            return usage("File ./remove.txt not found in local folder. Quitting...");
        cerr << cGreen << "Found removal file...\n";
        string_q contents;
        asciiFileToString("./remove.txt", contents);
        CStringArray lines;
        explode(lines, contents, '\n');
        for (auto line : lines) {
            CAppearance_base item(line);
            if (item.blk > 0) {
                removals.push_back(item);
                LOG_INFO(cYellow, "\tremoval instruction: ", cTeal, removals.size(), "-", item.blk, ".", item.txid, cOff, "\r");
            }
        }
        if (fileExists("./removed.txt"))
            ::remove("./remove.txt");
        ::sort(removals.begin(), removals.end());
        if (removals.size() > 0)
            handleRemove();
        return false;

    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);

    // BEG_CODE_INIT
    data = false;
    truncate = 0;
    skip = 1;
    start = NOPOS;
    end = NOPOS;
    // END_CODE_INIT

    monitors.clear();
    mode = "";
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // END_CODE_NOTES
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool COptions::loadMonitorData(CAppearanceArray_base& items, const address_t& addr) {
    ENTER8("loadMonitorData");
    string_q fn = getMonitorPath(addr);
    if (!fileExists(fn)) {
        replace(fn, getMonitorPath(""), "./");
        if (!contains(fn, ".acct.bin"))
            fn += ".acct.bin";
        if (!fileExists(fn))
            EXIT_FAIL("Failed to lock file '" + fn + "'. Quitting...\n");
    }

    size_t nRecords = (fileSize(fn) / sizeof(CAppearance_base));
    ASSERT(nRecords);
    CAppearance_base *buffer = new CAppearance_base[nRecords];
    if (buffer) {
        bzero(buffer, nRecords * sizeof(CAppearance_base));
        CArchive txCache(READING_ARCHIVE);
        if (txCache.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
            txCache.Read(buffer, sizeof(CAppearance_base), nRecords);
            txCache.Release();
        } else {
            EXIT_FAIL("Failed to lock file '" + fn + "'. Quitting...\n");

        }
        // Add to the items which may be non-empty
        items.reserve(items.size() + nRecords);
        for (size_t i = 0 ; i < nRecords ; i++)
            items.push_back(buffer[i]);
        delete [] buffer;
    } else {
        EXIT_FAIL("Could not allocate memory for address " + addr + "Quitting...\n");
    }
    EXIT_NOMSG8(true);
}
