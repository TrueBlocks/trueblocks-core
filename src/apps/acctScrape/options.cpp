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
    COption("@for_addr:<val>",   "force a scrape on the given account"),
    COption("",                  "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool makeMonitorFolder(const address_t& monitorAddr);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isAll = false;
    address_t forceAddr;

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

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--for_addr:")) {
            arg = substitute(substitute(arg, "-f:", ""), "--for_addr:", "");
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");
            forceAddr = arg;

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

    if (forceAddr.empty()) {
        if (!toml || !fileExists(toml->getFilename())) {
            string_q curDir = (isTestMode() ? "$DIR/" : getCWD());
            return usage("Cannot read toml file " + toml->getFilename() + " folder: " + curDir + ". Quitting...\n");
        }

        manageFields(toml->getConfigStr("fields", "hide", ""), false);
        manageFields(toml->getConfigStr("fields", "show", ""), true );
    }

    if (!isParity() || !nodeHasTraces())
        return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    string_q transCachePath = getTransCachePath("");
    if (!folderExists(transCachePath)) {
        cerr << "The cache folder '" << transCachePath << "' not found. Trying to create it." << endl;
        establishFolder(transCachePath);
        if (!folderExists(transCachePath))
            return usage("The cache folder '" + transCachePath + "' not created. Quiting...");
    }

    if (!forceAddr.empty()) {
        minWatchBlock = 0;
        maxWatchBlock = UINT32_MAX;
        CAccountWatch watch;
        watch.address = forceAddr;
        watch.name = forceAddr;
        watch.color = cBlue; //convertColor(watch.color);
        monitors.push_back(watch);

    } else {
        if (!loadMonitors())
            return false;
    }

    cacheFilename = getTransCachePath(monitors[0].address);
    string_q lb   = getTransCacheLast(monitors[0].address);
    if (fileExists(cacheFilename+".lck") || fileExists(lb + ".lck"))
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
        cerr << bBlack << Now().Format(FMT_JSON) << cOff << ": Monitoring " << cYellow << cacheFilename << cOff << "             \n";

    lastInCache = getLatestBlockFromCache();
//cout << "1: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;
    lastVisited = str_2_Uint(asciiFileToString(getTransCacheLast(monitors[0].address)));
//cout << "2: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;
    if (minWatchBlock > 0)
        lastVisited = max(minWatchBlock - 1, lastVisited);
//cout << "3: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;

    lastBlock  = min(lastInCache, maxWatchBlock);
//cout << "4: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;
    firstBlock = min(lastVisited, lastBlock);
//cout << "5: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;
    nBlocks    = min(lastBlock - firstBlock, maxBlocks);
//cout << "6: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;
    if (useIndex)
        nBlocks = 10000000;  // TODO(tjayrush): Not right
//cout << "7: " << lastInCache << " " << lastVisited << " " << lastBlock << " " << firstBlock << " " << nBlocks << endl;

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
    lastInCache    = NOPOS;
    lastVisited    = NOPOS;
    ex_data        = "";
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
bool COptions::shouldScrape(void) const {
    return (lastVisited <= lastInCache);
}

//--------------------------------------------------------------------------------
bool COptions::finalReport(void) const {

    if (isList) {
        CArchive cache(READING_ARCHIVE);
        if (!cache.Lock(cacheFilename, modeReadOnly, LOCK_NOWAIT))
            return usage("Could not open file: " + cacheFilename + ". Quitting.");
        while (!cache.Eof() && !shouldQuit()) {
            CAcctCacheItem item;
            cache >> item.blockNum >> item.transIndex;
            if (item.blockNum > 0)
                cout << item.blockNum << "\t" << item.transIndex << endl;
        }
        cache.Release();

    } else {

        cerr << name << " bn: " << firstBlock + nBlocks << *this << "\r";
        cerr.flush();

        if (blkStats.nSeen && logLevel > 0) {
            string_q logFile = blockCachePath("logs/acct-scrape.log");
            double startTime = qbNow();
            if (!fileExists(logFile))
                appendToAsciiFile(logFile, finalReport(startTime, true));
            if (logLevel >= 2 || blkStats.nSeen > 1000)
                appendToAsciiFile(logFile, finalReport(startTime, false));
        }
    }
    return true;
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

//-----------------------------------------------------------------------
bool COptions::loadMonitors(void) {

    minWatchBlock = UINT32_MAX;
    maxWatchBlock = 0;

    string_q watchStr = toml->getConfigJson("watches", "list", "");
    if (watchStr.empty())
        return usage("Empty list of watches. Quitting.");

    CAccountWatch watch;
    while (watch.parseJson3(watchStr)) {
        // cleanup and report on errors
        watch.color   = convertColor(watch.color);
        watch.address = str_2_Addr(toLower(watch.address));
        watch.nodeBal = getBalanceAt(watch.address, watch.firstBlock-1);
        watch.api_spec.method = toml->getConfigStr("api_spec", "method", "");
        watch.api_spec.uri = toml->getConfigStr("api_spec", "uri", "");
        watch.api_spec.headers = toml->getConfigStr("api_spec", "headers", "");
        if (!watch.api_spec.uri.empty()) {
            watch.abi_spec.loadAbiByAddress(watch.address);
            watch.abi_spec.loadAbiKnown("all");
        }

        string_q msg;
        if (!isAddress(watch.address)) {
            msg = "invalid address " + watch.address;
        }
        if (watch.name.empty()) {
            if (!msg.empty())
                msg += ", ";
            msg += "no name " + watch.name;
        }

        // add to array or return error
        if (msg.empty()) {
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
            maxWatchBlock = max(maxWatchBlock, watch.lastBlock);
            monitors.push_back(watch);

        } else {
            return usage(msg);

        }
        watch = CAccountWatch();  // reset
    }
    name = toml->getConfigStr("settings", "name", monitors[0].name);

    return true;
}

//-------------------------------------------------------------------------
void COptions::writeLastBlock(blknum_t bn) {
    if (!isTestMode())
        stringToAsciiFile(getTransCacheLast(monitors[0].address), uint_2_Str(bn) + "\n");
    else
        cerr << "Would have written lastBlock.txt file: " << bn << endl;
}

//-----------------------------------------------------------------------
const string_q fmt = "[{BLOCKNUMBER} ][{w:5:TRANSACTIONINDEX} ][{w:10:DATE} ][{ETHER}]";
//-------------------------------------------------------------------------
bool COptions::foundOne(const CAccountWatch *acct, const CBlock& block, const CTransaction *trans) {
    if (!blockCounted) {
        blockCounted = true;
        blkStats.nHit++; // only count the block once per block
    }
    transStats.nHit++;

    CBlock *pBlock = (CBlock*)&block;
    pBlock->finalized = isBlockFinal(block.timestamp, lastTimestamp, (60 * 4));

    lockSection(true);
    if (!isTestMode()) {
        // We found something...write it to the cache...
        txCache << block.blockNumber << trans->transactionIndex;
        txCache.flush();
        writeLastBlock(block.blockNumber);
        if (writeBlocks) {
            // pBlock->finalized is implicit here don't remove it above
            string_q fn = getBinaryFilename(block.blockNumber);
            if (!fileExists(fn))
                writeBlockToBinary(block, fn);
        }
    }

    // Send the data to an api if we have one
    if (!acct->api_spec.uri.empty()) {
        ((CTransaction*)trans)->extra_data = ex_data;
        ((CTransaction*)trans)->finalized = pBlock->finalized;

        if (trans->traces.size() == 0)
            getTraces(((CTransaction*)trans)->traces, trans->hash);
        acct->abi_spec.articulateTransaction((CTransaction*)trans);

        SHOW_FIELD(CFunction,    "message");
        SHOW_FIELD(CTransaction, "extra_data");
        SHOW_FIELD(CTransaction, "finalized");
        if (isTestMode() || fileExists("./debug")) {
            SHOW_FIELD(CLogEntry,  "data");
            SHOW_FIELD(CLogEntry,  "topics");
            SHOW_FIELD(CParameter, "type");
            cout << trans->Format() << endl;
        } else {
            ((CAccountWatch*)acct)->api_spec.sendData(trans->Format());
        }
        HIDE_FIELD(CTransaction, "extra_data");
        HIDE_FIELD(CTransaction, "finalized");
        HIDE_FIELD(CFunction,    "message");
    }
    lockSection(false);

    cerr << string_q(4,' ') << acct->displayName(false,true,true,8) << ": ";
    cerr << trans->Format(fmt) << " (" << blkStats.nSeen << " of " << nBlocks << ")" << string_q(15,' ') << endl;

    return true;
}
