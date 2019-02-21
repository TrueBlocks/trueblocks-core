/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("-useIndex",         "search for transactions using the address index"),
    COption("-maxBlocks:<val>",  "scan at most --maxBlocks blocks ('all' implies scan to end of chain)"),
    COption("-writeBlocks",      "write binary blocks to cache (default: do not write blocks)"),
    COption("-logLevel:<val>",   "specify the log level (default 1)"),
    COption("@noBloom(s)",       "do not use adaptive enhanced blooms (much faster if you use them)"),
    COption("@noBloc(k)s",       "do not use binary block cache (much faster if you use them)"),
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
            ignoreBlkCache = true;

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

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);

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

    if (useIndex && !folderExists(indexFolder_prod))
        return usage("Address index path `" + indexFolder_prod + "' not found. Quitting...");

    if (ignoreBlkCache) {
        cerr << "Switching to local node, ignoring binary block cache" << endl;
        setDataSource("local");
    }

    if (!isTestMode())
        cerr << bBlack << Now().Format(FMT_JSON) << cOff << ": Monitoring " << cYellow << getCWD() << cOff << "             \n";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    // We want to be able to run this more than once
    // optionOn(OPT_RUNONCE);

    lastBlock      = 0;
    minWatchBlock  = 0;
    maxWatchBlock  = UINT32_MAX;
    maxBlocks      = 10000;
    minArgs        = 0;
    lastTimestamp  = 0;
    writeBlocks    = false;
    ignoreBlooms   = false;
    useIndex       = false;
    isList         = false;
    ignoreBlkCache = false;
    firstBlock     = 0;
    nBlocks        = 0;
    blockCounted   = false;
    logLevel       = 1;
    monitorAddr    = "";
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
        if (isTestMode())
            cout << config << endl;
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COptions& item) {
    os << bBlack
        << " (bh.bq.bs.nb: "  << item.blkStats.nHit << "/" << item.blkStats.nQueried << "/" << item.blkStats.nSeen << "/" << item.nBlocks
        << ", ah.anh.as: "    << item.addrStats.nHit  << "/" << item.addrStats.nSkipped   << "/" << item.addrStats.nSeen
        << ", th.ts: "        << item.transStats.nHit  << "/" << item.transStats.nSeen
        << ", trh.trsk.trs: " << item.traceStats.nHit << "/" << item.traceStats.nSkipped << "/" << item.traceStats.nSeen << ")" << cOff;
    return os;
}

//-----------------------------------------------------------------------
string_q COptions::finalReport(double startTime, bool header) const {
    colorsOff();
    ostringstream os;
    if (header) {
        os << "date\t";
        os << "name\t";
        os << "address\t";
        os << "n-accts\t";
        os << "timing\t";
        os << "firstBlock\t";
        os << "nBlocks\t";
        os << "blksHit\t";
        os << "blksQueried\t";
        os << "blksSeen\t";
        os << "transHit\t";
        os << "transSeen\t";
        os << "tracesHit\t";
        os << "tracesSkipped\t";
        os << "tracesSeen\n";
    } else {
        os << substitute(Now().Format(FMT_JSON), " UTC", "") << "\t";
        os << monitors[0].name.substr(0,15) << "\t";
        os << monitors[0].address << "\t";
        os << monitors.size() << "\t";
        os << double_2_Str(max(0.0, qbNow() - startTime), 4) << "\t";
        os << firstBlock << "\t";
        os << nBlocks << "\t";
        os << blkStats.nHit << "\t";
        os << blkStats.nQueried << "\t";
        os << blkStats.nSeen << "\t";
        os << transStats.nHit << "\t";
        os << transStats.nSeen << "\t";
        os << traceStats.nHit << "\t";
        os << traceStats.nSkipped << "\t";
        os << traceStats.nSeen << "\n";
    }
    return os.str();
}
