/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("addr_list", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) to export"),
    COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_FLAG, "export format (one of [json*|txt|csv])"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("logs", "l", "", OPT_SWITCH, "export logs instead of transaction list"),
    COption("traces", "t", "", OPT_SWITCH, "export traces instead of transaction list"),
    COption("balances", "c", "", OPT_SWITCH, "export balance history instead of transaction list"),
    COption("appearances", "p", "", OPT_SWITCH, "export a list of appearances"),
    COption("count_only", "o", "", OPT_SWITCH, "display only the count of the number of data items requested"),
    COption("blocks", "b", "enum[on|off*]", OPT_HIDDEN | OPT_FLAG, "write blocks to the binary cache ('off' by default)"),
    COption("writeTxs", "s", "enum[on*|off]", OPT_HIDDEN | OPT_FLAG, "write transactions to the binary cache ('on' by default)"),
    COption("writeTraces", "r", "enum[on*|off]", OPT_HIDDEN | OPT_FLAG, "write traces to the binary cache ('on' by default)"),
    COption("ddos", "d", "enum[on*|off]", OPT_HIDDEN | OPT_FLAG, "skip over dDos transactions in export ('on' by default)"),
    COption("maxTraces", "m", "<uint>", OPT_HIDDEN | OPT_FLAG, "if --ddos:on, the number of traces defining a dDos (default = 250)"),
    COption("no_header", "n", "", OPT_HIDDEN | OPT_SWITCH, "do not show the header row"),
    COption("allABIs", "a", "", OPT_HIDDEN | OPT_SWITCH, "load all previously cached abi files"),
    COption("grabABIs", "g", "", OPT_HIDDEN | OPT_SWITCH, "using each trace's 'to' address, grab the abi for that address (improves articulation)"),
    COption("freshen", "f", "", OPT_HIDDEN | OPT_SWITCH, "freshen but do not print the exported data"),
    COption("deltas", "", "", OPT_HIDDEN | OPT_SWITCH, "for --balances option only, export only changes in balances"),
    COption("start", "", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to export (inclusive)"),
    COption("end", "", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to export (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more Ethereum addresses."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    ENTER("parseArguments");

    if (!standardOptions(command))
        EXIT_NOMSG(false);

    bool no_header = false, allABIs = false;
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, "-s") || startsWith(arg, "--writeTxs")) {
            arg = substitute(substitute(arg, "-s:", ""), "--writeTxs:", "");
            if (arg != "on" && arg != "off")
                EXIT_USAGE("Please provide either 'on' or 'off' for the --writeTxs options. Quitting...");
            writeTxs = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-r") || startsWith(arg, "--writeTraces")) {
            arg = substitute(substitute(arg, "-r:", ""), "--writeTraces:", "");
            if (arg != "on" && arg != "off")
                EXIT_USAGE("Please provide either 'on' or 'off' for the --writeTraces options. Quitting...");
            writeTraces = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-d") || startsWith(arg, "--ddos")) {
            arg = substitute(substitute(arg, "-d:", ""), "--ddos:", "");
            if (arg != "on" && arg != "off")
                EXIT_USAGE("Please provide either 'on' or 'off' for the --ddos options. Quitting...");
            skipDdos = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-m") || startsWith(arg, "--maxTraces")) {
            arg = substitute(substitute(arg, "-m:", ""), "--maxTraces:", "");
            if (!isNumeral(arg))
                EXIT_USAGE("Please provide a number (you provided " + arg + ") for --maxTraces. Quitting...");
            maxTraces = str_2_Uint(arg);

        } else if (startsWith(arg, "-s") || startsWith(arg, "--start")) {
            arg = substitute(substitute(arg, "-s:", ""), "--start:", "");
            if (!isNumeral(arg))
                EXIT_USAGE("Not a number for --startBlock: " + arg + ". Quitting.");
            scanRange.first = str_2_Uint(arg);

        } else if (startsWith(arg, "-e") || startsWith(arg, "--end")) {
            arg = substitute(substitute(arg, "-e:", ""), "--end:", "");
            if (!isNumeral(arg))
                EXIT_USAGE("Not a number for --endBlock: " + arg + ". Quitting.");
            scanRange.second = str_2_Uint(arg);

        } else if (arg == "-n" || arg == "--no_header") {
            no_header = true;

        } else if (arg == "-n" || arg == "--allABIs") {
            allABIs = true;

        } else if (arg == "-p" || arg == "--appearances") {
            doAppearances = true;

        } else if (arg == "-l" || arg == "--logs") {
            doLogs = true;

        } else if (arg == "-t" || arg == "--traces") {
            doTraces = true;

        } else if (arg == "-c" || arg == "--balances") {
            doBalances = true;

        } else if (arg == "-g" || arg == "--grabABIs") {
            doTraces = true;
            doABIs = true;

        } else if (arg == "-f" || arg == "--freshen") {
            freshen_only = true;

        } else if (arg == "-o" || arg == "--count_only") {
            count_only = true;

        } else if (arg == "--deltas") {
            deltas_only = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (startsWith(arg, "0x")) {

            arg = toLower(arg);

            if (!isAddress(arg))
                EXIT_USAGE(arg + " does not appear to be a valid address. Quitting...");

            string_q fn = getMonitorPath(arg);
            if (!fileExists(fn)) {
                fn = (isTestMode() ? substitute(fn, getMonitorPath(""), "./") : fn);
                EXIT_USAGE("File not found '" + fn + ". Quitting...");
            }

            if (fileExists(fn + ".lck"))
                EXIT_USAGE("The cache lock file is present. The program is either already "
                             "running or it did not end cleanly the\n\tlast time it ran. "
                             "Quit the already running program or, if it is not running, "
                             "remove the lock\n\tfile: " + fn + ".lck'. Quitting...");

            //if (fileSize(fn) == 0)
            //    EXIT_USAGE("Nothing to export. Quitting...");

            CAccountWatch watch;
            // below - don't change, sets bloom value also
            watch.setValueByName("address", toLower(arg));
            // above - don't change, sets bloom value also
            watch.setValueByName("name", toLower(arg));
            watch.extra_data = getVersionStr() + "/" + watch.address;
            watch.color = cTeal;
            watch.finishParse();
            monitors.push_back(watch);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                EXIT_USAGE("Invalid option: " + arg);
            }
        }
    }
    LOG4("Finished parsing command line");

    SHOW_FIELD(CTransaction, "traces");

    if ((doAppearances + doLogs + doTraces + doBalances) > 1)
        EXIT_USAGE("Please export only one of list, logs, traces, or balances. Quitting...");

    if (monitors.size() == 0)
        EXIT_USAGE("You must provide at least one Ethereum address. Quitting...");

    if (deltas_only && !doBalances)
        EXIT_USAGE("--deltas option is only available with --balances. Quitting...");

    // show certain fields and hide others
    //SEP4("default field hiding: " + defHide);
    manageFields(defHide, false);
    //SEP4("default field showing: " + defShow);
    manageFields(defShow, true);

    CToml toml(getMonitorPath(monitors[0].address + ".toml"));
    //SEP4("field hiding: " + toml.getConfigStr("fields", "hide", ""));
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    //SEP4("field showing: " + toml.getConfigStr("fields", "show", ""));
    manageFields(toml.getConfigStr("fields", "show", ""), true );

    // Load as many ABI files as we have
    if (!doAppearances && !doBalances) {
        LOG4("Loading ABIs");
        abis.loadAbiKnown("all");
        if (allABIs)
            abis.loadCachedAbis("all");
        LOG4("Finished loading ABIs");
    }

    // Try to articulate the watched addresses
    for (size_t i = 0 ; i < monitors.size() ; i++) {
        CAccountWatch *watch = &monitors[i];
        abis.loadAbiByAddress(watch->address);
        //abis.loadAbiKnown("all");
        string_q path = getMonitorPath(watch->address + ".toml");
        if (fileExists(path)) { // if there's a config file, let's use it user can tell us the names of other addresses
            CToml thisToml(path);
            string_q str = substitute(substitute(thisToml.getConfigJson("named", "list", ""),"[",""),"=",":");
            CAccountWatch item;
            while (item.parseJson3(str)) {
                item.address   = str_2_Addr(toLower(item.address));
                item.color     = convertColor(item.color);
                item.extra_data = getVersionStr() + "/" + item.address;
                item.finishParse();
                named.push_back(item);
                abis.loadAbiByAddress(item.address);
                item = CAccountWatch();
            }
        }
    }

    writeTxs    = getGlobalConfig("acctExport")->getConfigBool("settings", "writeTxs", writeTxs);;
    writeTraces = getGlobalConfig("acctExport")->getConfigBool("settings", "writeTraces", writeTraces);;
    skipDdos    = getGlobalConfig("acctExport")->getConfigBool("settings", "skipDdos", skipDdos);;
    maxTraces   = getGlobalConfig("acctExport")->getConfigBool("settings", "maxTraces", maxTraces);;

    if (exportFmt != JSON1 && exportFmt != API1) {
        string_q deflt, format;

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "format", STR_DISPLAY_TRANSACTION);
        format = toml.getConfigStr("formats", "trans_fmt", deflt);
        expContext().fmtMap["transaction_fmt"] = cleanFmt(format, exportFmt);

        if (format.empty())
            EXIT_USAGE("For non-json export a 'trans_fmt' string is required. Check your config file. Quitting...");
        if (!contains(toLower(format), "trace"))
            HIDE_FIELD(CTransaction, "traces");

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "trace", STR_DISPLAY_TRACE);
        format = toml.getConfigStr("formats", "trace_fmt", deflt);
        expContext().fmtMap["trace_fmt"] = cleanFmt(format, exportFmt);

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "log", STR_DISPLAY_LOGENTRY);
        format = toml.getConfigStr("formats", "logentry_fmt", deflt);
        expContext().fmtMap["logentry_fmt"] = cleanFmt(format, exportFmt);

        // This doesn't really work because CAppearance_base is not a subclass of CBaseNode. We phony it here for future reference.
        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "appearances", STR_DISPLAY_DISPLAYAPP);
        format = toml.getConfigStr("formats", "displayapp_fmt", deflt);
        expContext().fmtMap["displayapp_fmt"] = cleanFmt(format, exportFmt);

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "balance", STR_DISPLAY_BALANCERECORD);
        format = toml.getConfigStr("formats", "balancerecord_fmt", deflt);
        if (expContext().asEther) {
            format = substitute(format, "{BALANCE}", "{ETHER}");
            format = substitute(format, "{PRIORBALANCE}", "{ETHERPRIOR}");
            format = substitute(format, "{DIFF}", "{ETHERDIFF}");
        }
        if (expContext().asDollars) {
            format = substitute(format, "{BALANCE}", "{DOLLARS}");
            format = substitute(format, "{PRIORBALANCE}", "{DOLLARSPRIOR}");
            format = substitute(format, "{DIFF}", "{DOLLARSDIFF}");
        }
        expContext().fmtMap["balancerecord_fmt"] = cleanFmt(format, exportFmt);

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "balance", STR_DISPLAY_BALANCEDELTA);
        format = toml.getConfigStr("formats", "balancedelta_fmt", deflt);
        if (expContext().asEther) {
            format = substitute(format, "{BALANCE}", "{ETHER}");
            format = substitute(format, "{DIFF}", "{ETHERDIFF}");
        }
        if (expContext().asDollars) {
            format = substitute(format, "{BALANCE}", "{DOLLARS}");
            format = substitute(format, "{DIFF}", "{DOLLARSDIFF}");
        }
        expContext().fmtMap["balancedelta_fmt"] = cleanFmt(format, exportFmt);
    }

    expContext().fmtMap["header"] = "";
    if (!no_header) {
        if (doTraces) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["trace_fmt"], exportFmt);
        } else if (doLogs) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["logentry_fmt"], exportFmt);
        } else if (doAppearances) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["displayapp_fmt"], exportFmt);
        } else if (doBalances) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["balancerecord_fmt"], exportFmt);
            if (deltas_only)
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["balancedelta_fmt"], exportFmt);
            SHOW_FIELD(CBalanceRecord, "address");
            SHOW_FIELD(CBalanceDelta, "address");
        } else {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["transaction_fmt"], exportFmt);
        }
    }

    if (freshen_only)
        exportFmt = NONE1;

