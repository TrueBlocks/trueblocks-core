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
    COption("blocks", "", "list<blknum>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more block identifiers"),  // NOLINT
    COption("hashes", "e", "", OPT_SWITCH, "display only transaction hashes, default is to display full transaction detail"),  // NOLINT
    COption("uncles", "U", "", OPT_SWITCH, "display uncle blocks (if any) instead of the requested block"),
    COption("trace", "t", "", OPT_SWITCH, "export the traces from the block as opposed to the block data"),
    COption("apps", "a", "", OPT_SWITCH, "display only the list of address appearances in the block"),
    COption("uniq", "u", "", OPT_SWITCH, "display only the list of uniq address appearances in the block"),
    COption("uniq_tx", "n", "", OPT_SWITCH, "display only the list of uniq address appearances in each transaction"),
    COption("logs", "g", "", OPT_HIDDEN | OPT_SWITCH, "display only the logs found in the block(s)"),
    COption("topic", "p", "list<topic>", OPT_HIDDEN | OPT_FLAG, "for the --logs option only, filter logs to show only those with this topic(s)"),  // NOLINT
    COption("emitter", "m", "list<addr>", OPT_HIDDEN | OPT_FLAG, "for the --logs option only, filter logs to show only those logs emitted by the given address(es)"),  // NOLINT
    COption("count", "c", "", OPT_SWITCH, "display the number of the lists of appearances for --apps, --uniq, or --uniq_tx"),  // NOLINT
    COption("cache", "o", "", OPT_SWITCH, "force a write of the block to the cache"),
    COption("list", "l", "<blknum>", OPT_HIDDEN | OPT_FLAG, "summary list of blocks running backwards from latest block minus num"),  // NOLINT
    COption("list_count", "C", "<blknum>", OPT_HIDDEN | OPT_FLAG, "the number of blocks to report for --list option"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve one or more blocks from the chain or local cache."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_FORMAT_COUNT_JSON;
extern const char* STR_FORMAT_COUNT_TXT;
extern const char* STR_FORMAT_COUNT_TXT_VERBOSE;
extern const char* STR_FORMAT_FILTER_JSON;
extern const char* STR_FORMAT_FILTER_TXT;
extern const char* STR_FORMAT_LIST_JSON;
extern const char* STR_FORMAT_LIST;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool apps = false;
    bool uniq = false;
    bool uniq_tx = false;
    CStringArray topic;
    CAddressArray emitter;
    blknum_t list = NOPOS;
    // END_CODE_LOCAL_INIT

    Init();
    blknum_t latest = getBlockProgress(BP_CLIENT).client;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        // do not collapse
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-e" || arg == "--hashes") {
            hashes = true;

        } else if (arg == "-U" || arg == "--uncles") {
            uncles = true;

        } else if (arg == "-t" || arg == "--trace") {
            trace = true;

        } else if (arg == "-a" || arg == "--apps") {
            apps = true;

        } else if (arg == "-u" || arg == "--uniq") {
            uniq = true;

        } else if (arg == "-n" || arg == "--uniq_tx") {
            uniq_tx = true;

        } else if (arg == "-g" || arg == "--logs") {
            logs = true;

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--topic:")) {
            arg = substitute(substitute(arg, "-p:", ""), "--topic:", "");
            if (!parseTopicList2(this, topic, arg))
                return false;
        } else if (arg == "-p" || arg == "--topic") {
            return flag_required("topic");

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--emitter:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--emitter:", "");
            if (!parseAddressList(this, emitter, arg))
                return false;
        } else if (arg == "-m" || arg == "--emitter") {
            return flag_required("emitter");

        } else if (arg == "-c" || arg == "--count") {
            count = true;

        } else if (arg == "-o" || arg == "--cache") {
            cache = true;

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--list:")) {
            if (!confirmBlockNum("list", list, arg, latest))
                return false;
        } else if (arg == "-l" || arg == "--list") {
            return flag_required("list");

        } else if (startsWith(arg, "-C:") || startsWith(arg, "--list_count:")) {
            if (!confirmBlockNum("list_count", list_count, arg, latest))
                return false;
        } else if (arg == "-C" || arg == "--list_count") {
            return flag_required("list_count");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseBlockList2(this, blocks, arg, latest))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("blocks", blocks, blocks.empty());
    LOG_TEST_BOOL("hashes", hashes);
    LOG_TEST_BOOL("uncles", uncles);
    LOG_TEST_BOOL("trace", trace);
    LOG_TEST_BOOL("apps", apps);
    LOG_TEST_BOOL("uniq", uniq);
    LOG_TEST_BOOL("uniq_tx", uniq_tx);
    LOG_TEST_BOOL("logs", logs);
    LOG_TEST_LIST("topic", topic, topic.empty());
    LOG_TEST_LIST("emitter", emitter, emitter.empty());
    LOG_TEST_BOOL("count", count);
    LOG_TEST_BOOL("cache", cache);
    LOG_TEST("list", list, (list == NOPOS));
    LOG_TEST("list_count", list_count, (list_count == 20));
    // END_DEBUG_DISPLAY

    if (Mocked("blocks"))
        return false;

    if (cache)
        etherlib_init(defaultQuitHandler);

    // syntactic sugar so we deal with topics, but the option is called topic
    for (auto t : topic)
        topics.push_back(t);

    // syntactic sugar so we deal with emitters, but the option is called emitter
    for (auto e : emitter)
        emitters.push_back(e);

    listOffset = contains(command, "list") ? list : NOPOS;
    filterType = (uniq_tx ? "uniq_tx" : (uniq ? "uniq" : (apps ? "apps" : "")));

    if (cache && uncles)
        return usage(usageErrs[ERR_NOCACHEUNCLE]);

    if (cache && !filterType.empty())
        return usage(usageErrs[ERR_NOCACHEADDRESS]);

    if (trace && !isTracingNode())
        return usage(usageErrs[ERR_TRACINGREQUIRED]);

    if (trace && !filterType.empty())
        return usage(usageErrs[ERR_NOTRACEADDRESS]);

    if (trace && hashes)
        return usage(usageErrs[ERR_TRACEHASHEXCLUSIVE]);

    if (blocks.empty() && listOffset == NOPOS)
        return usage(usageErrs[ERR_ATLEASTONEBLOCK]);

    if ((!emitters.empty() || !topics.empty()))
        if (!logs)
            return usage(usageErrs[ERR_EMTOPONLYWITHLOG]);

    secsFinal =
        (timestamp_t)getGlobalConfig("getBlocks")->getConfigInt("settings", "secs_when_final", (uint64_t)secsFinal);

    if (hashes) {
        manageFields("CTransaction:all", FLD_HIDE);
        manageFields("CBlock:all", FLD_HIDE);
        manageFields("CBlock:hash,blockNumber,parentHash,timestamp,tx_hashes", FLD_SHOW);
    }

    if (expContext().exportFmt == NONE1)
        expContext().exportFmt = (isApiMode() ? API1 : (filterType.empty() ? JSON1 : TXT1));

    // Display formatting
    if (count) {
        string_q ff = verbose ? STR_FORMAT_COUNT_TXT_VERBOSE : STR_FORMAT_COUNT_TXT;
        if (verbose <= 2)
            ff = substitute(ff, "\t[{TRACE_COUNT}]", "");
        if (filterType.empty()) {
            replace(ff, "\"[{FILTER_TYPE}]\": [{ADDR_COUNT}]", "");
            replace(ff, "\t[{ADDR_COUNT}]", "");
        }
        configureDisplay("", "CBlock", ff);

    } else if (!filterType.empty()) {
        configureDisplay("", "CBlock", STR_FORMAT_FILTER_TXT);
        if (blocks.hasZeroBlock) {
            optionOn(OPT_PREFUND);
            loadNames();
        }

    } else if (listOffset != NOPOS) {
        configureDisplay("", "CBlock", STR_FORMAT_LIST);

    } else if (trace) {
        configureDisplay("getBlocks", "CTrace", STR_DISPLAY_TRACE);

    } else if (logs) {
        configureDisplay("getBlocks", "CLogEntry", STR_DISPLAY_LOGENTRY);

    } else {
        configureDisplay("getBlocks", "CBlock", STR_DISPLAY_BLOCK);
    }

    if (expContext().exportFmt == API1 || expContext().exportFmt == JSON1) {
        if (count) {
            string_q ff = STR_FORMAT_COUNT_JSON;
            if (filterType.empty()) {
                replace(ff, ",\n \"[{FILTER_TYPE}]\": [{ADDR_COUNT}]", "");
                replace(ff, "\t[{ADDR_COUNT}]", "");
            }
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(ff);

        } else if (listOffset != NOPOS) {
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(STR_FORMAT_LIST_JSON);

        } else if (!filterType.empty()) {
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(STR_FORMAT_FILTER_JSON);
        }
    }
    if (noHeader)
        expContext().fmtMap["header"] = "";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, OPT_RAW);

    // BEG_CODE_INIT
    hashes = false;
    uncles = false;
    trace = false;
    logs = false;
    count = false;
    cache = false;
    list_count = 20;
    // END_CODE_INIT

    filterType = "";
    secsFinal = (60 * 5);
    addrCounter = 0;
    listOffset = NOPOS;
    blocks.Init();
    topics.clear();
    emitters.clear();
    CBlockOptions::Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    expContext().exportFmt = NONE1;

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("`blocks` may be specified as either numbers or hashes.");
    notes.push_back("`special` blocks are detailed under `chifra when --list`.");
    notes.push_back("For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.");  // NOLINT
    notes.push_back("The --logs option is significantly faster if you provide an --emitter or a --topic.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    usageErrs[ERR_NOCACHEUNCLE] = "The --cache option is not available for uncle blocks.";
    usageErrs[ERR_NOCACHEADDRESS] = "The --cache option is not available when using one of the address options.";
    usageErrs[ERR_TRACINGREQUIRED] = "A tracing node is required for the --trace options to work properly.";
    usageErrs[ERR_NOTRACEADDRESS] = "The --trace option is not available when using one of the address options.";
    usageErrs[ERR_TRACEHASHEXCLUSIVE] = "The --hashes and --trace options are exclusive.";
    usageErrs[ERR_ATLEASTONEBLOCK] = "You must specify at least one block.";
    usageErrs[ERR_EMTOPONLYWITHLOG] = "The --emitter and --topic options are only available with the --log option.";
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::isMulti(void) const {
    return isApiMode() || ((blocks.stop - blocks.start) > 1 || blocks.hashList.size() > 1 || blocks.numList.size() > 1);
}

//--------------------------------------------------------------------------------
void interumReport(ostream& os, blknum_t i) {
    os << (!(i % 150) ? "." : (!(i % 1000)) ? "+" : "");  // dots '.' at every 150, '+' at every 1000
    os.flush();
}

//--------------------------------------------------------------------------------
const char* STR_FORMAT_COUNT_JSON =
    "{\n"
    " \"blockNumber\": [{BLOCKNUMBER}],\n"
    " \"transactionsCnt\": [{TRANSACTIONSCNT}],\n"
    " \"[{FILTER_TYPE}]\": [{ADDR_COUNT}]"
    "}\n";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_COUNT_TXT = "[{BLOCKNUMBER}]\t[{TRANSACTIONSCNT}]\t[{ADDR_COUNT}]";
const char* STR_FORMAT_COUNT_TXT_VERBOSE =
    "[{BLOCKNUMBER}]\t[{UNCLE_COUNT}]\t[{TRANSACTIONSCNT}]\t[{TRACE_COUNT}]\t[{ADDR_COUNT}]";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_FILTER_JSON =
    "{\n"
    " \"bn\": \"[{BN}]\",\n"
    " \"tx\": \"[{TX}]\",\n"
    " \"tc\": \"[{TC}]\",\n"
    " \"addr\": \"[{ADDR}]\",\n"
    " \"reason\": \"[{REASON}]\"\n"
    "}\n";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_FILTER_TXT = "[{BN}]\t[{TX}]\t[{TC}]\t[{ADDR}]\t[{REASON}]";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_LIST_JSON =
    "{\n"
    " \"hash\": \"[{HASH}]\",\n"
    " \"blockNumber\": [{BLOCKNUMBER}],\n"
    " \"timestamp\": [{TIMESTAMP}],\n"
    " \"difficulty\": \"[{DIFFICULTY}]\",\n"
    " \"miner\": \"[{MINER}]\",\n"
    " \"transactionsCnt\": [{TRANSACTIONSCNT}],\n"
    " \"unclesCnt\": [{UNCLE_COUNT}],\n"
    " \"gasLimit\": [{GASLIMIT}],\n"
    " \"gasUsed\": [{GASUSED}]\n"
    "}\n";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_LIST =
    "[{HASH}]\t"
    "[{BLOCKNUMBER}]\t"
    "[{TIMESTAMP}]\t"
    "[{DIFFICULTY}]\t"
    "[{MINER}]\t"
    "[{TRANSACTIONSCNT}]\t"
    "[{UNCLE_COUNT}]\t"
    "[{GASLIMIT}]\t"
    "[{GASUSED}]";
