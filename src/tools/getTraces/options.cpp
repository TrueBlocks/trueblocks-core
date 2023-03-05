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
    COption("transactions", "", "list<tx_id>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more transaction identifiers"),  // NOLINT
    COption("articulate", "a", "", OPT_SWITCH, "articulate the retrieved data if ABIs can be found"),
    COption("filter", "f", "<string>", OPT_FLAG, "call the node's trace_filter routine with bang-separated filter"),
    COption("count", "U", "", OPT_SWITCH, "show the number of traces for the transaction only (fast)"),
    COption("skip_ddos", "s", "", OPT_HIDDEN | OPT_TOGGLE, "skip over the 2016 ddos during export ('on' by default)"),
    COption("max", "m", "<uint64>", OPT_HIDDEN | OPT_FLAG, "if --skip_ddos is on, this many traces defines what a ddos transaction is"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Retrieve traces for the given transaction(s)."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--filter:")) {
            filter = substitute(substitute(arg, "-f:", ""), "--filter:", "");
        } else if (arg == "-f" || arg == "--filter") {
            return flag_required("filter");

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (arg == "-s" || arg == "--skip_ddos") {
            skip_ddos = !skip_ddos;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--max:")) {
            if (!confirmUint("max", max, arg))
                return false;
        } else if (arg == "-m" || arg == "--max") {
            return flag_required("max");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseTransList2(this, transList, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (!isTracingNode())
        return usage("Tracing is required for this program to work properly.");

    if (!filter.empty()) {
        string_q headerLine = "fromBlock,toBlock,fromAddress,toAddress,after,count";
        CStringArray headers;
        explode(headers, headerLine, ',');
        CTraceFilter f;
        string_q line = substitute(filter, "!", ",");
        f.parseCSV(headers, line);
        // block numbers may be hex, number or special, so handle them now
        CStringArray parts;
        explode(parts, filter, '!');
        if (parts.size() > 1)
            extractBlocksFromFilter(f.fromBlock, parts[0], f.toBlock, parts[1]);
        if (f.fromBlock > f.toBlock)
            return usage("filter.fromBlock must be less or equal to filter.toBlock.");
        if (f.fromBlock + 100 < f.toBlock)
            return usage("filter.fromBlock must be no more than 100 blocks before filter.toBlock.");
        filters.push_back(f);
        manageFields("CTraceAction:balance,init,refundAddress,selfDestructed", false);  // hide
        manageFields("CTraceResult:code,address", false);
        manageFields("CTrace:error", false);
        GETRUNTIME_CLASS(CTrace)->sortFieldList();
        GETRUNTIME_CLASS(CTraceAction)->sortFieldList();
        GETRUNTIME_CLASS(CTraceResult)->sortFieldList();
        if (isTestMode() && !isApiMode()) {
            ostringstream os;
            for (auto ff : filters) {
                os << ff << endl;
            }
            LOG_INFO(os.str());
        }
        return true;
    }

    if ((!articulate && filter.empty()) || count) {
        return usage("Should by ported in getTraces.");
    }

    if (isRaw) {
        return usage("Should not be --raw in getTraces.");
    }

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CParameter:strDefault", false);  // hide
        manageFields("CTransaction:price", false);     // hide
        manageFields("CTransaction:input", true);      // show
        manageFields("CLog:topics", true);             // show
        abi_spec.loadAbisFromKnown();
    }

    // Display formatting
    string_q format;
    switch (expContext().exportFmt) {
        case NONE1:
            format = STR_DISPLAY_TRACE;
            manageFields("CTransaction:" + cleanFmt(format));
            manageFields("CTrace:" + cleanFmt(format));
            manageFields("CTraceAction:" + substitute(cleanFmt(format), "ACTION::", ""));
            manageFields("CTraceResult:" + substitute(cleanFmt(format), "RESULT::", ""));
            break;
        case TXT1:
        case CSV1:
            format = getGlobalConfig("getTraces")->getConfigStr("display", "format", STR_DISPLAY_TRACE);
            manageFields("CTransaction:" + cleanFmt(format));
            manageFields("CTrace:" + cleanFmt(format));
            manageFields("CTraceAction:" + substitute(cleanFmt(format), "ACTION::", ""));
            manageFields("CTraceResult:" + substitute(cleanFmt(format), "RESULT::", ""));
            break;
        case JSON1:
            format = "";
            break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format);

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
    articulate = false;
    filter = "";
    count = false;
    // clang-format off
    skip_ddos = getGlobalConfig("getTraces")->getConfigBool("settings", "skip_ddos", true);
    max = getGlobalConfig("getTraces")->getConfigInt("settings", "max", 250);
    // clang-format on
    // END_CODE_INIT

    transList.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The `transactions` list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.");  // NOLINT
    notes.push_back("This tool checks for valid input syntax, but does not check that the transaction requested actually exists.");  // NOLINT
    notes.push_back("If the queried node does not store historical state, the results for most older transactions are undefined.");  // NOLINT
    notes.push_back("A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // clang-format off
    configs.push_back("`skip_ddos`: skip over the 2016 ddos during export ('on' by default).");
    configs.push_back("`max`: if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250).");  // NOLINT
    // clang-format on

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::extractBlocksFromFilter(blknum_t& b1, const string_q& p1, blknum_t& b2, const string_q& p2) {
    blknum_t latest = getLatestBlock_client();
    COptionsBlockList blocks(this);

    // if p1 is empty, the user provided nothing so just return
    if (p1.empty())
        return true;
    // parse p1 into b1
    if (!parseBlockList2(this, blocks, p1, latest))
        return usage("Could not parse invalid block " + p1 + ".");
    b1 = blocks.numList.size() ? blocks.numList[0] : latest;

    // if p2 is empty, set b2 to b1 and return
    if (p2.empty()) {
        b2 = b1;
        return true;
    }

    if (!parseBlockList2(this, blocks, p2, latest))
        return usage("Could not parse invalid block " + p2 + ".");
    b2 = blocks.numList.size() > 1 ? blocks.numList[1] : b1;

    return true;
}