//    if (count_only && !doAppearances)
//        EXIT_USAGE("the --count_only option is only available with the --appearances option. Quitting...");

    if (count_only) {
        string_q header;
        if (exportFmt == TXT1)
            header = "address\tcount";
        else if (exportFmt == CSV1)
            header = ("\"" + substitute(header, "\t", "\",\"") + "\"");
        expContext().fmtMap["header"] = header;
    }

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);

    monitors.clear();

    writeTxs = true;
    writeTraces = true;
    skipDdos = true;
    maxTraces = 250;
    articulate = false;
    doAppearances = false;
    nExported = 0;
    doLogs = false;
    doTraces = false;
    doBalances = false;
    doABIs = false;
    freshen_only = false;
    count_only = false;
    deltas_only = false;
    scanRange.second = getLastBlock_cache_ripe();

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    ts_array = NULL;
    ts_cnt = 0;
    Init();
    CDisplayApp::registerClass();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "addr_list", "<address> [address...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty two characters long.\n";
        return ret;
    }
    return str;
}

//-----------------------------------------------------------------------
bool COptions::loadOneAddress(CAppearanceArray_base& apps, const address_t& addr) {

    ENTER("loadOneAddress");

    if (hackAppAddr.empty())
        hackAppAddr = addr;

    string_q fn = getMonitorPath(addr);

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
            EXIT_FAIL("Could not open cache file.");
        }

        // Add to the apps which may be non-empty
        apps.reserve(apps.size() + nRecords);
        for (size_t i = 0 ; i < nRecords ; i++) {
            if (buffer[i].blk == 0)
                prefundAddrMap[buffer[i].txid] = toLower(addr);
            if (buffer[i].txid == 99999 || buffer[i].txid == 99998 || buffer[i].txid == 99997)
                blkRewardMap[buffer[i].blk] = addr;
            apps.push_back(buffer[i]);
        }

        delete [] buffer;

    } else {
        EXIT_FAIL("Could not allocate memory for address " + addr);
    }
    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
