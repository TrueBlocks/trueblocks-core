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
    COption("transactions", "", "list<tx_id>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)"),  // NOLINT
    COption("articulate", "a", "", OPT_SWITCH, "articulate the transactions if an ABI is found for the 'to' address"),
    COption("count", "c", "", OPT_SWITCH, "show the number of traces for the transaction only (fast)"),
    COption("skip_ddos", "s", "", OPT_HIDDEN | OPT_TOGGLE, "skip over 2018 ddos during export ('on' by default)"),
    COption("max_traces", "m", "<uint64>", OPT_HIDDEN | OPT_FLAG, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)"),  // NOLINT
    COption("filter", "f", "<string>", OPT_HIDDEN | OPT_FLAG, "Call trace_filter with bang-seperated string fromBlk!toBlk[!fromAddr[!toAddr[!after[!count]]]]"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Retrieve a transaction's traces from the cache or the node."),
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

        } else if (arg == "-c" || arg == "--count") {
            count = true;

        } else if (arg == "-s" || arg == "--skip_ddos") {
            skip_ddos = !skip_ddos;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--max_traces:")) {
            if (!confirmUint("max_traces", max_traces, arg))
                return false;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--filter:")) {
            filter = substitute(substitute(arg, "-f:", ""), "--filter:", "");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseTransList2(this, transList, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (!nodeHasTraces())
        return usage("Tracing is required for this program to work properly. Quitting...");

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
            return usage("filter.fromBlock must be less or equal to filter.toBlock. Quitting...");
        if (f.fromBlock + 100 < f.toBlock)
            return usage("filter.fromBlock must be no more than 100 blocks before filter.toBlock. Quitting...");
        filters.push_back(f);
        manageFields("CTraceAction:balance,init,refundAddress,selfDestructed", false);  // hide
        manageFields("CTraceResult:code,newContract", false);
        manageFields("CTrace:error", false);
        GETRUNTIME_CLASS(CTrace)->sortFieldList();
        GETRUNTIME_CLASS(CTraceAction)->sortFieldList();
        GETRUNTIME_CLASS(CTraceResult)->sortFieldList();
        if (isTestMode() && !isApiMode()) {
            ostringstream os;
            for (auto ff: filters) {
                os << ff << endl;
            }
            LOG_INFO(os.str());
        }
        return true;
    }

    // Data wrangling
    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

    if (isRaw)
        expContext().exportFmt = JSON1;

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CParameter:str_default", false);  // hide
        manageFields("CTransaction:price", false);      // hide
        manageFields("CFunction:outputs", true);        // show
        manageFields("CTransaction:input", true);       // show
        manageFields("CLogEntry:topics", true);         // show
        abi_spec.loadAbiKnown();
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
            format = getGlobalConfig("getTrace")
                         ->getConfigStr("display", "format", format.empty() ? STR_DISPLAY_TRACE : format);
            manageFields("CTransaction:" + cleanFmt(format));
            manageFields("CTrace:" + cleanFmt(format));
            manageFields("CTraceAction:" + substitute(cleanFmt(format), "ACTION::", ""));
            manageFields("CTraceResult:" + substitute(cleanFmt(format), "RESULT::", ""));
            break;
        case YAML1:
        case API1:
        case JSON1:
            format = "";
            break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format);
    if (isNoHeader)
        expContext().fmtMap["header"] = "";
    if (count)
        expContext().fmtMap["format"] = expContext().fmtMap["header"] = "[{HASH}]\t[{TRACESCNT}]";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW);

    // BEG_CODE_INIT
    articulate = false;
    count = false;
    skip_ddos = getGlobalConfig("getTrace")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("getTrace")->getConfigInt("settings", "max_traces", 250);
    filter = "";
    // END_CODE_INIT

    transList.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    first = true;
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`transactions` is one or more space-separated identifiers which may be either a transaction hash, | a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.");  // NOLINT
    notes.push_back("This tool checks for valid input syntax, but does not check that the transaction requested exists.");  // NOLINT
    notes.push_back("If the queried node does not store historical state, the results are undefined.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::extractBlocksFromFilter(blknum_t& b1, const string_q& p1, blknum_t& b2, const string_q& p2) {
    blknum_t latest = getLatestBlock_client();
    COptionsBlockList blocks;

    // if p1 is empty, the user provided nothing so just return
    if (p1.empty())
        return true;
    // parse p1 into b1
    if (!parseBlockList2(this, blocks, p1, latest))
        return usage("Could not parse invalid block " + p1 + ". Quitting...");
    b1 = blocks.numList.size() ? blocks.numList[0] : latest;

    // if p2 is empty, set b2 to b1 and return
    if (p2.empty()) {
        b2 = b1;
        return true;
    }

    if (!parseBlockList2(this, blocks, p2, latest))
        return usage("Could not parse invalid block " + p2 + ". Quitting...");
    b2 = blocks.numList.size() > 1 ? blocks.numList[1] : b1;

    return true;
}
