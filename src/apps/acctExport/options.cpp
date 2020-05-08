/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) to export"),
    COption("appearances", "p", "", OPT_SWITCH, "export a list of appearances"),
    COption("receipts", "r", "", OPT_SWITCH, "export receipts instead of transaction list"),
    COption("logs", "l", "", OPT_SWITCH, "export logs instead of transaction list"),
    COption("traces", "t", "", OPT_SWITCH, "export traces instead of transaction list"),
    COption("balances", "b", "", OPT_SWITCH, "export balance history instead of transaction list"),
    COption("hashes_only", "e", "", OPT_SWITCH, "export the IPFS hashes of the index chunks the address appears in"),
    COption("count_only", "c", "", OPT_SWITCH, "display only the count of the number of data items requested"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("write_txs", "i", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("write_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("skip_ddos", "s", "", OPT_HIDDEN | OPT_TOGGLE, "toggle skipping over 2016 dDos transactions ('on' by default)"),  // NOLINT
    COption("max_traces", "m", "<uint64>", OPT_HIDDEN | OPT_FLAG, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)"),  // NOLINT
    COption("all_abis", "A", "", OPT_HIDDEN | OPT_SWITCH, "load all previously cached abi files"),
    COption("grab_abis", "g", "", OPT_HIDDEN | OPT_SWITCH, "using each trace's 'to' address, grab the abi for that address (improves articulation)"),  // NOLINT
    COption("freshen", "f", "", OPT_HIDDEN | OPT_SWITCH, "freshen but do not print the exported data"),
    COption("deltas", "D", "", OPT_HIDDEN | OPT_SWITCH, "for --balances option only, export only changes in balances"),
    COption("occurrence", "o", "<blknum>", OPT_FLAG, "for each loaded list of appearances, export only this occurrence"),  // NOLINT
    COption("emitter", "M", "", OPT_SWITCH, "available for --logs option only, export will only export if the address emitted the event"),  // NOLINT
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more Ethereum addresses."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern int xor_options(bool, bool);
extern string_q report_cache(int);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    CAddressArray addrs;
    bool write_txs = false;
    bool write_traces = false;
    bool all_abis = false;
    blknum_t start = NOPOS;
    blknum_t end = NOPOS;
    // END_CODE_LOCAL_INIT

    latestBlock = getLatestBlock_client();
    blknum_t latest = latestBlock;
    string_q origCmd = command;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-p" || arg == "--appearances") {
            appearances = true;

        } else if (arg == "-r" || arg == "--receipts") {
            receipts = true;

        } else if (arg == "-l" || arg == "--logs") {
            logs = true;

        } else if (arg == "-t" || arg == "--traces") {
            traces = true;

        } else if (arg == "-b" || arg == "--balances") {
            balances = true;

        } else if (arg == "-e" || arg == "--hashes_only") {
            hashes_only = true;

        } else if (arg == "-c" || arg == "--count_only") {
            count_only = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-i" || arg == "--write_txs") {
            write_txs = true;

        } else if (arg == "-R" || arg == "--write_traces") {
            write_traces = true;

        } else if (arg == "-s" || arg == "--skip_ddos") {
            skip_ddos = !skip_ddos;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--max_traces:")) {
            if (!confirmUint("max_traces", max_traces, arg))
                return false;

        } else if (arg == "-A" || arg == "--all_abis") {
            all_abis = true;

        } else if (arg == "-g" || arg == "--grab_abis") {
            grab_abis = true;

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg == "-D" || arg == "--deltas") {
            deltas = true;

        } else if (startsWith(arg, "-o:") || startsWith(arg, "--occurrence:")) {
            if (!confirmBlockNum("occurrence", occurrence, arg, latest))
                return false;

        } else if (arg == "-M" || arg == "--emitter") {
            emitter = true;

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
            if (!parseAddressList2(this, addrs, arg))
                return false;

            // END_CODE_AUTO
        }
    }

#ifdef MOCK_DATA
    if (mockData)
        return false;
#endif

    // Once we know how many exported items there will be (see loadAllAppearances), we will decide
    // what to cache. If the user has either told us via the command line or the config file, we will
    // use those settings. By default, user and config cache settins are off (0), so if they are
    // not zero, we know the user has made their desires known.
    const CToml* conf = getGlobalConfig("acctExport");

    // Caching options (i.e. write_opt) are as per config file...
    write_opt = xor_options(conf->getConfigBool("settings", "write_txs", false),
                            conf->getConfigBool("settings", "write_traces", false));
    if (write_opt)
        write_opt |= CACHE_BYCONFIG;

    // ...unless user has explicitly told us what to do on the command line...
    if (contains(origCmd, "write")) {
        write_opt = xor_options(write_txs, write_traces);
        write_opt |= (CACHE_BYUSER);
    }

    // ... but may not be done. In loadAllAppearances, if write_opt is not set by user, we set it to cache transactions
    // or traces if there are less than 1,000 exported items

    for (auto addr : addrs) {
        string_q fn = getMonitorPath(addr);
        if (!fileExists(fn)) {
            fn = (isTestMode() ? substitute(fn, getMonitorPath(""), "./") : fn);
            EXIT_USAGE("File not found '" + fn + ". Quitting...");
        }

        if (fileExists(fn + ".lck"))
            EXIT_USAGE(
                "The cache lock file is present. The program is either already "
                "running or it did not end cleanly the\n\tlast time it ran. "
                "Quit the already running program or, if it is not running, "
                "remove the lock\n\tfile: " +
                fn + ".lck'. Quitting...");

        CAccountWatch watch;
        // below - don't change, sets bloom value also
        watch.setValueByName("address", toLower(addr));
        // above - don't change, sets bloom value also
        watch.setValueByName("name", toLower(addr));
        watch.finishParse();
        monitors.push_back(watch);
    }

    if (start != NOPOS)
        scanRange.first = start;
    if (end != NOPOS)
        scanRange.second = end;
    if (grab_abis)
        traces = true;

    SHOW_FIELD(CTransaction, "traces");

    if ((appearances + receipts + logs + traces + balances + hashes_only) > 1)
        EXIT_USAGE("Please export only one of list, receipts, logs, traces, balances or hashes_only. Quitting...");

    if (emitter && !logs)
        EXIT_USAGE("The emitter option is only available when exporting logs. Quitting...");

    if (emitter && monitors.size() > 1)
        EXIT_USAGE("The emitter option is only available when exporting logs from a single address. Quitting...");

    if (monitors.size() == 0)
        EXIT_USAGE("You must provide at least one Ethereum address. Quitting...");

    if (deltas && !balances)
        EXIT_USAGE("--deltas option is only available with --balances. Quitting...");

    // show certain fields and hide others
    // SEP4("default field hiding: " + defHide);
    manageFields(defHide, false);
    // SEP4("default field showing: " + defShow);
    string_q show = defShow + (isApiMode() ? "|CTransaction:encoding,function,input,compressedTx,etherGasCost,dollars" : "");
    manageFields(show, true);

    CToml toml(getMonitorCnfg(monitors[0].address));
    // SEP4("field hiding: " + toml.getConfigStr("fields", "hide", ""));
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    // SEP4("field showing: " + toml.getConfigStr("fields", "show", ""));
    manageFields(toml.getConfigStr("fields", "show", ""), true);

    // Load as many ABI files as we have
    if (!appearances && !balances && !hashes_only) {
        abis.loadAbiKnown();
        if (all_abis)
            abis.loadAbisMonitors();
    }

    // Try to articulate the watched addresses
    for (size_t i = 0; i < monitors.size(); i++) {
        CAccountWatch* watch = &monitors[i];
        abis.loadAbiByAddress(watch->address);
        // abis.loadAbiKnown();
        string_q path = getMonitorCnfg(watch->address);
        if (fileExists(path)) {  // if there's a config file, let's use it user can tell us the names of other addresses
            CToml thisToml(path);
            string_q str = substitute(substitute(thisToml.getConfigJson("named", "list", ""), "[", ""), "=", ":");
            CAccountWatch item;
            while (item.parseJson3(str)) {
                item.address = str_2_Addr(toLower(item.address));
                item.finishParse();
                abis.loadAbiByAddress(item.address);
                item = CAccountWatch();
            }
        }
    }

    if (expContext().exportFmt != JSON1 && expContext().exportFmt != API1) {
        string_q deflt, format;

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "format", STR_DISPLAY_TRANSACTION);
        format = toml.getConfigStr("formats", "trans_fmt", deflt);
        expContext().fmtMap["transaction_fmt"] = cleanFmt(format);

        if (format.empty())
            EXIT_USAGE("For non-json export a 'trans_fmt' string is required. Check your config file. Quitting...");
        if (!contains(toLower(format), "trace"))
            HIDE_FIELD(CTransaction, "traces");

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "receipt", STR_DISPLAY_RECEIPT);
        format = toml.getConfigStr("formats", "receipt_fmt", deflt);
        expContext().fmtMap["receipt_fmt"] = cleanFmt(format);

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "log", STR_DISPLAY_LOGENTRY);
        format = toml.getConfigStr("formats", "logentry_fmt", deflt);
        expContext().fmtMap["logentry_fmt"] = cleanFmt(format);

        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "trace", STR_DISPLAY_TRACE);
        format = toml.getConfigStr("formats", "trace_fmt", deflt);
        expContext().fmtMap["trace_fmt"] = cleanFmt(format);

        // This doesn't really work because CAppearance_base is not a subclass of CBaseNode. We phony it here for future
        // reference.
        deflt = getGlobalConfig("acctExport")->getConfigStr("display", "appearances", STR_DISPLAY_DISPLAYAPP);
        format = toml.getConfigStr("formats", "displayapp_fmt", deflt);
        expContext().fmtMap["displayapp_fmt"] = cleanFmt(format);

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
        expContext().fmtMap["balancerecord_fmt"] = cleanFmt(format);

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
        expContext().fmtMap["balancedelta_fmt"] = cleanFmt(format);
    }

    expContext().fmtMap["header"] = "";
    if (!isNoHeader) {
        if (traces) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["trace_fmt"]);
        } else if (receipts) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["receipt_fmt"]);
        } else if (logs) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["logentry_fmt"]);
        } else if (appearances) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["displayapp_fmt"]);
        } else if (balances) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["balancerecord_fmt"]);
            if (deltas)
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["balancedelta_fmt"]);
            SHOW_FIELD(CBalanceRecord, "address");
            SHOW_FIELD(CBalanceDelta, "address");
        } else {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["transaction_fmt"]);
        }
    }

    if (occurrence != NOPOS) {
        if (freshen)
            EXIT_USAGE("The 'occurrence' option cannot be used with the 'freshe' option. Quitting...");
        if (monitors.size() > 1)
            EXIT_USAGE("The 'occurrence' option is only available for a single address. Quitting...");
        // weird hack because 'latest' in the sense of an occurrence is different than latest in the sense of a block
        if ((contains(command, "occurrence:latest") || contains(command, "o:latest")) && occurrence == latestBlock) {
            occurrence = (NOPOS - 1);
        }
    }

    if (freshen)
        expContext().exportFmt = NONE1;

    //    if (count_only && !doAppearances)
    //        EXIT_USAGE("the --count_only option is only available with the --appearances option. Quitting...");

    if (count_only) {
        string_q header;
        if (expContext().exportFmt == TXT1)
            header = "address\tcount";
        else if (expContext().exportFmt == CSV1)
            header = ("\"" + substitute(header, "\t", "\",\"") + "\"");
        expContext().fmtMap["header"] = header;
    }

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT | OPT_MOCKDATA);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    monitors.clear();

    // BEG_CODE_INIT
    appearances = false;
    receipts = false;
    logs = false;
    traces = false;
    balances = false;
    hashes_only = false;
    count_only = false;
    articulate = false;
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    grab_abis = false;
    freshen = false;
    deltas = false;
    occurrence = NOPOS;
    emitter = false;
    // END_CODE_INIT

    nExported = 0;
    scanRange.second = getLatestBlock_cache_ripe();
    items.clear();

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    ts_array = NULL;
    ts_cnt = 0;
    Init();
    CDisplayApp::registerClass();
    CCounts::registerClass();
    CIpfshash::registerClass();
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`addresses` must start with '0x' and be forty two characters long.");
    notes.push_back("By default, transactions and traces are cached if the number of exported | items is <= to 1,000 items. Otherwise, if you specify any `write_*` options, | your preference predominates.");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool COptions::loadOneAddress(CAppearanceArray_base& apps, const address_t& addr) {
    ENTER("loadOneAddress");

    if (hackAppAddr.empty())
        hackAppAddr = addr;

    string_q fn = getMonitorPath(addr);

    size_t nRecords = (fileSize(fn) / sizeof(CAppearance_base));
    ASSERT(nRecords);

    CAppearance_base* buffer = new CAppearance_base[nRecords];
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
        for (size_t i = 0; i < nRecords; i++) {
            if (buffer[i].blk == 0)
                prefundAddrMap[buffer[i].txid] = toLower(addr);
            if (buffer[i].txid == 99999 || buffer[i].txid == 99998 || buffer[i].txid == 99997)
                blkRewardMap[buffer[i].blk] = addr;
            if (occurrence == NOPOS) {  // no filter
                apps.push_back(buffer[i]);
            } else if (occurrence == (NOPOS - 1) && i == (nRecords - 1)) {  // special case for 'latest'
                apps.push_back(buffer[i]);
                occurrence = i;
            } else if (occurrence == i) {
                apps.push_back(buffer[i]);
            }
        }

        if (expContext().fmtMap["meta"].empty() && occurrence != NOPOS) {
            blknum_t next = (occurrence == nRecords - 1) ? occurrence : occurrence + 1;
            blknum_t prev = (occurrence == 0) ? 0 : occurrence - 1;
            string_q links;
            links += ",\n\"links\": ";
            links += "{";
            links += " \"next\": " + uint_2_Str(next) + ",";
            links += " \"prev\": " + uint_2_Str(prev) + ",";
            links += " \"current\": " + uint_2_Str(occurrence) + ",";
            links += " \"count\": " + uint_2_Str(nRecords);
            links += "}\n";
            expContext().fmtMap["meta"] = links;
        }

        delete[] buffer;

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
        if (freshen) {
            // If we're freshening...
            blknum_t lastExport = str_2_Uint(asciiFileToString(getMonitorExpt(monitor.address)));
            if (scanRange.first == 0)  // we can start where the last export happened on any address...
                scanRange.first = lastExport;
            if (lastExport < scanRange.first)  // ...but the eariest of the last exports is where we start
                scanRange.first = lastExport;
        }
    }

    if (tmp.size() == 0) {
        if (!freshen)
            LOG_INFO("Nothing to export" + (monitors.size() ? (" from " + monitors[0].address) : "") + ".");
        return false;
    }

    // Should be sorted already, so it can't hurt
    sort(tmp.begin(), tmp.end());

    items.push_back(tmp[0]);
    for (auto item : tmp) {
        CAppearance_base* prev = &items[items.size() - 1];
        // TODO(tjayrush): I think this removes dups. Is it really necessary?
        if (item.blk != prev->blk || item.txid != prev->txid) {
            if (item.blk > latestBlock) {
                static bool hasFuture = false;
                if (!hasFuture) {
                    LOG_WARN("Cache file contains blocks ahead of the chain. Some items will not be exported.");
                    hasFuture = true;
                }
            } else {
                items.push_back(item);
            }
        }
    }

    // Make sure the timestamps column is at least as up to date as this monitor
    if (items.size()) {
        if (!freshenTimestamps(items[items.size() - 1].blk))
            EXIT_FAIL("Could not freshen timestamp file.");

        if (!loadTimestampFile(&ts_array, ts_cnt))
            EXIT_FAIL("Could not open timestamp file.");

        // If the user has not told us what to cache via the config file or the command line, we
        // cache transactions and traces if there are less than 1,000 of them...
        if (!write_opt && items.size() <= 1000)
            write_opt = (CACHE_TXS | CACHE_TRACES | CACHE_BYDEFAULT);
    }

    EXIT_NOMSG(true);
}

//------------------------------------------------------------------------
int xor_options(bool txs, bool traces) {
    int ret = CACHE_NONE;
    if (txs)
        ret |= CACHE_TXS;
    if (traces)
        ret |= CACHE_TRACES;
    return ret;
}

//------------------------------------------------------------------------
string_q report_cache(int opt) {
    ostringstream os;
    if (opt == CACHE_NONE) {
        os << "CACHE_NONE ";
    }
    if (opt & CACHE_TXS) {
        os << "CACHE_TXS ";
    }
    if (opt & CACHE_TRACES) {
        os << "CACHE_TRACES ";
    }
    if (opt & CACHE_BYCONFIG) {
        os << "CACHE_BYCONFIG ";
    }
    if (opt & CACHE_BYUSER) {
        os << "CACHE_BYUSER ";
    }
    if (opt & CACHE_BYDEFAULT) {
        os << "CACHE_BYDEFAULT ";
    }
    return os.str();
}
