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
    COption("creations", "n", "", OPT_SWITCH, "export contract creations instead of transaction list"),
    COption("selfdestructs", "u", "", OPT_SWITCH, "export contract selfdestructs instead of transaction list"),
    COption("accounting", "C", "", OPT_SWITCH, "export accounting records instead of transaction list"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("write_txs", "i", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("write_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("skip_ddos", "s", "", OPT_HIDDEN | OPT_TOGGLE, "toggle skipping over 2016 dDos transactions ('on' by default)"),  // NOLINT
    COption("max_traces", "m", "<uint64>", OPT_HIDDEN | OPT_FLAG, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)"),  // NOLINT
    COption("all_abis", "A", "", OPT_HIDDEN | OPT_SWITCH, "load all previously cached abi files"),
    COption("freshen", "f", "", OPT_HIDDEN | OPT_SWITCH, "freshen but do not print the exported data"),
    COption("freshen_max", "F", "<blknum>", OPT_HIDDEN | OPT_FLAG, "maximum number of records to process for --freshen option"),  // NOLINT
    COption("deltas", "D", "", OPT_HIDDEN | OPT_SWITCH, "for --balances option only, export only changes in balances"),
    COption("emitter", "M", "", OPT_SWITCH, "available for --logs option only, export will only export if the address emitted the event"),  // NOLINT
    COption("count", "U", "", OPT_SWITCH, "only available for --appearances mode, if present return only the number of records"),  // NOLINT
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_SKIP, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_SKIP, "last block to process (inclusive)"),
    COption("first_record", "c", "<blknum>", OPT_HIDDEN | OPT_FLAG, "the first record to process"),
    COption("max_records", "e", "<blknum>", OPT_HIDDEN | OPT_FLAG, "the maximum number of records to process before reporting"),  // NOLINT
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

        } else if (arg == "-n" || arg == "--creations") {
            creations = true;

        } else if (arg == "-u" || arg == "--selfdestructs") {
            selfdestructs = true;

        } else if (arg == "-C" || arg == "--accounting") {
            accounting = true;

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

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (startsWith(arg, "-F:") || startsWith(arg, "--freshen_max:")) {
            if (!confirmBlockNum("freshen_max", freshen_max, arg, latest))
                return false;

        } else if (arg == "-D" || arg == "--deltas") {
            deltas = true;

        } else if (arg == "-M" || arg == "--emitter") {
            emitter = true;

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmUint("start", start, arg))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmUint("end", end, arg))
                return false;

        } else if (startsWith(arg, "-c:") || startsWith(arg, "--first_record:")) {
            if (!confirmBlockNum("first_record", first_record, arg, latest))
                return false;

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--max_records:")) {
            if (!confirmBlockNum("max_records", max_records, arg, latest))
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
        CMonitor monitor;

        // below - don't change the next line, it sets bloom value also
        monitor.setValueByName("address", toLower(addr));
        // below - don't change the previous line, it sets bloom value also
        monitor.setValueByName("name", toLower(addr));

        if (!monitor.exists())
            EXIT_USAGE("Monitor file for address " + addr + " not found. Quitting...");
        string_q unused;
        if (monitor.isLocked(unused))
            LOG_ERR(
                "The cache file is locked. The program is either already "
                "running or it did not end cleanly the\n\tlast time it ran. "
                "Quit the already running program or, if it is not running, "
                "remove the lock\n\tfile: " +
                getMonitorPath(addr) + +".lck'. Proceeding anyway...");
        monitor.clearLocks();
        monitor.finishParse();
        monitors.push_back(monitor);
    }

    if (start != NOPOS)
        scanRange.first = start;
    if (end != NOPOS)
        scanRange.second = end;

    SHOW_FIELD(CTransaction, "traces");

    if ((appearances + receipts + logs + traces + balances) > 1)
        EXIT_USAGE("Please export only one of list, receipts, logs, traces, or balances. Quitting...");

    if (emitter && !logs)
        EXIT_USAGE("The emitter option is only available when exporting logs. Quitting...");

    if (emitter && monitors.size() > 1)
        EXIT_USAGE("The emitter option is only available when exporting logs from a single address. Quitting...");

    if (monitors.size() == 0)
        EXIT_USAGE("You must provide at least one Ethereum address. Quitting...");

    if (deltas && !balances)
        EXIT_USAGE("--deltas option is only available with --balances. Quitting...");

    if (count) {
        if (receipts || logs || traces || balances || emitter || deltas)
            EXIT_USAGE("--count option is only available with --appearances option. Quitting...");
        bool isText = expContext().exportFmt != JSON1 && expContext().exportFmt != API1;
        string_q format =
            getGlobalConfig("acctExport")->getConfigStr("display", "format", isText ? STR_DISPLAY_MONITORCOUNT : "");
        expContext().fmtMap["monitorcount_fmt"] = cleanFmt(format);
        expContext().fmtMap["header"] = isNoHeader ? "" : cleanFmt(format);
        for (auto monitor : monitors) {
            CMonitorCount monCount;
            monCount.address = monitor.address;
            monCount.fileSize = fileSize(getMonitorPath(monitor.address));
            monCount.nRecords = monCount.fileSize / sizeof(CAppearance_base);
            counts.push_back(monCount);
        }

    } else {
        // show certain fields and hide others
        // SEP4("default field hiding: " + defHide);
        manageFields(defHide, false);
        // SEP4("default field showing: " + defShow);
        string_q show =
            defShow +
            (isApiMode() ? "|CTransaction:encoding,function,input,etherGasCost,dollars|CTrace:traceAddress" : "");
        manageFields(show, true);

        // Load as many ABI files as we have
        if (!appearances && !balances) {
            abis.loadAbiKnown();
            if (all_abis)
                abis.loadAbisMonitors();
        }

        // Try to articulate the monitored addresses
        for (size_t i = 0; i < monitors.size(); i++) {
            CMonitor* monitor = &monitors[i];
            // abis.loadAbiByAddress(monitor->address);
            if (isContractAt(monitor->address, latestBlock))
                loadAbiAndCache(abis, monitor->address, false, errors);
            // abis.loadAbiKnown();
        }

        if (expContext().exportFmt != JSON1 && expContext().exportFmt != API1) {
            string_q format;

            format = getGlobalConfig("acctExport")->getConfigStr("display", "format", STR_DISPLAY_TRANSACTION);
            expContext().fmtMap["transaction_fmt"] = cleanFmt(format);

            if (format.empty())
                EXIT_USAGE("For non-json export a 'trans_fmt' string is required. Check your config file. Quitting...");
            if (!contains(toLower(format), "trace"))
                HIDE_FIELD(CTransaction, "traces");

            format = getGlobalConfig("acctExport")->getConfigStr("display", "receipt", STR_DISPLAY_RECEIPT);
            expContext().fmtMap["receipt_fmt"] = cleanFmt(format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "log", STR_DISPLAY_LOGENTRY);
            expContext().fmtMap["logentry_fmt"] = cleanFmt(format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "trace", STR_DISPLAY_TRACE);
            expContext().fmtMap["trace_fmt"] = cleanFmt(format);

            // This doesn't really work because CAppearance_base is not a subclass of CBaseNode. We phony it here for
            // future reference.
            format = getGlobalConfig("acctExport")->getConfigStr("display", "appearances", STR_DISPLAY_DISPLAYAPP);
            expContext().fmtMap["displayapp_fmt"] = cleanFmt(format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "balance", STR_DISPLAY_BALANCERECORD);
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

            format = getGlobalConfig("acctExport")->getConfigStr("display", "balance", STR_DISPLAY_BALANCEDELTA);
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
        HIDE_FIELD(CParameter, "str_default");
        HIDE_FIELD(CTransaction, "datesh");
        HIDE_FIELD(CTransaction, "time");
        HIDE_FIELD(CTransaction, "age");

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

        if (freshen)
            expContext().exportFmt = NONE1;
    }

    if (accounting) {
        if (addrs.size() != 1)
            EXIT_USAGE("You may only use --accounting option with a single address. Quitting...");
        if (freshen)
            EXIT_USAGE("Do not use the --accounting option with --freshen. Quitting...");
        if (appearances || logs || traces || balances)
            EXIT_USAGE("Do not use the --accounting option with other options. Quitting...");
        accountForAddr = addrs[0];
        articulate = true;
        // manageFields("CTransaction:input,receipt,articulatedTx,hash", false);
        manageFields("CTransaction:statements", true);
    }

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);
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
    creations = false;
    selfdestructs = false;
    accounting = false;
    articulate = false;
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    freshen = false;
    freshen_max = 5000;
    deltas = false;
    emitter = false;
    count = false;
    first_record = 0;
    max_records = NOPOS;
    // END_CODE_INIT

    nExported = 0;
    nProcessing = 0;
    nTransactions = 0;
    nCacheItemsRead = 0;
    nCacheItemsWritten = 0;
    scanRange.second = getLatestBlock_cache_ripe();
    items.clear();
    monitors.clear();
    counts.clear();

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    ts_array = NULL;
    ts_cnt = 0;
    Init();
    CDisplayApp::registerClass();
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
    nTransactions += nRecords;

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
        for (size_t i = first_record; i < min(((blknum_t)nRecords), (first_record + max_records)); i++) {
            if (buffer[i].blk == 0)
                prefundAddrMap[buffer[i].txid] = toLower(addr);
            if (buffer[i].txid == 99999 || buffer[i].txid == 99998 || buffer[i].txid == 99997)
                blkRewardMap[buffer[i].blk] = addr;
            apps.push_back(buffer[i]);
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
            blknum_t lastExport = monitor.getLastExportedBlock();
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
    nProcessing = items.size();

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

//-----------------------------------------------------------------------
bool COptions::reportOnNeighbors(void) {
    addr_name_map_t fromAndToMap;
    for (auto addr : fromNameExistsMap)
        fromAndToMap[addr.first] = 1L;
    for (auto addr : toNameExistsMap)
        if (fromAndToMap[addr.first] == 1L)
            fromAndToMap[addr.first] = 2L;

    CNameStatsArray fromAndToUnnamed;
    CNameStatsArray fromAndToNamed;
    for (auto addr : fromAndToMap) {
        CAccountName acct;
        acct.address = addr.first;
        getNamedAccount(acct, addr.first);
        if (acct.name.empty()) {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            fromAndToUnnamed.push_back(stats);
        } else {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            fromAndToNamed.push_back(stats);
        }
    }

    {
        sort(fromAndToNamed.begin(), fromAndToNamed.end());
        ostringstream os;
        bool frst = true;
        os << ", \"namedFromAndTo\": {";
        for (auto stats : fromAndToNamed) {
            if (fromAndToMap[stats.address] == 2) {
                if (!frst)
                    os << ",";
                os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
                   << "\", \"count\": " << stats.count << " }";
                frst = false;
            }
        }
        os << "}\n";
        expContext().fmtMap["meta"] += os.str();
    }

    {
        sort(fromAndToUnnamed.begin(), fromAndToUnnamed.end());
        ostringstream os;
        os << ", \"unNamedFromAndTo\": {";
        bool frst = true;
        for (auto stats : fromAndToUnnamed) {
            if (fromAndToMap[stats.address] == 2) {
                if (!frst)
                    os << ",";
                os << "\"" << stats.address << "\": " << stats.count;
                frst = false;
            }
        }
        os << "}";
        expContext().fmtMap["meta"] += os.str();
    }

    CNameStatsArray fromUnnamed;
    CNameStatsArray fromNamed;
    for (auto addr : fromNameExistsMap) {
        CAccountName acct;
        acct.address = addr.first;
        getNamedAccount(acct, addr.first);
        if (acct.name.empty()) {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            fromUnnamed.push_back(stats);
        } else {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            fromNamed.push_back(stats);
        }
    }

    {
        sort(fromNamed.begin(), fromNamed.end());
        ostringstream os;
        bool frst = true;
        os << ", \"namedFrom\": {";
        for (auto stats : fromNamed) {
            if (fromAndToMap[stats.address] != 2) {
                if (!frst)
                    os << ",";
                os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
                   << "\", \"count\": " << stats.count << " }";
                frst = false;
            }
        }
        os << "}\n";
        expContext().fmtMap["meta"] += os.str();
    }

    {
        sort(fromUnnamed.begin(), fromUnnamed.end());
        ostringstream os;
        os << ", \"unNamedFrom\": {";
        bool frst = true;
        for (auto stats : fromUnnamed) {
            if (fromAndToMap[stats.address] != 2) {
                if (!frst)
                    os << ",";
                os << "\"" << stats.address << "\": " << stats.count;
                frst = false;
            }
        }
        os << "}";
        expContext().fmtMap["meta"] += os.str();
    }

    CNameStatsArray toUnnamed;
    CNameStatsArray toNamed;
    for (auto addr : toNameExistsMap) {
        CAccountName acct;
        acct.address = addr.first;
        getNamedAccount(acct, addr.first);
        if (isZeroAddr(acct.address)) {
            acct.tags = "Contract Creation";
            acct.name = "Contract Creation";
        }
        if (acct.name.empty()) {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            toUnnamed.push_back(stats);
        } else {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            toNamed.push_back(stats);
        }
    }

    {
        sort(toNamed.begin(), toNamed.end());
        ostringstream os;
        bool frst = true;
        os << ", \"namedTo\": {";
        for (auto stats : toNamed) {
            if (fromAndToMap[stats.address] != 2) {
                if (!frst)
                    os << ",";
                os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
                   << "\", \"count\": " << stats.count << " }";
                frst = false;
            }
        }
        os << "}\n";
        expContext().fmtMap["meta"] += os.str();
    }

    {
        sort(toUnnamed.begin(), toUnnamed.end());
        ostringstream os;
        os << ", \"unNamedTo\": {";
        bool frst = true;
        for (auto stats : toUnnamed) {
            if (fromAndToMap[stats.address] != 2) {
                if (!frst)
                    os << ",";
                os << "\"" << stats.address << "\": " << stats.count;
                frst = false;
            }
        }
        os << "}";

        expContext().fmtMap["meta"] += os.str();
    }

    return true;
}
