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
    COption("statements", "A", "", OPT_SWITCH, "export reconciliations instead of transactional data (requires --accounting option)"),  // NOLINT
    COption("neighbors", "n", "", OPT_SWITCH, "export the neighbors of the given address"),
    COption("accounting", "C", "", OPT_SWITCH, "attach accounting records to the exported data (applies to transactions export only)"),  // NOLINT
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("cache", "i", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("cache_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("factory", "y", "", OPT_SWITCH, "scan for contract creations from the given address(es) and report address of those contracts"),  // NOLINT
    COption("count", "U", "", OPT_SWITCH, "only available for --appearances mode, if present, return only the number of records"),  // NOLINT
    COption("first_record", "c", "<blknum>", OPT_FLAG, "the first record to process"),
    COption("max_records", "e", "<blknum>", OPT_FLAG, "the maximum number of records to process before reporting"),
    COption("relevant", "", "", OPT_SWITCH, "for log and accounting export only, export only logs relevant to one of the given export addresses"),  // NOLINT
    COption("emitter", "", "list<addr>", OPT_FLAG, "for log export only, export only logs if emitted by one of these address(es)"),  // NOLINT
    COption("topic", "", "list<topic>", OPT_FLAG, "for log export only, export only logs with this topic(s)"),
    COption("asset", "", "list<addr>", OPT_FLAG, "for the statements option only, export only reconciliations for this asset"),  // NOLINT
    COption("clean", "", "", OPT_SWITCH, "clean (i.e. remove duplicate appearances) from all existing monitors"),
    COption("freshen", "f", "", OPT_HIDDEN | OPT_SWITCH, "freshen but do not print the exported data"),
    COption("staging", "s", "", OPT_HIDDEN | OPT_SWITCH, "enable search of staging (not yet finalized) folder"),
    COption("unripe", "u", "", OPT_HIDDEN | OPT_SWITCH, "enable search of unripe (neither staged nor finalized) folder (assumes --staging)"),  // NOLINT
    COption("load", "", "<string>", OPT_HIDDEN | OPT_FLAG, "a comma separated list of dynamic traversers to load"),
    COption("reversed", "", "", OPT_HIDDEN | OPT_SWITCH, "produce results in reverse chronological order"),
    COption("by_date", "b", "", OPT_HIDDEN | OPT_SWITCH, "produce results sorted by date (report by address otherwise)"),  // NOLINT
    COption("summarize_by", "z", "enum[yearly|quarterly|monthly|weekly|daily|hourly|blockly|tx]", OPT_HIDDEN | OPT_FLAG, "for --accounting only, summarize reconciliations by this time period"),  // NOLINT
    COption("deep", "D", "", OPT_HIDDEN | OPT_SWITCH, "for --neighbors option only, dig deeply into detail (otherwise, to and from only)"),  // NOLINT
    COption("first_block", "F", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("last_block", "L", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more addresses."),
    COption("delete", "", "", OPT_SWITCH, "delete a monitor, but do not remove it"),
    COption("undelete", "", "", OPT_SWITCH, "undelete a previously deleted monitor"),
    COption("remove", "", "", OPT_SWITCH, "remove a previously deleted monitor"),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    replaceAll(command, "--delete", "--deleteMe");

    // BEG_CODE_LOCAL_INIT
    CAddressArray addrs;
    CTopicArray topics;
    CAddressArray emitter;
    CStringArray topic;
    CAddressArray asset;
    bool freshen = false;
    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
    bool deleteMe = false;
    bool undelete = false;
    bool remove = false;
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
        } else if (arg == "--clean") {
            clean = true;
        }
    }
    if (!clean && legit.empty() && !term.empty())
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

        } else if (arg == "-A" || arg == "--statements") {
            statements = true;

        } else if (arg == "-n" || arg == "--neighbors") {
            neighbors = true;

        } else if (arg == "-C" || arg == "--accounting") {
            accounting = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-i" || arg == "--cache") {
            cache = true;

        } else if (arg == "-R" || arg == "--cache_traces") {
            cache_traces = true;

        } else if (arg == "-y" || arg == "--factory") {
            factory = true;

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (startsWith(arg, "-c:") || startsWith(arg, "--first_record:")) {
            if (!confirmBlockNum("first_record", first_record, arg, latest))
                return false;
        } else if (arg == "-c" || arg == "--first_record") {
            return flag_required("first_record");

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--max_records:")) {
            if (!confirmBlockNum("max_records", max_records, arg, latest))
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

        } else if (arg == "--clean") {
            clean = true;

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg == "-s" || arg == "--staging") {
            staging = true;

        } else if (arg == "-u" || arg == "--unripe") {
            unripe = true;

        } else if (startsWith(arg, "--load:")) {
            load = substitute(substitute(arg, "-:", ""), "--load:", "");
        } else if (arg == "--load") {
            return flag_required("load");

        } else if (arg == "--reversed") {
            reversed = true;

        } else if (arg == "-b" || arg == "--by_date") {
            by_date = true;

        } else if (startsWith(arg, "-z:") || startsWith(arg, "--summarize_by:")) {
            if (!confirmEnum("summarize_by", summarize_by, arg))
                return false;
        } else if (arg == "-z" || arg == "--summarize_by") {
            return flag_required("summarize_by");

        } else if (arg == "-D" || arg == "--deep") {
            deep = true;

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

        } else if (arg == "--deleteMe") {
            deleteMe = true;

        } else if (arg == "--undelete") {
            undelete = true;

        } else if (arg == "--remove") {
            remove = true;

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

    if (deleteMe)
        crudCommands.push_back("delete");

    if (undelete)
        crudCommands.push_back("undelete");

    if (remove)
        crudCommands.push_back("remove");

    if (Mocked(""))
        return false;

    for (auto t : topics)
        logFilter.topics.push_back(t);

    if (!isApiMode() && max_records == 250)
        max_records = NOPOS;

    freshenOnly = freshen;

    if (!accounting && !summarize_by.empty())
        return usage("You may use --summarized_by only with the --accounting option.");

    if (!bloomsAreInitalized())
        return usage("Bloom filters not found in " + indexFolder_blooms +
                     ". You must run 'chifra init' before running this command.");

    if (clean) {
        if (!process_clean())
            return usage("Clean function returned false.");
        return false;
    }

    // Handle the easy cases first...
    if (isCrudCommand())
        return process_rm(addrs);

    if (accounting && !isArchiveNode())
        return usage("The --accounting option requires historical balances which your RPC server does not provide.");

    // We need at least one address to scrape...
    if (addrs.size() == 0)
        return usage("You must provide at least one Ethereum address.");

    if (topics.size() && !logs)
        return usage("Use the topics option only with --logs.");

    if (fourbytes.size() && (logs || receipts || statements || appearances))
        return usage("Use the fourbytes option only with non-logs commands.");

    if ((appearances + receipts + statements + logs + traces) > 1)
        return usage("Please export only one of list, receipts, statements, logs, or traces.");

    if (emitter.size() > 0 && !logs)
        return usage("The --emitter option is only available when exporting logs.");

    if (!topics.empty() && !logs)
        return usage("The --topic option is only available when exporting logs.");

    if (asset.size() > 0 && !statements)
        return usage("The --asset option is only available when exporting statements.");

    if (factory && !traces)
        return usage("The --factory option is only available when exporting traces.");

    if (count && (receipts || statements || logs || traces || factory))
        return usage("--count option is only available with --appearances option.");

    if (accounting && (addrs.size() != 1))
        return usage("You may only use --accounting option with a single address.");

    if (accounting && freshenOnly)
        return usage("Do not use the --accounting option with --freshen.");

    if ((accounting) && (appearances || logs || traces || receipts || statements))
        return usage("Do not use the --accounting option with other options.");

    if (freshenOnly && (logs || traces || receipts || statements))
        return usage("Do not use the --freshen option with other options.");

    for (auto e : emitter)
        logFilter.emitters.push_back(e);

    for (auto t : topic)
        logFilter.topics.push_back(t);

    for (auto addr : asset)
        assetFilter[addr] = true;

    if (!loadNames())
        return usage("Could not load names database.");

    // Where will we start?
    blknum_t nextBlockToVisit = NOPOS;

    for (auto addr : addrs) {
        CMonitor monitor;
        monitor.setValueByName("address", toLower(addr));
        monitor.setValueByName("name", toLower(addr));
        monitor.clearMonitorLocks();
        monitor.finishParse();
        monitor.isStaging = !fileExists(monitor.getPathToMonitor(monitor.address, false));
        if (monitor.monitorExists()) {
            string_q unused;
            if (monitor.isMonitorLocked(unused))
                LOG_ERR(
                    "The cache file is locked. The program is either already "
                    "running or it did not end cleanly the\n\tlast time it ran. "
                    "Quit the already running program or, if it is not running, "
                    "remove the lock\n\tfile: " +
                    monitor.getPathToMonitor(addr, false) + ".lck'. Proceeding anyway...");
            string_q msg;
            if (monitor.isMonitorLocked(msg))  // If locked, we fail
                return usage(msg);
            nextBlockToVisit = min(nextBlockToVisit, monitor.getNextBlockToVisit());
            LOG_TEST("Monitor found for", addr, false);
            LOG_TEST("Last block in monitor", monitor.getLastBlockInMonitorPlusOne(), false);
        } else {
            if (!isTestMode())
                LOG_WARN("Monitor not found for ", addr + ". Continuing anyway.");
            nextBlockToVisit = 0;  // monitor.getNextBlockToVisit()
        }
        if (accountedFor.address.empty()) {
            accountedFor.address = monitor.address;
            findName(monitor.address, accountedFor);
            accountedFor.isContract = !getCodeAt(monitor.address, latest).empty();
        }
        allMonitors.push_back(monitor);
    }

    if (appearances || count)
        articulate = false;
    if (articulate) {
        abi_spec.loadAbisFromKnown();
        for (auto monitor : allMonitors) {
            if (isContractAt(monitor.address, meta.client))
                abi_spec.loadAbiFromEtherscan(monitor.address);
        }
    }

    if (!setDisplayFormatting())
        return false;

    // Are we visiting unripe and/or staging in our search?
    if (staging && unripe)
        return usage("Please choose only one of --staging or --unripe.");

    // Last block depends on scrape type or user input `end` option (with appropriate check)
    blknum_t lastBlockToVisit = max((blknum_t)1, unripe ? meta.unripe : staging ? meta.staging : meta.finalized);
    listRange = make_pair((nextBlockToVisit == NOPOS ? 0 : nextBlockToVisit), lastBlockToVisit);

    if (isTestMode() && (staging || unripe))
        return usage("--staging and --unripe are disabled for testing.");

    if (unripe && (cache || cache_traces)) {
        cache = cache_traces = false;
        LOG_INFO("Turning off caching for unripe blocks.");
    }

    // always freshen (i.e. query blooms) up to every block
    if (!process_freshen())
        return usage("freshen returns false.");

    if (first_block > last_block)
        return usage("--first_block must be less than or equal to --last_block.");
    exportRange = make_pair(first_block, last_block);

    if (count) {
        cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CMonitorCount)->m_ClassName);
        for (auto monitor : allMonitors) {
            CMonitorCount monCount;
            monCount.address = monitor.address;
            monCount.fileSize = monitor.getFileSize(monitor.getPathToMonitor(monitor.address, false));
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
            if (!loadAllAppearances())
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
    statements = false;
    neighbors = false;
    accounting = false;
    articulate = false;
    // clang-format off
    cache = getGlobalConfig("acctExport")->getConfigBool("settings", "cache", false);
    cache_traces = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_traces", false);
    // clang-format on
    factory = false;
    count = false;
    first_record = 0;
    max_records = 250;
    relevant = false;
    clean = false;
    staging = false;
    unripe = false;
    load = "";
    reversed = false;
    by_date = false;
    summarize_by = "";
    deep = false;
    // clang-format off
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    // clang-format on
    // END_CODE_INIT

    if (!cache && getGlobalConfig("acctExport")->getConfigBool("settings", "cache_txs", false))
        cache = true;  // backwards compat

    meta = getMetaData();
    listRange = make_pair(0, NOPOS);

    allMonitors.clear();
    monApps.clear();

    accountedFor.address = "";

    // We don't clear these because they are part of meta data
    // prefundAddrMap.clear();
    // blkRewardMap.clear();
    // toNameExistsMap.clear();
    // fromNameExistsMap.clear();
    // abiMap.clear();

    minArgs = 0;
    fileRange = make_pair(NOPOS, NOPOS);
    allMonitors.clear();
    possibles.clear();
    reportFreq = reportDef = 7;
    slowQueries = 0;
    maxSlowQueries =
        isApiMode() ? getGlobalConfig("acctExport")->getConfigInt("settings", "max_slow_queries", 50) : NOPOS;

    // Establish folders. This may be redundant, but it's cheap.
    establishMonitorFolders();
    establishFolder(indexFolder);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_ripe);
    establishFolder(cacheFolder_tmp);
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
    notes.push_back("For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.");  // NOLINT
    notes.push_back("The --logs option is significantly faster if you provide an --emitter or a --topic.");
    notes.push_back("Neighbors include every address that appears in any transaction in which the export address also appears.");  // NOLINT
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
    if (count) {
        bool isText = expContext().exportFmt != JSON1 && expContext().exportFmt != API1;
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
        if (expContext().exportFmt != JSON1 && expContext().exportFmt != API1) {
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
            HIDE_FIELD(CAppearanceDisplay, "description");
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
            articulate = true;
            manageFields("CTransaction:statements", true);
            if (statements) {
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
            SHOW_FIELD(CReceipt, "blockNumber");
            SHOW_FIELD(CReceipt, "transactionIndex");
        }
    }
    articulate = (articulate && (!isTestMode() || getEnvStr("TEST_NO_ART") != "true"));

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

// TODO(tjayrush): If an abi file is changed, we should re-articulate.
// TODO(tjayrush): accounting can not be freshen, appearances, logs, receipts, statements, traces, but must be
// TODO(tjayrush): articulate - why?
// TODO(tjayrush): accounting must be exportFmt API1 - why?
// TODO(tjayrush): accounting must be for one monitor address - why?
// TODO(tjayrush): accounting requires node balances - why?
// TODO(tjayrush): Used to do this: if any ABI files was newer, re-read abi and re-articulate in cache
// TODO(tjayrush): What does blkRewardMap do? Needs testing
// TODO(tjayrush): Reconciliation loads traces -- plus it reduplicates the isSuicide, isGeneration, isUncle shit
// TODO(tjayrush): writeLastEncountered is weird (in fact removed -- used to keep freshen from revisiting blocks twice
// TODO(tjayrush): writeLastBlockInMonitor is really weird
// TODO(tjayrush): We used to write traces sometimes
// TODO(tjayrush): We used to cache the monitored txs - I think it was pretty fast (we used the monitor staging folder)
// TODO(tjayrush): We used to do a ten address thing that would scan the index for ten addrs at a time and then
// TODO(tjayrush): (oddly) scan for --freshen on acctExport one address at a time. All that got removed when we
// TODO(tjayrush): made chifra so light weight. It might be interesting to put it back in (or re-write it actually)
// TODO(tjayrush): While looking into that, make sure to take advantage of 'needsUpdate' which would do this:
// TODO(tjayrush): (1) store the number of records in monitor file, (2) freshen monitor file, (3) only --freshen if the
// TODO(tjayrush): We used to check to see if the monitor file was locked. That may have gotten lost. Don't spin. Just
// TODO(tjayrush): die right away if it's locked.

// TODO(tjayrush): Re-enable mocked data
// if (mocked) {
//     string_q which = origMode;
//     } else if (origMode == "status") {
//         if (contains(tool_flags, "monitors")) {
//             origMode = "monitors";
//         }
//     }
//     uint64_t nMocked = getGlobalConfig("")->getConfigInt("dev", "n_mocked", 100);
//     string_q path = getPathToRootConfig("mocked/" + origMode + ".json");
//     if (fileExists(path)) {
//         if (origMode == "export") {
//             for (size_t i = 0; i < nMocked; i++) {
//                 LOG_PROGRESS("Extracting", i, nMocked, "\r");
//                 usleep(30000);
//             }
//             CStringArray lines;
//             asciiFileToLines(path, lines);
//             size_t cnt = 0;
//             size_t record = 0;
//             size_t recordSize = lines.size() / nMocked;
//             for (auto line : lines) {
//                 cout << line << endl;
//                 if (!(++cnt % recordSize)) {
//                     LOG_PROGRESS("Displaying", record++, nMocked, "\r");
//                     usleep(10000);
//                 }
//             }
//             return false;
//         } else {
//             cout << asciiFileToString(path);
//             return false;
//         }
//     }
//     tool_flags += " --mocked ";
// }

//-----------------------------------------------------------------------
bool COptions::isEmitter(const address_t& test) const {
    for (auto monitor : allMonitors)
        if (monitor.address == test)
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool COptions::isRelevant(const CLogEntry& log) const {
    string_q str = toLower(log.Format(STR_DISPLAY_LOGENTRY));
    for (auto monitor : allMonitors)
        if (contains(str, monitor.address.substr(2)))
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool fourByteFilter(const string_q& input, const COptions* opt) {
    ASSERT(!opt->freshenOnly);
    if (opt->fourbytes.empty())
        return true;

    for (auto four : opt->fourbytes)
        if (startsWith(input, four))
            return true;

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

    string_q statsFile = getPathToRootConfig("performance_scraper.csv");

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
