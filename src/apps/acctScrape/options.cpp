/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("-maxBlocks:<val>",  "scan at most --maxBlocks blocks ('all' implies scan to end of chain)"),
    COption("-logLevel:<val>",   "specify the log level (default 1)"),
    COption("@useBlooms",        "search for transactions using bloom filters instead of address index"),
    COption("@noBloom(s)",       "do not use adaptive enhanced blooms (much faster if you use them)"),
    COption("@noBloc(k)s",       "do not use binary block cache (much faster if you use them)"),
    COption("@for_addr:<val>",   "force a scrape on the given account"),
    COption("@new_version",      "use the new version of index query"),
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

        } else if (arg == "-u" || arg == "--useBlooms") {
            useBlooms = true;

        } else if (arg == "--new_version") {
            new_version = true;

        } else if (arg == "-k" || arg == "--noBlocks") {
            ignoreBlkCache = true;

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
    primary = monitors[0];
    string_q configFile = "./" + primary.address + ".toml";
    if (fileExists(configFile)) {
        CToml *toml = new CToml(configFile);
        if (toml) {
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
            delete toml;
        }
    }

    if (getCurlContext()->nodeRequired) {
        CBlock latest;
        getBlock(latest, "latest");
        lastTimestamp = latest.timestamp;
        if (!isParity() || !nodeHasTraces())
            return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    } else {
        lastTimestamp = date_2_Ts(Now());
    }

    if (isAll)
        maxBlocks = INT_MAX;

    string_q transCachePath = getMonitorPath("");
    if (!folderExists(transCachePath)) {
        cerr << "The cache folder '" << transCachePath << "' not found. Trying to create it." << endl;
        establishFolder(transCachePath);
        if (!folderExists(transCachePath))
            return usage("The cache folder '" + transCachePath + "' not created. Quiting...");
    }

    for (auto monitor : monitors) {
        string_q fn = getMonitorPath(monitor.address);
        if (fileExists(fn + ".lck"))
            return usage("The cache file '" + fn + "' is locked. Quitting...");

        fn = getMonitorLast(monitor.address);
        if (fileExists(fn + ".lck"))
            return usage("The last block file '" + fn + "' is locked. Quitting...");
    }

    if (!useBlooms) {
        if (new_version) {
            if (!folderExists(indexFolder_binary_v2))
                return usage("Address index path '" + indexFolder_binary_v2 + "' not found. Quitting...");
        } else {
            if (!folderExists(indexFolder_sorted_v2))
                return usage("Address index path '" + indexFolder_sorted_v2 + "' not found. Quitting...");
        }
    } else if (useBlooms && !folderExists(bloomFolder_v2))
        return usage("Bloom filter path '" + bloomFolder_v2 + "' not found. Quitting...");

    if (ignoreBlkCache) {
        cerr << "Switching to local node, ignoring binary block cache" << endl;
        setDataSource("local");
    }

    blknum_t lastInCache = getLastBlock_cache_final();
    blknum_t lastVisited = str_2_Uint(asciiFileToString(getMonitorLast(primary.address)));
    startScrape = min(lastVisited, lastInCache);
    scrapeCnt   = min(lastInCache - startScrape, maxBlocks);

    if (lastVisited > lastInCache)
        cerr << "Nothing to scrape: startBlock (" << uint_2_Str(startScrape) << ") nBlocks (" << uint_2_Str(scrapeCnt) << ")." << endl;
    else {
        cerr << " (start: " << cTeal << startScrape << cOff;
        cerr << " end: " + cTeal << (startScrape + scrapeCnt) << cOff;
        cerr << (!useBlooms ? "" : (" n: " + cTeal + uint_2_Str(scrapeCnt))) << cOff << ")" << endl;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    // We want to be able to run this more than once
    // optionOn(OPT_RUNONCE);

    minArgs        = 0;
    lastTimestamp  = 0;
    ignoreBlooms   = false;
    useBlooms      = false;
    ignoreBlkCache = false;
    startScrape    = 0;
    scrapeCnt      = 0;
    blockCounted   = false;
    logLevel       = 1;
    new_version    = false;
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
        string_q logFile = getCachePath("logs/acct-scrape.log");
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
        os << primary.name.substr(0,15) << "\t";
        os << primary.address << "\t";
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
