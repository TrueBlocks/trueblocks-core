/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("blocks", "", "list<blknum>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more blocks to retrieve"),  // NOLINT
    COption("hashes_only", "e", "", OPT_SWITCH, "display only transaction hashes, default is to display full transaction detail"),  // NOLINT
    COption("addrs", "a", "", OPT_SWITCH, "display all addresses included in the block"),
    COption("uniq", "u", "", OPT_SWITCH, "display only uniq addresses found per block"),
    COption("uniq_tx", "n", "", OPT_SWITCH, "display only uniq addresses found per transaction"),
    COption("count_only", "c", "", OPT_SWITCH, "display counts of appearances (for --addrs, --uniq, or --uniq_tx only)"),  // NOLINT
    COption("uncles", "U", "", OPT_SWITCH, "display uncle blocks (if any) instead of the requested block"),
    COption("force", "o", "", OPT_HIDDEN | OPT_SWITCH, "force a re-write of the block to the cache"),
    COption("", "", "", OPT_DESCRIPTION, "Returns block(s) from local cache or directly from a running node."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_FORMAT_COUNT_JSON;
extern const char* STR_FORMAT_COUNT_TXT;
extern const char* STR_FORMAT_COUNT_TXT_VERBOSE;
extern const char* STR_FORMAT_FILTER_JSON;
extern const char* STR_FORMAT_FILTER_TXT;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool addrs = false;
    bool uniq = false;
    bool uniq_tx = false;
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
        } else if (arg == "-e" || arg == "--hashes_only") {
            hashes_only = true;

        } else if (arg == "-a" || arg == "--addrs") {
            addrs = true;

        } else if (arg == "-u" || arg == "--uniq") {
            uniq = true;

        } else if (arg == "-n" || arg == "--uniq_tx") {
            uniq_tx = true;

        } else if (arg == "-c" || arg == "--count_only") {
            count_only = true;

        } else if (arg == "-U" || arg == "--uncles") {
            uncles = true;

        } else if (arg == "-o" || arg == "--force") {
            force = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseBlockList2(this, blocks, arg, latest))
                return false;

            // END_CODE_AUTO
        }
    }

    if (force)
        etherlib_init(defaultQuitHandler);

    if (expContext().isParity) {
        HIDE_FIELD(CTransaction, "cumulativeGasUsed");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CTransaction, "timestamp");
        RENAME_FIELD(CBlock, "blockNumber", "number");
        GETRUNTIME_CLASS(CBlock)->sortFieldList();
    }

    if (hashes_only) {
        HIDE_FIELD(CTransaction, "blockHash");
        HIDE_FIELD(CTransaction, "blockNumber");
        HIDE_FIELD(CTransaction, "transactionIndex");
        HIDE_FIELD(CTransaction, "nonce");
        HIDE_FIELD(CTransaction, "timestamp");
        HIDE_FIELD(CTransaction, "from");
        HIDE_FIELD(CTransaction, "to");
        HIDE_FIELD(CTransaction, "value");
        HIDE_FIELD(CTransaction, "gas");
        HIDE_FIELD(CTransaction, "gasPrice");
        HIDE_FIELD(CTransaction, "input");
        HIDE_FIELD(CTransaction, "isError");
        HIDE_FIELD(CTransaction, "receipt");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CReceipt, "contractAddress");
        HIDE_FIELD(CReceipt, "gasUsed");
        HIDE_FIELD(CReceipt, "logs");
        HIDE_FIELD(CReceipt, "status");
        HIDE_FIELD(CBlock, "gasLimit");
        HIDE_FIELD(CBlock, "gasUsed");
        HIDE_FIELD(CBlock, "miner");
        HIDE_FIELD(CBlock, "difficulty");
        HIDE_FIELD(CBlock, "price");
        HIDE_FIELD(CBlock, "finalized");
    }

    filterType = (uniq_tx ? "uniq_tx" : (uniq ? "uniq" : (addrs ? "addrs" : "")));
    if (filterType.empty() && count_only)
        return usage("--count_only option is only available with either --addrs, --uniq, or --uniq_tx. Quitting...");

    if (!filterType.empty() && force)
        return usage("The --force option is not available when using one of the address options. Quitting...");

    if (uncles && force)
        return usage("The --force option is not available for uncles. Quitting...");

    if (!blocks.hasBlocks())
        return usage("You must specify at least one block. Quitting...");

    secsFinal =
        (timestamp_t)getGlobalConfig("getBlock")->getConfigInt("settings", "secs_when_final", (uint64_t)secsFinal);

    if (expContext().exportFmt == NONE1)
        expContext().exportFmt = (isApiMode() ? API1 : (filterType.empty() ? JSON1 : TXT1));

    // Display formatting
    if (count_only) {
        string_q ff = verbose ? STR_FORMAT_COUNT_TXT_VERBOSE : STR_FORMAT_COUNT_TXT;
        if (verbose <= 2)
            ff = substitute(ff, "\t[{TRACE_COUNT}]", "");
        configureDisplay("", "CBlock", ff);
    } else if (!filterType.empty()) {
        configureDisplay("", "CBlock", STR_FORMAT_FILTER_TXT);
    } else {
        configureDisplay("getBlock", "CBlock", STR_DISPLAY_BLOCK);
    }

    if (expContext().exportFmt == API1 || expContext().exportFmt == JSON1) {
        if (count_only)
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(STR_FORMAT_COUNT_JSON);
        else if (!filterType.empty())
            expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(STR_FORMAT_FILTER_JSON);
    }
    if (isNoHeader)
        expContext().fmtMap["header"] = "";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    // BEG_CODE_INIT
    hashes_only = false;
    count_only = false;
    uncles = false;
    force = false;
    // END_CODE_INIT

    filterType = "";
    secsFinal = (60 * 5);
    addrCounter = 0;
    blocks.Init();
    CBlockOptions::Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    first = true;
    expContext().exportFmt = NONE1;
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("`special` blocks are detailed under `whenBlock --list`.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
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
