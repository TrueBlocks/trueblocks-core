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
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) to export"),
    COption("accounting", "C", "", OPT_SWITCH, "attach accounting records to the exported data (applies to transactions export only)"),  // NOLINT
    COption("statements", "A", "", OPT_SWITCH, "for the accounting options only, export only statements"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("cache", "o", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("cache_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("first_record", "c", "<uint64>", OPT_FLAG, "the first record to process"),
    COption("max_records", "e", "<uint64>", OPT_FLAG, "the maximum number of records to process"),
    COption("asset", "P", "list<addr>", OPT_FLAG, "for the accounting options only, export statements only for this asset"),  // NOLINT
    COption("flow", "f", "enum[in|out|zero]", OPT_FLAG, "for the accounting options only, export statements with incoming, outgoing, or zero value"),  // NOLINT
    COption("first_block", "F", "<blknum>", OPT_FLAG, "first block to process (inclusive)"),
    COption("last_block", "L", "<blknum>", OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more addresses."),
    // clang-format on
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    CAddressArray addrs;
    CAddressArray asset;
    blknum_t first_block = 0;
    blknum_t last_block = NOPOS;

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
        } else if (arg == "-C" || arg == "--accounting") {
            accounting = true;

        } else if (arg == "-A" || arg == "--statements") {
            statements = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-o" || arg == "--cache") {
            cache = true;

        } else if (arg == "-R" || arg == "--cache_traces") {
            cache_traces = true;

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

        } else if (startsWith(arg, "-P:") || startsWith(arg, "--asset:")) {
            arg = substitute(substitute(arg, "-P:", ""), "--asset:", "");
            if (!parseAddressList(this, asset, arg))
                return false;
        } else if (arg == "-P" || arg == "--asset") {
            return flag_required("asset");

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--flow:")) {
            if (!confirmEnum("flow", flow, arg))
                return false;
        } else if (arg == "-f" || arg == "--flow") {
            return flag_required("flow");

        } else if (startsWith(arg, "-O:") || startsWith(arg, "--load:")) {
            load = substitute(substitute(arg, "-O:", ""), "--load:", "");
        } else if (arg == "-O" || arg == "--load") {
            return flag_required("load");

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

        } else {
            if (!parseAddressList(this, addrs, arg))
                return false;
        }
    }

    if (first_record == 0) {
        first_record = 1;
    }

    if (!isApiMode() && (max_records == 250 || max_records == 0))
        max_records = (((size_t)-100000000));  // this is a very large number that won't wrap

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

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, OPT_PREFUND);

    accounting = false;
    statements = false;
    articulate = false;
    // clang-format off
    cache = getGlobalConfig("acctExport")->getConfigBool("settings", "cache", false);
    cache_traces = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_traces", false);
    // clang-format on
    first_record = 1;
    max_records = 250;
    flow = "";

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

    // clang-format off
    notes.push_back("An `address` must be either an ENS name or start with '0x' and be forty-two characters long.");
    notes.push_back("Articulating the export means turn the EVM's byte data into human-readable text (if possible).");
    notes.push_back("If provided, --max_records dominates, also, if provided, --first_record overrides --first_block.");
    notes.push_back("The --first_record and --max_record options are zero-based (as are the block options).");
    notes.push_back("The _block and _record options are ignored when used with the --count option.");
    // clang-format on

    // clang-format off
    configs.push_back("`cache`: write transactions to the cache (see notes).");
    configs.push_back("`cache_traces`: write traces to the cache (see notes).");
    // clang-format on
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::setDisplayFormatting(void) {
    bool isText = expContext().exportFmt != JSON1;
    string_q hide = substitute(defHide, "|CLog: data, topics", "");
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

        format = getGlobalConfig("acctExport")->getConfigStr("display", "statement", STR_DISPLAY_RECONCILIATION);
        expContext().fmtMap["reconciliation_fmt"] = cleanFmt(format);
        manageFields("CReconciliation:" + format);
    } else {
        HIDE_FIELD(CAppearance, "traceIndex");
        HIDE_FIELD(CAppearance, "reason");
    }
    HIDE_FIELD(CFunction, "stateMutability");
    HIDE_FIELD(CParameter, "strDefault");
    HIDE_FIELD(CParameter, "components");
    HIDE_FIELD(CParameter, "internalType");
    SHOW_FIELD(CTransaction, "traces");

    expContext().fmtMap["header"] = "";
    if (!noHeader) {
        if (statements) {
            expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["reconciliation_fmt"]);
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

    articulate = (articulate && (!isTestMode() || getEnvStr("TEST_NO_ARTICULATION") != "true"));

    // TODO(tjayrush): This doesn't work for some reason (see test case acctExport_export_logs.txt)
    if (!articulate)
        HIDE_FIELD(CLog, "compressedTx");

    return true;
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
