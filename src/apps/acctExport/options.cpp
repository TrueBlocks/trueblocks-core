/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
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
    COption("receipts", "r", "", OPT_SWITCH, "export receipts instead of transaction list"),
    COption("logs", "l", "", OPT_SWITCH, "export logs instead of transaction list"),
    COption("traces", "t", "", OPT_SWITCH, "export traces instead of transaction list"),
    COption("accounting", "C", "", OPT_SWITCH, "export accounting records instead of transaction list"),
    COption("tokens", "O", "", OPT_SWITCH, "export accounting for ERC 20 tokens (assumes ETH accounting as above)"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("cache_txs", "i", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("cache_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("skip_ddos", "d", "", OPT_HIDDEN | OPT_TOGGLE, "toggle skipping over 2016 dDos transactions ('on' by default)"),  // NOLINT
    COption("max_traces", "m", "<uint64>", OPT_HIDDEN | OPT_FLAG, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)"),  // NOLINT
    COption("freshen", "f", "", OPT_HIDDEN | OPT_SWITCH, "freshen but do not print the exported data"),
    COption("factory", "y", "", OPT_SWITCH, "scan for contract creations from the given address(es) and report address of those contracts"),  // NOLINT
    COption("emitter", "", "", OPT_SWITCH, "for log export only, export only if one of the given export addresses emitted the event"),  // NOLINT
    COption("source", "", "list<addr>", OPT_FLAG, "for log export only, export only one of these addresses emitted the event"),  // NOLINT
    COption("relevant", "", "", OPT_SWITCH, "for log export only, if true export only logs relevant to one of the given export addresses"),  // NOLINT
    COption("count", "U", "", OPT_SWITCH, "only available for --appearances mode, if present return only the number of records"),  // NOLINT
    COption("first_block", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("last_block", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("first_record", "c", "<blknum>", OPT_FLAG, "the first record to process"),
    COption("max_records", "e", "<blknum>", OPT_FLAG, "the maximum number of records to process before reporting"),
    COption("clean", "", "", OPT_SWITCH, "clean (i.e. remove duplicate appearances) from all existing monitors"),
    COption("staging", "s", "", OPT_HIDDEN | OPT_SWITCH, "enable search of staging (not yet finalized) folder"),
    COption("unripe", "u", "", OPT_HIDDEN | OPT_SWITCH, "enable search of unripe (neither staged nor finalized) folder (assumes --staging)"),  // NOLINT
    COption("load", "", "<string>", OPT_HIDDEN | OPT_FLAG, "a comma separated list of dynamic traversers to load"),
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
    bool freshen = false;
    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;
    // END_CODE_LOCAL_INIT

    blknum_t latest = bp.client;
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

        } else if (arg == "-C" || arg == "--accounting") {
            accounting = true;

        } else if (arg == "-O" || arg == "--tokens") {
            tokens = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-i" || arg == "--cache_txs") {
            cache_txs = true;

        } else if (arg == "-R" || arg == "--cache_traces") {
            cache_traces = true;

        } else if (arg == "-d" || arg == "--skip_ddos") {
            skip_ddos = !skip_ddos;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--max_traces:")) {
            if (!confirmUint("max_traces", max_traces, arg))
                return false;
        } else if (arg == "-m" || arg == "--max_traces") {
            return flag_required("max_traces");

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg == "-y" || arg == "--factory") {
            factory = true;

        } else if (arg == "--emitter") {
            emitter = true;

        } else if (startsWith(arg, "--source:")) {
            arg = substitute(substitute(arg, "-:", ""), "--source:", "");
            if (!parseAddressList(this, source, arg))
                return false;
        } else if (arg == "--source") {
            return flag_required("source");

        } else if (arg == "--relevant") {
            relevant = true;

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--first_block:")) {
            if (!confirmBlockNum("first_block", first_block, arg, latest))
                return false;
        } else if (arg == "-S" || arg == "--first_block") {
            return flag_required("first_block");

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--last_block:")) {
            if (!confirmBlockNum("last_block", last_block, arg, latest))
                return false;
        } else if (arg == "-E" || arg == "--last_block") {
            return flag_required("last_block");

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

        } else if (arg == "--clean") {
            clean = true;

        } else if (arg == "-s" || arg == "--staging") {
            staging = true;

        } else if (arg == "-u" || arg == "--unripe") {
            unripe = true;

        } else if (startsWith(arg, "--load:")) {
            load = substitute(substitute(arg, "-:", ""), "--load:", "");
        } else if (arg == "--load") {
            return flag_required("load");

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

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("addrs", addrs, addrs.empty());
    LOG_TEST_LIST("topics", topics, topics.empty());
    LOG_TEST_LIST("fourbytes", fourbytes, fourbytes.empty());
    LOG_TEST_BOOL("appearances", appearances);
    LOG_TEST_BOOL("receipts", receipts);
    LOG_TEST_BOOL("logs", logs);
    LOG_TEST_BOOL("traces", traces);
    LOG_TEST_BOOL("accounting", accounting);
    LOG_TEST_BOOL("tokens", tokens);
    LOG_TEST_BOOL("articulate", articulate);
    LOG_TEST_BOOL("cache_txs", cache_txs);
    LOG_TEST_BOOL("cache_traces", cache_traces);
    LOG_TEST_BOOL("skip_ddos", skip_ddos);
    LOG_TEST("max_traces", max_traces, (max_traces == 250));
    LOG_TEST_BOOL("freshen", freshen);
    LOG_TEST_BOOL("factory", factory);
    LOG_TEST_BOOL("emitter", emitter);
    LOG_TEST_LIST("source", source, source.empty());
    LOG_TEST_BOOL("relevant", relevant);
    LOG_TEST_BOOL("count", count);
    LOG_TEST("first_block", first_block, (first_block == 0));
    LOG_TEST("last_block", last_block, (last_block == NOPOS));
    LOG_TEST("first_record", first_record, (first_record == 0));
    LOG_TEST("max_records", max_records, (max_records == (isApiMode() ? 250 : NOPOS)));
    LOG_TEST_BOOL("clean", clean);
    LOG_TEST_BOOL("staging", staging);
    LOG_TEST_BOOL("unripe", unripe);
    LOG_TEST("load", load, (load == ""));
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    freshenOnly = freshen;

    if (!bloomsAreInitalized())
        return usage("Bloom filters not found. You must run 'chifra init' before running this command.");

    if (clean) {
        if (!process_clean())
            return usage("Clean function returned false.");
        return false;
    }

    // Handle the easy cases first...
    if (isCrudCommand())
        return process_rm(addrs);

    // We need at least one address to scrape...
    if (addrs.size() == 0)
        return usage("You must provide at least one Ethereum address.");

    if (topics.size() && !logs)
        return usage("Use the topics option only with --logs.");

    if (fourbytes.size() && (logs || receipts || appearances))
        return usage("Use the fourbytes option only with non-logs commands.");

    if ((appearances + receipts + logs + traces) > 1)
        return usage("Please export only one of list, receipts, logs, or traces.");

    if (emitter && !logs)
        return usage("The --emitter option is only available when exporting logs.");

    if (!source.empty() && !logs)
        return usage("The --source option is only available when exporting logs.");

    if (factory && !traces)
        return usage("The --factory option is only available when exporting traces.");

    if (count && (receipts || logs || traces || emitter || factory))
        return usage("--count option is only available with --appearances option.");

    if ((accounting) && (addrs.size() != 1))
        return usage("You may only use --accounting option with a single address.");

    if ((accounting) && freshenOnly)
        return usage("Do not use the --accounting option with --freshen.");

    if ((accounting) && (appearances || logs || traces || receipts))
        return usage("Do not use the --accounting option with other options.");

    if ((freshenOnly) && (appearances || logs || traces || receipts))
        return usage("Do not use the --freshen option with other options.");

    // Where will we start?
    blknum_t firstBlockToVisit = NOPOS;

    for (auto addr : addrs) {
        CMonitor monitor;
        monitor.setValueByName("address", toLower(addr));
        monitor.setValueByName("name", toLower(addr));
        monitor.clearMonitorLocks();
        monitor.finishParse();
        monitor.fm_mode = (fileExists(monitor.getMonitorPath(monitor.address)) ? FM_PRODUCTION : FM_STAGING);
        if (monitor.monitorExists()) {
            string_q unused;
            if (monitor.isMonitorLocked(unused))
                LOG_ERR(
                    "The cache file is locked. The program is either already "
                    "running or it did not end cleanly the\n\tlast time it ran. "
                    "Quit the already running program or, if it is not running, "
                    "remove the lock\n\tfile: " +
                    monitor.getMonitorPath(addr) + +".lck'. Proceeding anyway...");
            string_q msg;
            if (monitor.isMonitorLocked(msg))  // If locked, we fail
                return usage(msg);
            firstBlockToVisit = min(firstBlockToVisit, monitor.getLastVisited());
            LOG_TEST("Monitor found for", addr, false);
            LOG_TEST("Last block in monitor", monitor.getLastBlockInMonitor(), false);
        } else {
            LOG_WARN("Monitor not found for ", addr + ". Continuing anyway.   ", false);
            firstBlockToVisit = 0;
        }
        if (accountedFor.empty()) {
            CAccountName acct;
            acct.address = monitor.address;
            getNamedAccount(acct, monitor.address);
            accountedForName = acct.name;
            accountedFor = acct.address;
        }
        allMonitors.push_back(monitor);
    }

    if (appearances || count)
        articulate = false;
    if (articulate) {
        abi_spec.loadAbisFromKnown();
        for (auto monitor : allMonitors) {
            if (isContractAt(monitor.address, bp.client))
                abi_spec.loadAbiFromEtherscan(monitor.address);
        }
    }

    if (!setDisplayFormatting())
        return false;

    // Are we visiting unripe and/or staging in our search?
    if (staging && unripe)
        return usage("Please choose only one of --staging or --unripe.");

    // Last block depends on scrape type or user input `end` option (with appropriate check)
    blknum_t lastBlockToVisit = max((blknum_t)1, unripe ? bp.unripe : staging ? bp.staging : bp.finalized);
    listRange = make_pair((firstBlockToVisit == NOPOS ? 0 : firstBlockToVisit), lastBlockToVisit);

    if (isTestMode() && (staging || unripe))
        return usage("--staging and --unripe are disabled for testing.");

    if (unripe && (cache_txs || cache_traces)) {
        cache_txs = cache_traces = false;
        LOG_INFO("Turning off caching for unripe blocks.");
    }

    if (!process_freshen())
        return usage("freshen returns false.");

    if (first_block > last_block)
        return usage("--first_block must be less than or equal to --last_block.");
    blockRange = make_pair(first_block, last_block);

    if (count) {
        cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CMonitorCount)->m_ClassName);
        for (auto monitor : allMonitors) {
            string_q path = monitor.getMonitorPath(monitor.address);
            CMonitorCount monCount;
            monCount.address = monitor.address;
            monCount.fileSize = fileSize(path);
            monCount.nRecords = fileSize(path) / sizeof(CAppearance_base);
            cout << ((isJson() && !firstOut) ? ", " : "");
            cout << monCount;
            firstOut = false;
            if (shouldQuit())
                break;
        }
        cout << exportPostamble(errors, expContext().fmtMap["meta"]);
        return false;

    } else {
        if (load.empty() && !loadAllAppearances())
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND);
    optionOn(OPT_CRUD);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    // BEG_CODE_INIT
    appearances = false;
    receipts = false;
    logs = false;
    traces = false;
    accounting = false;
    tokens = false;
    articulate = false;
    // clang-format off
    cache_txs = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_txs", false);
    cache_traces = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_traces", false);
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    factory = getGlobalConfig("acctExport")->getConfigBool("settings", "factory", false);
    // clang-format on
    emitter = false;
    source.clear();
    relevant = false;
    count = false;
    first_record = 0;
    max_records = (isApiMode() ? 250 : NOPOS);
    clean = false;
    staging = false;
    unripe = false;
    load = "";
    // END_CODE_INIT

    bp = getBlockProgress(BP_ALL);
    listRange = make_pair(0, NOPOS);

    allMonitors.clear();
    apps.clear();

    accountedFor = "";

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

    // Establish folders. This may be redundant, but it's cheap.
    establishMonitorFolders();
    establishFolder(indexFolder);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_ripe);
    establishFolder(getCachePath("tmp/"));
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    CMonitorCount::registerClass();
    CAppearanceDisplay::registerClass();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("An `address` must start with '0x' and be forty-two characters long.");
    // clang-format on
    // END_CODE_NOTES

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
        string_q show =
            defShow + (isApiMode() ? "|CTransaction:encoding,function,input,etherGasCost|CTrace:traceAddress" : "");
        manageFields(show, true);
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

            format = getGlobalConfig("acctExport")->getConfigStr("display", "trace", STR_DISPLAY_TRACE);
            expContext().fmtMap["trace_fmt"] = cleanFmt(format);
            manageFields("CTrace:" + format);

            // This doesn't really work because CAppearance_base is not a subclass of CBaseNode. We phony it here for
            // future reference.
            format =
                getGlobalConfig("acctExport")->getConfigStr("display", "appearances", STR_DISPLAY_APPEARANCEDISPLAY);
            HIDE_FIELD(CAppearanceDisplay, "tags");
            HIDE_FIELD(CAppearanceDisplay, "symbol");
            HIDE_FIELD(CAppearanceDisplay, "source");
            HIDE_FIELD(CAppearanceDisplay, "decimals");
            HIDE_FIELD(CAppearanceDisplay, "description");
            HIDE_FIELD(CAppearanceDisplay, "is_custom");
            HIDE_FIELD(CAppearanceDisplay, "is_prefund");
            HIDE_FIELD(CAppearanceDisplay, "is_contract");
            HIDE_FIELD(CAppearanceDisplay, "is_erc20");
            HIDE_FIELD(CAppearanceDisplay, "is_erc721");
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
        HIDE_FIELD(CParameter, "str_default");
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
            manageFields("CTransaction:reconciliations", false);
            bool nodeHasBals = nodeHasBalances(false);
            string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
            if (!nodeHasBals) {
                string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);
                if (rpcProvider == balanceProvider || balanceProvider.empty())
                    return usage("--accounting requires historical balances. The RPC server does not have them.");
                setRpcProvider(balanceProvider);
                if (!nodeHasBalances(false))
                    return usage("balanceServer is set, but it does not have historical state.");
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
// TODO(tjayrush): accounting can not be freshen, appearances, logs, receipts, traces, but must be articulate - why?
// TODO(tjayrush): accounting must be exportFmt API1 - why?
// TODO(tjayrush): accounting must be for one monitor address - why?
// TODO(tjayrush): accounting requires node balances - why?
// TODO(tjayrush): Used to do this: if any ABI files was newer, re-read abi and re-articulate in cache
// TODO(tjayrush): What does prefundAddrMap and prefundMap do? Needs testing
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
//     string_q path = configPath("mocked/" + origMode + ".json");
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
bool COptions::wasEmittedBy(const address_t& test) const {
    for (auto e : source)
        if (e == test)
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
string_q CScrapeStatistics::Header(const string_q& fmt) const {
    return Format(substitute(fmt, "{", "{p:"));
}

//-----------------------------------------------------------------------
void COptions::writePerformanceData(void) {
    if (stats.nFiles == stats.nSkipped)
        return;

    string_q statsFile = configPath("performance_scraper.csv");

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
bool fourByteFilter(const string_q& input, const COptions* opt) {
    ASSERT(!opt->freshenOnly);
    if (opt->fourbytes.empty())
        return true;

    for (auto four : opt->fourbytes)
        if (startsWith(input, four))
            return true;

    return false;
}
