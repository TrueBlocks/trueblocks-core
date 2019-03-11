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
    COption("@for_addr:<val>",   "force a scrape on the given account"),
    COption("",                  "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//extern bool makeMonitorFolder(const address_t& addr);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isAll = false;
    blknum_t maxBlocks = 10000;
    blknum_t lastInCache = NOPOS;
    blknum_t lastVisited = NOPOS;

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

        } else if (startsWith(arg, "0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");
            CAccountWatch watch;
            watch.setValueByName("address", toLower(arg)); // don't change, sets bloom value also
            watch.setValueByName("name", toLower(arg));
            watch.extra_data = toLower("chifra/" + getVersionStr() + ": " + watch.address);
            watch.color = cBlue;
            watch.finishParse();
            monitors.push_back(watch);

        } else if (arg == "-u" || arg == "--useIndex") {
            useIndex = true;

        } else if (arg == "-k" || arg == "--noBlocks") {
            ignoreBlkCache = true;

        } else if (arg == "-w" || arg == "--writeBlocks") {
            writeBlocks = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            return usage("Invalid option: " + arg);
        }
    }

    if (monitors.size() == 0)
        return usage("You must provide at least one Ethereum address. Quitting...");

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);
    if (fileExists("./" + monitors[0].address + ".toml")) {
        CToml *toml = new CToml("./" + monitors[0].address + ".toml");
        monitors[0].toml = toml;  // watch descructor will clean this up
        manageFields(toml->getConfigStr("fields", "hide", ""), false);
        manageFields(toml->getConfigStr("fields", "show", ""), true );
        api_spec.method = toml->getConfigStr("api_spec", "method", "");
        api_spec.uri = toml->getConfigStr("api_spec", "uri", "");
        api_spec.headers = toml->getConfigStr("api_spec", "headers", "");
        if (!api_spec.uri.empty()) {
            for (size_t i = 0 ; i < monitors.size() ; i++) {
                monitors[i].abi_spec.loadAbiByAddress(monitors[i].address);
                monitors[i].abi_spec.loadAbiKnown("all");
            }
        }
    }

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

    if (!isParity() || !nodeHasTraces())
        return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    string_q transCachePath = getTransCachePath("");
    if (!folderExists(transCachePath)) {
        cerr << "The cache folder '" << transCachePath << "' not found. Trying to create it." << endl;
        establishFolder(transCachePath);
        if (!folderExists(transCachePath))
            return usage("The cache folder '" + transCachePath + "' not created. Quiting...");
    }

    string_q lb = getTransCacheLast(monitors[0].address);
    if (fileExists(getTransCachePath(monitors[0].address)+".lck") || fileExists(lb + ".lck"))
        return usage("The cache file '" + (getTransCachePath(monitors[0].address)+".lck") + "' is locked. Quitting...");

    string_q bloomPath = blockCachePath("/blooms/");
    if (!useIndex && !folderExists(bloomPath))
        return usage("The bloom file cache '" + bloomPath + "' was not found. Quitting...");

    if (useIndex && !folderExists(indexFolder_prod))
        return usage("Address index path `" + indexFolder_prod + "' not found. Quitting...");

    if (ignoreBlkCache) {
        cerr << "Switching to local node, ignoring binary block cache" << endl;
        setDataSource("local");
    }

    lastInCache = getLatestBlockFromCache();
    lastVisited = str_2_Uint(asciiFileToString(getTransCacheLast(monitors[0].address)));
    startScrape = min(lastVisited, lastInCache);
    scrapeCnt   = min(lastInCache - startScrape, maxBlocks);

    if (!isTestMode()) {
        cerr << bBlack << Now().Format(FMT_JSON) << cOff;
        cerr << ": Monitoring ";
        cerr << cYellow << getTransCachePath(monitors[0].address) << cOff;
        cerr << " (start: " << cTeal << startScrape << cOff;
        cerr << " end: " + cTeal << uint_2_Str(lastInCache) << cOff;
        if (!useIndex)
            cerr << " n: " + cTeal << uint_2_Str(scrapeCnt) << cOff;
        cerr << ")                    \n";
    }
    if (useIndex)
        scrapeCnt = 10000000;  // TODO(tjayrush): Not right

    return (lastVisited <= lastInCache);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    // We want to be able to run this more than once
    // optionOn(OPT_RUNONCE);

    minArgs        = 0;
    lastTimestamp  = 0;
    writeBlocks    = false;
    ignoreBlooms   = false;
    useIndex       = false;
    ignoreBlkCache = false;
    startScrape    = 0;
    scrapeCnt      = 0;
    blockCounted   = false;
    logLevel       = 1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : blkStats(), addrStats(), transStats(), traceStats() {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::finalReport(void) const {

    cerr << " bn: " << startScrape + scrapeCnt << *this << "\r";
    cerr.flush();

    if (blkStats.nSeen && logLevel > 0) {
        string_q logFile = blockCachePath("logs/acct-scrape.log");
        double startTime = qbNow();
        if (!fileExists(logFile))
            appendToAsciiFile(logFile, finalReport(startTime, true));
        if (logLevel >= 2 || blkStats.nSeen > 1000)
            appendToAsciiFile(logFile, finalReport(startTime, false));
    }
    return true;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COptions& item) {
    os << bBlack;
    os << " (bh.bq.bs.nb: "  << item.blkStats.nHit << "/" << item.blkStats.nQueried << "/" << item.blkStats.nSeen << "/" << item.scrapeCnt;
    os << ", ah.anh.as: "    << item.addrStats.nHit  << "/" << item.addrStats.nSkipped   << "/" << item.addrStats.nSeen;
    os << ", th.ts: "        << item.transStats.nHit  << "/" << item.transStats.nSeen;
    os << ", trh.trsk.trs: " << item.traceStats.nHit << "/" << item.traceStats.nSkipped << "/" << item.traceStats.nSeen << ")";
    os << cOff;
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
        os << "scrapeCnt\t";
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
        os << startScrape << "\t";
        os << scrapeCnt << "\t";
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
