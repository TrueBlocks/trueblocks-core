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
    COption("uncles", "c", "", OPT_SWITCH, "display uncle blocks (if any) instead of the requested block"),
    COption("traces", "t", "", OPT_SWITCH, "export the traces from the block as opposed to the block data"),
    COption("uniq", "u", "", OPT_SWITCH, "display a list of uniq address appearances per transaction"),
    COption("flow", "f", "enum[from|to|reward]", OPT_FLAG, "for the uniq option only, export only from or to (including trace from or to)"),  // NOLINT
    COption("logs", "l", "", OPT_SWITCH, "display only the logs found in the block(s)"),
    COption("emitter", "m", "list<addr>", OPT_FLAG, "for the --logs option only, filter logs to show only those logs emitted by the given address(es)"),  // NOLINT
    COption("topic", "B", "list<topic>", OPT_FLAG, "for the --logs option only, filter logs to show only those with this topic(s)"),  // NOLINT
    COption("articulate", "a", "", OPT_SWITCH, "for the --logs option only, articulate the retrieved data if ABIs can be found"),  // NOLINT
    COption("big_range", "r", "<uint64>", OPT_FLAG, "for the --logs option only, allow for block ranges larger than 500"),  // NOLINT
    COption("count", "U", "", OPT_SWITCH, "display the number of the lists of appearances for --addrs or --uniq"),
    COption("cache", "o", "", OPT_SWITCH, "force a write of the block to the cache"),
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
extern const char* STR_FMT_BLOCKLOGS;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool uniq = false;
    CAddressArray emitter;
    CStringArray topic;
    // END_CODE_LOCAL_INIT

    Init();
    blknum_t latest = getLatestBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        // do not collapse
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-e" || arg == "--hashes") {
            hashes = true;

        } else if (arg == "-c" || arg == "--uncles") {
            uncles = true;

        } else if (arg == "-t" || arg == "--traces") {
            traces = true;

        } else if (arg == "-u" || arg == "--uniq") {
            uniq = true;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--flow:")) {
            if (!confirmEnum("flow", flow, arg))
                return false;
        } else if (arg == "-f" || arg == "--flow") {
            return flag_required("flow");

        } else if (arg == "-l" || arg == "--logs") {
            logs = true;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--emitter:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--emitter:", "");
            if (!parseAddressList(this, emitter, arg))
                return false;
        } else if (arg == "-m" || arg == "--emitter") {
            return flag_required("emitter");

        } else if (startsWith(arg, "-B:") || startsWith(arg, "--topic:")) {
            arg = substitute(substitute(arg, "-B:", ""), "--topic:", "");
            if (!parseTopicList2(this, topic, arg))
                return false;
        } else if (arg == "-B" || arg == "--topic") {
            return flag_required("topic");

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (startsWith(arg, "-r:") || startsWith(arg, "--big_range:")) {
            if (!confirmUint("big_range", big_range, arg))
                return false;
        } else if (arg == "-r" || arg == "--big_range") {
            return flag_required("big_range");

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (arg == "-o" || arg == "--cache") {
            cache = true;

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

    if (!cache && !articulate && !uncles && !traces && !logs && !uniq) {
        return usage("Nope");
    }

    if (cache)
        etherlib_init(defaultQuitHandler);

    // syntactic sugar so we deal with topics, but the option is called topic
    for (auto t : topic)
        logFilter.topics.push_back(t);

    // syntactic sugar so we deal with emitters, but the option is called emitter
    for (auto e : emitter)
        logFilter.emitters.push_back(e);

    filterType = (uniq ? "uniq" : "");

    big_range = max(big_range, uint64_t(50));

    if (traces && !isTracingNode())
        return usage(usageErrs[ERR_TRACINGREQUIRED]);

    if (hashes) {
        manageFields("CTransaction:all", FLD_HIDE);
        manageFields("CBlock:all", FLD_HIDE);
        manageFields("CBlock:hash,blockNumber,parentHash,timestamp,tx_hashes", FLD_SHOW);
    }

    if (expContext().exportFmt == NONE1)
        expContext().exportFmt = (isApiMode() ? JSON1 : (filterType.empty() ? JSON1 : TXT1));

    // Display formatting
    if (count) {
        string_q ff = verbose ? STR_FORMAT_COUNT_TXT_VERBOSE : STR_FORMAT_COUNT_TXT;
        if (verbose <= 2) {
            ff = substitute(ff, "\t[{TRACE_COUNT}]", "");
        }
        if (filterType.empty()) {
            replace(ff, "\"[{FILTER_TYPE}]\": [{ADDR_COUNT}]", "");
            replace(ff, "\t[{ADDR_COUNT}]", "");
        }
        if (!uncles) {
            replace(ff, "\t[{UNCLESCNT}]", "");
        }
        configureDisplay("", "CBlock", ff);

    } else if (!filterType.empty()) {
        configureDisplay("", "CBlock", STR_FORMAT_FILTER_TXT);
        if (blocks.hasZeroBlock)
            loadPrefundBalances();

    } else if (traces) {
        configureDisplay("getBlocks", "CTrace", STR_DISPLAY_TRACE);

    } else if (logs) {
        configureDisplay("getBlocks", "CLog", STR_DISPLAY_LOG);
        manageFields("CLog:topic0,topic1,topic2,topic3", FLD_HIDE);
        manageFields(
            "CLog:blocknumber,blockhash,transactionindex,transactionhash,timestamp,"
            "logindex,address,data,compressedlog",
            FLD_SHOW);
        bool isText = expContext().exportFmt == TXT1 || expContext().exportFmt == CSV1;
        if (isText) {
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(STR_FMT_BLOCKLOGS);
        }

    } else {
        configureDisplay("getBlocks", "CBlock", STR_DISPLAY_BLOCK);
    }

    if (expContext().exportFmt == JSON1) {
        if (count) {
            string_q ff = STR_FORMAT_COUNT_JSON;
            if (filterType.empty()) {
                replace(ff, ",\n \"[{FILTER_TYPE}]\": [{ADDR_COUNT}]", "");
                replace(ff, "\t[{ADDR_COUNT}]", "");
            }
            if (!uncles) {
                replace(ff, "\n \"unclesCnt\": [{UNCLESCNT}]", "");
            }
            replaceAll(ff, ",,", ",");
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(ff);

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
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_RAW);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    hashes = false;
    uncles = false;
    traces = false;
    flow = "";
    logs = false;
    articulate = false;
    big_range = 500;
    count = false;
    cache = false;
    // END_CODE_INIT

    filterType = "";
    secsFinal = (60 * 5);  // Used to be configurable, but no longer
    addrCounter = 0;
    blocks.Init();
    logFilter = CLogFilter();
    CBlockOptions::Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    expContext().exportFmt = NONE1;

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`Blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("`Blocks` may be specified as either numbers or hashes.");
    notes.push_back("`Special` blocks are detailed under `chifra when --list`.");
    notes.push_back("With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.");  // NOLINT
    notes.push_back("The --logs option is significantly faster if you provide an --emitter and/or a --topic.");
    notes.push_back("Multiple topics match on topic0, topic1, and so on, not on different topic0's.");
    notes.push_back("For the --logs option, large block ranges may crash the node, use --big_range to specify a larger range.");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS

    usageErrs[ERR_NOCACHEUNCLE] = "The --cache option is not available for uncle blocks.";
    usageErrs[ERR_NOCACHEADDRESS] = "The --cache option is not available when using one of the address options.";
    usageErrs[ERR_TRACINGREQUIRED] = "A tracing node is required for the --traces options to work properly.";
    usageErrs[ERR_NOTRACEADDRESS] = "The --traces option is not available when using one of the address options.";
    usageErrs[ERR_TRACEHASHEXCLUSIVE] = "The --hashes and --traces options are exclusive.";
    usageErrs[ERR_ATLEASTONEBLOCK] = "You must specify at least one block.";
    usageErrs[ERR_EMTOPONLYWITHLOG] = "The --emitter and --topic options are only available with the --log option.";
    usageErrs[ERR_ARTWITHOUTLOGS] = "The --artcilate option is only available with the --logs option.";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
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
    " \"unclesCnt\": [{UNCLESCNT}],\n"
    " \"[{FILTER_TYPE}]\": [{ADDR_COUNT}]"
    "}\n";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_COUNT_TXT = "[{BLOCKNUMBER}]\t[{TRANSACTIONSCNT}]\t[{UNCLESCNT}]\t[{ADDR_COUNT}]";
const char* STR_FORMAT_COUNT_TXT_VERBOSE =
    "[{BLOCKNUMBER}]\t[{TRANSACTIONSCNT}]\t[{UNCLESCNT}]\t[{TRACE_COUNT}]\t[{ADDR_COUNT}]";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_FILTER_JSON =
    "{\n"
    " \"bn\": \"[{BLOCKNUMBER}]\",\n"
    " \"tx\": \"[{TRANSACTIONINDEX}]\",\n"
    " \"tc\": \"[{TRACEINDEX}]\",\n"
    " \"addr\": \"[{ADDRESS}]\",\n"
    " \"reason\": \"[{REASON}]\"\n"
    "}\n";

//--------------------------------------------------------------------------------
const char* STR_FORMAT_FILTER_TXT = "[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t[{TRACEINDEX}]\t[{ADDRESS}]\t[{REASON}]";

//--------------------------------------------------------------------------------
const char* STR_FMT_BLOCKLOGS =
    "[{BLOCKNUMBER}]\t[{BLOCKHASH}]\t[{TRANSACTIONINDEX}]\t[{TRANSACTIONHASH}]\t"
    "[{TIMESTAMP}]\t[{LOGINDEX}]\t[{ADDRESS}]\t[{TOPIC0}]\t[{TOPIC1}]\t[{TOPIC2}]\t"
    "[{TOPIC3}]\t[{DATA}]\t[{COMPRESSEDLOG}]";
