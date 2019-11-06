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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    COption("transactions", "", "list<tx_id>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate the transactions if an ABI is found for the 'to' address"),
    COption("logs", "l", "", OPT_SWITCH, "display the receipt's logs"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve a transaction's receipt from the local cache or a running node."),
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

        } else if (arg == "-l" || arg == "--logs") {
            logs = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (!parseTransList2(this, transList, arg)) {
            return false;

            // END_CODE_AUTO
        }
    }

    // Data wrangling
    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

    if (logs)
        SHOW_FIELD(CReceipt, "logs");

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

    // Display formatting
    string_q format;
    switch (exportFmt) {
    case NONE1:
    case TXT1:
    case CSV1:
        format = getGlobalConfig("getReceipt")->getConfigStr("display", "format", format.empty() ? STR_DISPLAY_RECEIPT : format);
        if (logs)
            format += "\t[{LOGSCNT}]";
        manageFields("CReceipt:" + cleanFmt(format, exportFmt));
        break;
    case API1:
    case JSON1:
        format = "";
        break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (isNoHeader)
        expContext().fmtMap["header"] = "";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    // BEG_CODE_INIT
    articulate = false;
    logs = false;
    // END_CODE_INIT

    transList.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    first = true;
    notes = "`transactions` is one or more space-separated identifiers which may be either a transaction hash,|";
    notes += "a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.\n";
    notes += "This tool checks for valid input syntax, but does not check that the transaction requested exists.\n";
    notes += "This tool retrieves information from the local node or rpcProvider if configured (see documentation).\n";
    notes += "If the queried node does not store historical state, the results may be undefined.\n";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options")
        return substitute(str, "transactions", "<tx_id> [tx_id...]");
    return str;
}
