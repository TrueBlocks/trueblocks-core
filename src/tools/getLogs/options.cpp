/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~!trans_list",    "a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)"),
    COption("-articulate",     "articulate the transactions if an ABI is found for the 'to' address"),
    COption("@fmt:<fmt>",      "export format (one of [none|json|txt|csv|api])"),
    COption("",                "Retrieve a transaction's logs from the local cache or a running node."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            string_q errorMsg;
            if (!wrangleTxId(arg, errorMsg))
                return usage(errorMsg);
            string_q ret = transList.parseTransList(arg);
            if (!ret.empty())
                return usage(ret);
        }
    }

    // Data wrangling
    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

    if (isRaw)
        exportFmt = JSON1;

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CParameter:strDefault", false);  // hide
        manageFields("CTransaction:price", false);  // hide
        manageFields("CFunction:outputs", true);  // show
        manageFields("CTransaction:input", true);  // show
        manageFields("CLogEntry:topics", true);  // show
        abi_spec.loadAbiKnown("all");
    }

    if (api_mode) {
        manageFields("CLogEntry:all", false);
        manageFields("CLogEntry:address,logIndex,type,compressedLog,topics,data", true);
    }

    // Display formatting
    string_q format;
    switch (exportFmt) {
        case NONE1:
        case TXT1:
        case CSV1:
            format = getGlobalConfig()->getConfigStr("display", "format", format.empty() ? STR_DISPLAY : format);
            manageFields("CLogEntry:" + cleanFmt(format, exportFmt));
            break;
        case API1:
        case JSON1:
            format = "";
            break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    transList.Init();
    option1 = false;
    articulate = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);
    Init();
    first = true;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "trans_list", "<transID> [transID...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{trans_list}] is one or more space-separated identifiers which may be either a transaction hash,|"
                "a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.\n";
        ret += "This tool checks for valid input syntax, but does not check that the transaction requested exists.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured "
                    "(see documentation).\n";
        ret += "If the queried node does not store historical state, the results may be undefined.\n";
        return ret;
    }
    return str;
}

//OLD_CODE
#if 0
    COption("@to_file",     "send results to a temporary file and return the filename"),
    COption("@output:<fn>", "send results to file 'fn' and return the filename"),
#endif

//--------------------------------------------------------------------------------
const char* STR_DISPLAY =
"[{BLOCKNUMBER}]\t"
"[{TRANSACTIONINDEX}]\t"
"[{LOGINDEX}]\t"
"[{ADDRESS}]\t"
"[{TOPIC0}]\t"
"[{TOPIC1}]\t"
"[{TOPIC2}]\t"
"[{TOPIC3}]\t"
"[{DATA}]\t"
"[{TYPE}]\t"
"[{COMPRESSEDLOG}]";
