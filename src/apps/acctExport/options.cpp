// TODO(tjayrush): If an abi file is newer than the monitor file - clear the cache
// TODO(tjayrush): accounting disallows freshen, apps, logs, receipts, statements, traces, but requires articulate
// TODO(tjayrush): accounting must be for one monitor address - why?
// TODO(tjayrush): accounting requires node balances - why?
// TODO(tjayrush): Used to do this: if any ABI files was newer, re-read abi and re-articulate in cache
// TODO(tjayrush): What does blkRewardMap do? Needs testing
// TODO(tjayrush): Reconciliation loads traces -- plus it reduplicates the isSuicide, isGeneration, isUncle shit
// TODO(tjayrush): If a monitor file is locked, remove the lock and move on (don't read) but don't wait either

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

extern const char* STR_MONITOR_LOCKED;
//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) to export"),
    COption("topics", "", "list<topic>", OPT_POSITIONAL, "filter by one or more log topics (only for --logs option)"),
    COption("fourbytes", "", "list<fourbyte>", OPT_POSITIONAL, "filter by one or more fourbytes (only for transactions and trace options)"),  // NOLINT
    COption("appearances", "p", "", OPT_SWITCH, "export a list of appearances"),
    COption("receipts", "r", "", OPT_SWITCH, "export receipts instead of transactional data"),
    COption("logs", "l", "", OPT_SWITCH, "export logs instead of transactional data"),
    COption("traces", "t", "", OPT_SWITCH, "export traces instead of transactional data"),
    COption("neighbors", "n", "", OPT_SWITCH, "export the neighbors of the given address"),
    COption("accounting", "C", "", OPT_SWITCH, "attach accounting records to the exported data (applies to transactions export only)"),  // NOLINT
    COption("statements", "A", "", OPT_SWITCH, "for the accounting options only, export only statements"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("cache", "i", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("cache_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("count", "U", "", OPT_SWITCH, "only available for --appearances mode, if present, return only the number of records"),  // NOLINT
    COption("first_record", "c", "<uint64>", OPT_FLAG, "the first record to process"),
    COption("max_records", "e", "<uint64>", OPT_FLAG, "the maximum number of records to process"),
    COption("relevant", "", "", OPT_SWITCH, "for log and accounting export only, export only logs relevant to one of the given export addresses"),  // NOLINT
    COption("emitter", "", "list<addr>", OPT_FLAG, "for log export only, export only logs if emitted by one of these address(es)"),  // NOLINT
    COption("topic", "", "list<topic>", OPT_FLAG, "for log export only, export only logs with this topic(s)"),
    COption("asset", "", "list<addr>", OPT_FLAG, "for the accounting options only, export statements only for this asset"),  // NOLINT
    COption("flow", "f", "enum[in|out|zero]", OPT_FLAG, "for the accounting options only, export statements with incoming, outgoing, or zero value"),  // NOLINT
    COption("factory", "y", "", OPT_SWITCH, "for --traces only, report addresses created by (or self-destructed by) the given address(es)"),  // NOLINT
    COption("load", "", "<string>", OPT_HIDDEN | OPT_FLAG, "a comma separated list of dynamic traversers to load"),
    COption("reversed", "", "", OPT_HIDDEN | OPT_SWITCH, "produce results in reverse chronological order"),
    COption("first_block", "F", "<blknum>", OPT_FLAG, "first block to process (inclusive)"),
    COption("last_block", "L", "<blknum>", OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more addresses."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CAddressArray addrs;
    CTopicArray topics;
    CAddressArray emitter;
    CStringArray topic;
    CAddressArray asset;
    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
    // END_CODE_LOCAL_INIT

    blknum_t latest = meta.client;
    string_q origCmd = command;

    Init();
    explode(arguments, command, ' ');

    // Weirdness, if the user doesn't provide a valid address, topic, or fourbyte, report bad addr
    string_q term, legit;
    for (auto arg : arguments) {
        if (!contains(arg, "-")) {  // is positional
            if (isAddress(arg) || isTopic(arg) || isFourbyte(arg)) {
                legit = arg;
            }
            term = arg;
        }
    }
    if (legit.empty() && !term.empty())
        return parseAddressList(this, addrs, term);
    // Weirdness, if the user doesn't provide a valid address, topic, or fourbyte, report bad addr

    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
        } else if (arg == "--cache_tx") {
            cache = true;
            // BEG_CODE_AUTO
        } else if (arg == "-p" || arg == "--appearances") {
            appearances = true;

        } else if (arg == "-r" || arg == "--receipts") {
            receipts = true;

        } else if (arg == "-l" || arg == "--logs") {
            logs = true;

        } else if (arg == "-t" || arg == "--traces") {
            traces = true;

        } else if (arg == "-n" || arg == "--neighbors") {
            neighbors = true;

        } else if (arg == "-C" || arg == "--accounting") {
            accounting = true;

        } else if (arg == "-A" || arg == "--statements") {
            statements = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-i" || arg == "--cache") {
            cache = true;

        } else if (arg == "-R" || arg == "--cache_traces") {
            cache_traces = true;

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (startsWith(arg, "-c:") || startsWith(arg, "--first_record:")) {
            if (!confirmUint("first_record", first_record, arg))
                return false;
        } else if (arg == "-c" || arg == "--first_record") {
            return flag_required("first_record");

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--max_records:")) {
            if (!confirmUint("max_records", max_records, arg))
                return false;
        } else if (arg == "-e" || arg == "--max_records") {
            return flag_required("max_records");

        } else if (arg == "--relevant") {
            relevant = true;

        } else if (startsWith(arg, "--emitter:")) {
            arg = substitute(substitute(arg, "-:", ""), "--emitter:", "");
            if (!parseAddressList(this, emitter, arg))
                return false;
        } else if (arg == "--emitter") {
            return flag_required("emitter");

        } else if (startsWith(arg, "--topic:")) {
            arg = substitute(substitute(arg, "-:", ""), "--topic:", "");
            if (!parseTopicList2(this, topic, arg))
                return false;
        } else if (arg == "--topic") {
            return flag_required("topic");

        } else if (startsWith(arg, "--asset:")) {
            arg = substitute(substitute(arg, "-:", ""), "--asset:", "");
            if (!parseAddressList(this, asset, arg))
                return false;
        } else if (arg == "--asset") {
            return flag_required("asset");

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--flow:")) {
            if (!confirmEnum("flow", flow, arg))
                return false;
        } else if (arg == "-f" || arg == "--flow") {
            return flag_required("flow");

        } else if (arg == "-y" || arg == "--factory") {
            factory = true;

        } else if (startsWith(arg, "--load:")) {
            load = substitute(substitute(arg, "-:", ""), "--load:", "");
        } else if (arg == "--load") {
            return flag_required("load");

        } else if (arg == "--reversed") {
            reversed = true;

        } else if (startsWith(arg, "-F:") || startsWith(arg, "--first_block:")) {
            if (!confirmBlockNum("first_block", first_block, arg, latest))
                return false;
        } else if (arg == "-F" || arg == "--first_block") {
            return flag_required("first_block");

        } else if (startsWith(arg, "-L:") || startsWith(arg, "--last_block:")) {
            if (!confirmBlockNum("last_block", last_block, arg, latest))
                return false;
        } else if (arg == "-L" || arg == "--last_block") {
            return flag_required("last_block");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else if (isAddress(arg)) {
            if (!parseAddressList(this, addrs, arg))
                return false;

        } else if (isTopic(arg)) {
            if (!parseTopicList2(this, topics, arg))
                return false;

        } else {
            if (!parseFourbyteList(this, fourbytes, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (first_record == 0) {
        return usage("Should not happen: " + uint_2_Str(first_record) + "  must be at least 1.");
    }

    if (!isApiMode() && (max_records == 250 || max_records == 0))
        max_records = (((size_t)-100000000));  // this is a very large number that won't wrap

    if (!isArchiveNode() && accounting) {
        return usage("The --accounting option requires historical balances which your RPC server does not provide.");
    }

    for (auto e : emitter) {
        logFilter.emitters.push_back(e);
    }

    for (auto t : topics) {
        logFilter.topics.push_back(t);
    }

    for (auto t : topic) {
        logFilter.topics.push_back(t);
    }

    for (auto addr : asset) {  // asset is an array even though it's singular
        ledgerManager.setAssetFilter(addr);
    }

    if (!loadNames()) {
        return usage("Could not load names database.");
    }

    exportRange = make_pair(first_block, last_block);

    for (auto addr : addrs) {
        CMonitor monitor;
        monitor.setValueByName("address", toLower(addr));
        monitor.setValueByName("name", toLower(addr));
        monitor.finishParse();
        monitor.isStaging = !fileExists(monitor.getPathToMonitor(monitor.address, false));

        if (ledgerManager.accountedFor.empty()) {
            ledgerManager.accountedFor = monitor.address;
            ledgerManager.name.address = monitor.address;
            findName(monitor.address, ledgerManager.name);
            ledgerManager.name.petname = addr_2_Petname(ledgerManager.accountedFor, '-');  // order matters
            ledgerManager.name.isContract = !getCodeAt(monitor.address, latest).empty();
        }

        allMonitors.push_back(monitor);
    }

    if (appearances || count) {
        articulate = false;
    }

    if (articulate) {
        abi_spec.loadAbisFromKnown();
        for (auto monitor : allMonitors) {
            if (isContractAt(monitor.address, meta.client))
                abi_spec.loadAbiFromEtherscan(monitor.address);
        }
    }

    if (!setDisplayFormatting()) {
        return false;
    }

    // TODO: This can be removed
    CMonitor m;
    cleanFolder(m.getPathToMonitor("", true));

    if (count) {
        cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CMonitorCount)->m_ClassName);
        for (auto monitor : allMonitors) {
            CMonitorCount monCount;
            monCount.address = monitor.address;
            // CMonitorCount doesn't have a petname
            // monCount.petname = addr_2_Petname(monCount.address, '-');
            monCount.fileSize = fileSize(monitor.getPathToMonitor(monitor.address, false));
            monCount.nRecords = monitor.getRecordCnt(monitor.getPathToMonitor(monitor.address, false));
            cout << ((isJson() && !firstOut) ? ", " : "");
            cout << monCount;
            firstOut = false;
            if (shouldQuit())
                break;
        }
        cout << exportPostamble(errors, expContext().fmtMap["meta"]);
        return false;

    } else {
        if (load.empty()) {
            if (!loadMonitors())
                return false;

        } else {
            string_q fileName = cacheFolder_objs + load;
            LOG_INFO("Trying to load dynamic library ", fileName);

            if (!fileExists(fileName)) {
                replace(fileName, "/objs/", "/objs/lib");
                fileName = fileName + ".so";
                LOG_INFO("Trying to load dynamic library ", fileName);
            }

            if (!fileExists(fileName)) {
                fileName = substitute(fileName, ".so", ".dylib");
                LOG_INFO("Trying to load dynamic library ", fileName);
            }

            if (fileExists(fileName)) {
                LOG_INFO(bYellow, "Found dynamic library ", fileName, cOff);
                load = fileName;

            } else {
                return usage("Could not load dynamic traverser for " + fileName + ".");
            }
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_PREFUND);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    appearances = false;
    receipts = false;
    logs = false;
    traces = false;
    neighbors = false;
    accounting = false;
    statements = false;
    articulate = false;
    // clang-format off
    cache = getGlobalConfig("acctExport")->getConfigBool("settings", "cache", false);
    cache_traces = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_traces", false);
    // clang-format on
    count = false;
    first_record = 1;
    max_records = 250;
    relevant = false;
    flow = "";
    factory = false;
    load = "";
    reversed = false;
    // END_CODE_INIT

    // clang-format off
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    // clang-format on

    if (!cache && getGlobalConfig("acctExport")->getConfigBool("settings", "cache_txs", false))
        cache = true;  // backwards compat

    meta = getMetaData();

    allMonitors.clear();

    ledgerManager.appArray.clear();
    ledgerManager.accountedFor = "";
    ledgerManager.name.address = "";
    ledgerManager.name.petname = "";

    // We don't clear these because they are part of meta data
    // prefundAddrMap.clear();
    // blkRewardMap.clear();
    // toNameExistsMap.clear();
    // fromNameExistsMap.clear();
    // abiMap.clear();

    minArgs = 0;
    fileRange = make_pair(NOPOS, NOPOS);
    allMonitors.clear();

    // Establish folders. This may be redundant, but it's cheap.
    establishCacheFolders();
    establishIndexFolders();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    CMonitorCount::registerClass();
    CAppearanceDisplay::registerClass();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("An `address` must start with '0x' and be forty-two characters long.");
    notes.push_back("Articulating the export means turn the EVM's byte data into human-readable text (if possible).");
    notes.push_back("For the --logs option, you may optionally specify one or more --emitter, one or more --topics, or both.");  // NOLINT
    notes.push_back("The --logs option is significantly faster if you provide an --emitter or a --topic.");
    notes.push_back("Neighbors include every address that appears in any transaction in which the export address also appears.");  // NOLINT
    notes.push_back("If provided, --max_records dominates, also, if provided, --first_record overrides --first_block.");
    // clang-format on
    // END_CODE_NOTES

    // clang-format off
    configs.push_back("`cache`: write transactions to the cache (see notes).");
    configs.push_back("`cache_traces`: write traces to the cache (see notes).");
    configs.push_back("`skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).");
    configs.push_back("`max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction | is (default = 250).");  // NOLINT
    // clang-format on

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::setDisplayFormatting(void) {
    bool isText = expContext().exportFmt != JSON1;
    if (count) {
        string_q format =
            getGlobalConfig("acctExport")->getConfigStr("display", "format", isText ? STR_DISPLAY_MONITORCOUNT : "");
        expContext().fmtMap["monitorcount_fmt"] = cleanFmt(format);
        expContext().fmtMap["header"] = noHeader ? "" : cleanFmt(format);

    } else {
        string_q hide = substitute(defHide, "|CLogEntry: data, topics", "");
        manageFields(hide, false);
        string_q show = defShow;
        show += (isApiMode() ? "|CTransaction:encoding,function,input,etherGasCost|CTrace:traceAddress" : "");
        manageFields(show, true);
        manageFields("CTrace: blockHash, blockNumber, transactionHash, transactionIndex", false);  // hide
        if (isText) {
            string_q format;

            format = getGlobalConfig("acctExport")->getConfigStr("display", "format", STR_DISPLAY_TRANSACTION);
            expContext().fmtMap["transaction_fmt"] = cleanFmt(format);
            manageFields("CTransaction:" + format);

            if (format.empty())
                return usage("For non-json export a 'trans_fmt' string is required. Check your config file.");
            if (!contains(toLower(format), "trace"))
                HIDE_FIELD(CTransaction, "traces");

            format = getGlobalConfig("acctExport")->getConfigStr("display", "receipt", STR_DISPLAY_RECEIPT);
            expContext().fmtMap["receipt_fmt"] = cleanFmt(format);
            manageFields("CReceipt:" + format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "log", STR_DISPLAY_LOGENTRY);
            expContext().fmtMap["logentry_fmt"] = cleanFmt(format);
            manageFields("CLogEntry:" + format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "statement", STR_DISPLAY_RECONCILIATION);
            expContext().fmtMap["reconciliation_fmt"] = cleanFmt(format);
            manageFields("CReconciliation:" + format);
            if (statements)
                expContext().fmtMap["header"] = noHeader ? "" : cleanFmt(format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "neighbor", STR_DISPLAY_APPEARANCE);
            replace(format, "[{TC}]\t", "");
            expContext().fmtMap["appearance_fmt"] = cleanFmt(format);
            manageFields("CAppearance:" + format);
            if (neighbors)
                expContext().fmtMap["header"] = noHeader ? "" : cleanFmt(format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "trace", STR_DISPLAY_TRACE);
            expContext().fmtMap["trace_fmt"] = cleanFmt(format);
            manageFields("CTrace:" + format);

            // This doesn't really work because CAppearance_mon is not a subclass of CBaseNode. We phony it here
            // for future reference.
            format =
                getGlobalConfig("acctExport")->getConfigStr("display", "appearances", STR_DISPLAY_APPEARANCEDISPLAY);
            HIDE_FIELD(CAppearanceDisplay, "tags");
            HIDE_FIELD(CAppearanceDisplay, "symbol");
            HIDE_FIELD(CAppearanceDisplay, "source");
            HIDE_FIELD(CAppearanceDisplay, "decimals");
            HIDE_FIELD(CAppearanceDisplay, "petname");
            HIDE_FIELD(CAppearanceDisplay, "isCustom");
            HIDE_FIELD(CAppearanceDisplay, "isPrefund");
            HIDE_FIELD(CAppearanceDisplay, "isContract");
            HIDE_FIELD(CAppearanceDisplay, "isErc20");
            HIDE_FIELD(CAppearanceDisplay, "isErc721");
            if (!verbose) {
                replace(format, "\t[{TIMESTAMP}]\t[{DATE}]\t[{NAME}]", "");
            } else {
                SHOW_FIELD(CAppearanceDisplay, "timestamp");
                SHOW_FIELD(CAppearanceDisplay, "date");
                SHOW_FIELD(CAppearanceDisplay, "name");
            }
            expContext().fmtMap["appearancedisplay_fmt"] = cleanFmt(format);
            manageFields("CAppearanceDisplay:" + format);
        }
        HIDE_FIELD(CFunction, "stateMutability");
        HIDE_FIELD(CParameter, "strDefault");
        HIDE_FIELD(CParameter, "components");
        HIDE_FIELD(CParameter, "internalType");
        HIDE_FIELD(CTransaction, "datesh");
        HIDE_FIELD(CTransaction, "time");
        HIDE_FIELD(CTransaction, "age");
        SHOW_FIELD(CTransaction, "traces");

        expContext().fmtMap["header"] = "";
        if (!noHeader) {
            if (traces) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["trace_fmt"]);
            } else if (receipts) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["receipt_fmt"]);
            } else if (statements) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["reconciliation_fmt"]);
            } else if (neighbors) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["appearance_fmt"]);
            } else if (logs) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["logentry_fmt"]);
            } else if (appearances) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["appearancedisplay_fmt"]);
            } else {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["transaction_fmt"]);
            }
        }

        if (accounting) {
            articulate = !getEtherscanKey(false).empty();
            manageFields("CTransaction:statements", true);
            if (statements && isText) {
                string_q format =
                    getGlobalConfig("acctExport")->getConfigStr("display", "statement", STR_DISPLAY_RECONCILIATION);
                expContext().fmtMap["header"] = noHeader ? "" : cleanFmt(format);
                expContext().fmtMap["reconciliation_fmt"] = cleanFmt(format);
                manageFields("CReconciliation:" + format);
            }
        }

        if (logs) {
            SHOW_FIELD(CLogEntry, "blockNumber");
            SHOW_FIELD(CLogEntry, "transactionIndex");
            SHOW_FIELD(CLogEntry, "transactionHash");
            SHOW_FIELD(CReceipt, "blockNumber");
            SHOW_FIELD(CReceipt, "transactionIndex");
        }

        if (traces) {
            SHOW_FIELD(CTrace, "blockNumber");
            SHOW_FIELD(CTrace, "transactionIndex");
            SHOW_FIELD(CTrace, "blockHash");
            SHOW_FIELD(CTrace, "transactionHash");
        }
    }
    articulate = (articulate && (!isTestMode() || getEnvStr("TEST_NO_ARTICULATION") != "true"));

    // TODO(tjayrush): This doesn't work for some reason (see test case acctExport_export_logs.txt)
    if (!articulate)
        HIDE_FIELD(CLogEntry, "compressedTx");

    if (receipts) {
        SHOW_FIELD(CReceipt, "blockNumber");
        SHOW_FIELD(CReceipt, "transactionIndex");
    } else if (appearances) {
        manageFields("CAccountName:all", false);
        manageFields(verbose ? "CAccountName:address,name" : "CAccountName:address,name,timestamp,date", true);
    }

    return true;
}

