/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("-maxBlocks:<val>",  "the maximum number of blocks to visit during this run"),
    COption("-oneBlock:<val>",   "check if the block would be a hit"),
    COption("-oneTra(n)s:<val>", "check if the block and transaction would be a hit"),
    COption("-writeBlocks",      "write binary blocks to cache (default: do not write blocks)"),
    COption("-maxBlocks:<val>",  "scan at most --maxBlocks blocks ('all' implies scan to end of chain)"),
    COption("@noBloom(s)",       "do not use adaptive enhanced blooms (much faster if you use them)"),
    COption("@noBloc(k)s",       "do not use binary block cache (much faster if you use them)"),
    COption("@logLevel:<val>",   "specify the log level (default 1)"),
    COption("@useIndex",         "search for transactions using the address index"),
    COption("@l(i)st",           "ignore all other options and export tab delimited bn.txid records"),
    COption("",                  "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool makeMonitorFolder(const address_t& monitorAddr);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isAll = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "--all" || arg == "-a") arg = "--maxBlocks:all";
        if (startsWith(arg, "-m:") || startsWith(arg, "--maxBlocks:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--maxBlocks:", "");
            if (arg == "all") {
                isAll = true;
            } else {
                if (isUnsigned(arg))
                    maxBlocks = str_2_Uint(arg);
                else
                    return usage("Please provide an integer value of maxBlocks. Quitting...");
            }

        } else if (startsWith(arg, "-o:") || startsWith(arg, "--oneBlock:")) {
            arg = substitute(substitute(arg, "-o:", ""), "--oneBlock:", "");
            if (isUnsigned(arg))
                oneBlock = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for oneBlock. Quitting...");

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--oneTrans:")) {
            arg = substitute(substitute(arg, "-n:", ""), "--oneTrans:", "");
            if (isUnsigned(arg))
                oneTrans = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for oneTrans. Quitting...");

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--level:")) {
            arg = substitute(substitute(arg, "-l:", ""), "--level:", "");
            if (isUnsigned(arg))
                logLevel = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for --logLevel. Quitting...");

        } else if (arg == "-s" || arg == "--noBlooms") {
            ignoreBlooms = true;

        } else if (arg == "-i" || arg == "--list") {
            isList = true;
            useIndex = true;

        } else if (arg == "-u" || arg == "--useIndex") {
            useIndex = true;

        } else if (arg == "-k" || arg == "--noBlocks") {
            ignoreBlockCache = true;

        } else if (arg == "-w" || arg == "--writeBlocks") {
            writeBlocks = true;

        } else if (startsWith(arg, "0x")) {

            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
            if (!monitorAddr.empty())
                return usage("Please provide only a single address. Quitting...");
            monitorAddr = toLower(arg);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            return usage("Invalid option: " + arg);
        }
    }

//#define WHICH_BLOCK 4759050
//#define WHICH_TRANS 199
#ifdef WHICH_BLOCK
    visitor.opts.oneBlock = WHICH_BLOCK;
#ifdef WHICH_TRANS
    visitor.opts.oneTrans = WHICH_TRANS;