bool COptions::loadAllAppearances(void) {

    ENTER("loadAllAppearances");

    CAppearanceArray_base tmp;
    for (auto monitor : monitors) {
        if (!loadOneAddress(tmp, monitor.address))
            EXIT_FAIL("Could not load data.");
        if (freshen_only) {
            // If we're freshening...
            blknum_t lastExport = str_2_Uint(asciiFileToString(getMonitorExpt(monitor.address)));
            if (scanRange.first == 0) // we can start where the last export happened on any address...
                scanRange.first = lastExport;
            if (lastExport < scanRange.first) // ...but the eariest of the last exports is where we start
                scanRange.first = lastExport;
        }
    }

    if (tmp.size() == 0) {
        if (!freshen_only)
            LOG_INFO("Nothing to export" + (monitors.size() ? (" from " + monitors[0].address) : "") + ".");
        return false;
    }

    // Should be sorted already, so it can't hurt
    sort(tmp.begin(), tmp.end());

    bool hasFuture = false;
    blknum_t lastAtClient = getLastBlock_client();
    items.push_back(tmp[0]);
    for (auto item : tmp) {
        CAppearance_base *prev = &items[items.size() - 1];
        // TODO(tjayrush): I think this removes dups. Is it really necessary?
        if (item.blk != prev->blk || item.txid != prev->txid) {
            if (item.blk > lastAtClient)
                hasFuture = true;
            else
                items.push_back(item);
        }
    }
    //LOG1("Items array: " + uint_2_Str(items.size()) + " - " + uint_2_Str(items.size() * sizeof(CAppearance_base)));
    if (hasFuture)
        LOG_WARN("Cache file contains blocks ahead of the chain. Some items will not be exported.");

    if (!freshenTimestampFile(items[items.size()-1].blk)) {
        EXIT_FAIL("Could not freshen timestamp file.");
    }

    if (!loadTimestampArray(&ts_array, ts_cnt))
        EXIT_FAIL("Could not open timestamp file.");

    EXIT_NOMSG(true);
}