//-----------------------------------------------------------------------
bool COptions::isEmitter(const address_t& test) const {
    for (auto monitor : allMonitors) {
        if (monitor.address == test) {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool COptions::isRelevant(const CLogEntry& log) const {
    string_q str = toLower(log.Format(STR_DISPLAY_LOGENTRY));
    for (auto monitor : allMonitors) {
        if (contains(str, monitor.address.substr(2))) {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool COptions::fourByteFilter(const string_q& input) const {
    ASSERT(!opt->watch);
    if (fourbytes.empty()) {
        return true;
    }

    for (auto four : fourbytes) {
        if (startsWith(input, four)) {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------
string_q CScrapeStatistics::Header(const string_q& fmt) const {
    return Format(substitute(fmt, "{", "{p:"));
}

//-----------------------------------------------------------------------
void COptions::writePerformanceData(void) {
    if (stats.nFiles == stats.nSkipped)
        return;

    string_q statsFile = rootConfigs + "perf/performance_scraper.csv";

    string_q fmt = substitute(STR_DISPLAY_SCRAPESTATISTICS, "\t", ",");
    if (!fileExists(statsFile)) {
        ostringstream header;
        header << stats.Header(fmt) << endl;
        stringToAsciiFile(statsFile, header.str());
    }

    ostringstream data;
    data << stats.Format(fmt) << endl;
    appendToAsciiFile(statsFile, data.str());
}

//-----------------------------------------------------------------------
const char* STR_MONITOR_LOCKED =
    "The cache file is locked. The program is either already "
    "running or it did not end cleanly the\n\tlast time it ran. "
    "Quit the already running program or, if it is not running, "
    "remove the lock\n\tfile: '{0}.lck'.";