#endif
#endif

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);

    if (oneTrans && !oneBlock)
        return usage("If you specify oneTrans, you must specify oneBlock. Quitting...");

    if (isAll && oneBlock)
        return usage("Choose either --all or --oneBlock, not both. Quitting...");

    CBlock latest;
    getBlock(latest, "latest");
    lastTimestamp = latest.timestamp;

    if (isAll)
        maxBlocks = INT_MAX;

    // Note: Pick up these defaults from the blockScrape application settings.

    // If writeBlocks is true, then the user told us what to do on the command line and we obey them. If it's false
    // then we check to see if the config file for blockScrape is set to write blocks. If the blockScraper is writing
    // blocks, we don't have to. If not, then we do want to write blocks here (therefore we negate the config value).
    if (!writeBlocks)
        writeBlocks = !getGlobalConfig("blockScrape")->getConfigBool("settings", "writeBlocks", true);

    // Exclusions are always picked up from the blockScraper
    exclusions = toLower(getGlobalConfig("blockScrape")->getConfigStr("exclusions", "list", ""));

    // If we're doing listing, we need a config file.
    if (isList) {
        if (!monitorAddr.empty()) {
            // the user wants to create a new monitor, so accomodate him.
            if (!makeMonitorFolder(monitorAddr))
                return usage("Could not create monitor folder. Quitting...");
            if (isTestMode())
                return false;
            if (toml) {
                delete toml;
            }
            toml = new CToml("./config.toml");
        }
    } else {
        if (!monitorAddr.empty())
            return usage("You may provide an address only with the --list option. Quitting...");
    }

    if (!toml || !fileExists(toml->getFilename()))
        return usage("Cannot read toml file './config.toml'. Are you in the right folder? Quitting...\n");

    manageFields(toml->getConfigStr("fields", "hide", ""), false);
    manageFields(toml->getConfigStr("fields", "show", ""), true );

    if (!isParity() || !nodeHasTraces())
        return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    if (!folderExists(getTransCachePath(""))) {
        cerr << "The cache folder '" << getTransCachePath("") << "' not found. Trying to create it." << endl;
        establishFolder(getTransCachePath(""));
        if (!folderExists(getTransCachePath("")))
            return usage("The cache folder '" + getTransCachePath("") + "' not created. Quiting...");
    }

    if (!loadMonitors())
        return false;

    cacheFilename = getTransCachePath(monitors[0].address);
    if (fileExists(cacheFilename+".lck") || fileExists(getTransCachePath("lastBlock.txt.lck")))
        return usage("The cache file '" + (cacheFilename+".lck") + "' is locked. Quitting...");

    string_q bloomPath = blockCachePath("/blooms/");
    if (!useIndex && !folderExists(bloomPath))
        return usage("The bloom file cache '" + bloomPath + "' was not found. Quitting...");

    addrIndexPath = blockCachePath("addr_index/sorted_by_addr/");
    if (useIndex && !folderExists(addrIndexPath))
        return usage("Address index path `" + addrIndexPath + "' not found. Quitting...");

    if (ignoreBlockCache) {
        cerr << "Switching to local node, ignoring binary block cache" << endl;
        setDataSource("local");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    lastBlock     = 0;
    minWatchBlock = 0;
    maxWatchBlock = UINT32_MAX;
    maxBlocks     = 10000;
    minArgs       = 0;
    oneBlock      = 0;
    oneTrans      = 0;
    lastTimestamp = 0;
    writeBlocks   = false;
    ignoreBlooms  = false;
    useIndex      = false;
    isList        = false;
    ignoreBlockCache = false;
    firstBlock    = 0;
    nBlocks       = 0;
    blockCounted  = false;
    debugging     = 0;
    logLevel      = 1;
//    addrIndexPath = "";
//    monitorAddr = "";
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : blkStats(), addrStats(), transStats(), traceStats(), txCache(WRITING_ARCHIVE) {
    Init();
    toml = new CToml("./config.toml");
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    if (toml)
        delete toml;
}

//--------------------------------------------------------------------------------
static const char *STR_CONFIG_FILE =
"[settings]\n"
"name = NAME\n"
"\n"
"[display]\n"
"accounting = false\n"
"logs = false\n"
"trace = false\n"
"single = false\n"
"parse = false\n"
"json = true\n"
"\n"
"[formats]\n"
"trans_fmt = [{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t[{ISERROR}]\t[{EVENTS}]\t[{TRACES}]\n"
"\n"
"[[watches]]\n"
"    list = [ { address = \"ADDR\", name = \"NAME\", firstBlock = 0 }\n"
"]\n";

//--------------------------------------------------------------------------------
bool makeMonitorFolder(const address_t& addr) {
    string_q name = toUpper(addr.substr(2,4) + "..." + addr.substr(addr.length()-4,4));
    string_q path = "./" + toLower("99_" + name) + "/";
    establishFolder(path);
    establishFolder(path + "cache/");
    if (!chdir(path.c_str())) {
        string_q config = STR_CONFIG_FILE;
        config = substitute(config, "NAME", name);
        config = substitute(config, "ADDR", addr);
        stringToAsciiFile("./config.toml", config);
        if (isTestMode()) {
            cout << getCWD() << "config.toml: " << fileExists("config.toml") << endl;
            cout << config << endl;
        }
        return true;
    }
    return false;
}
