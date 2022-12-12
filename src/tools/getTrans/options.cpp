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
    COption("trace", "t", "", OPT_SWITCH, "include the transaction's traces in the results"),
    COption("uniq", "u", "", OPT_SWITCH, "display a list of uniq addresses found in the transaction"),
    COption("flow", "f", "enum[from|to]", OPT_FLAG, "for the uniq option only, export only from or to (including trace from or to)"),  // NOLINT
    COption("account_for", "A", "<address>", OPT_FLAG, "reconcile the transaction as per the provided address"),
    COption("cache", "o", "", OPT_SWITCH, "force the results of the query into the tx cache (and the trace cache if applicable)"),  // NOLINT
    COption("source", "s", "", OPT_HIDDEN | OPT_SWITCH, "find the source of the funds sent to the receiver"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve one or more transactions from the chain or local cache."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    address_t account_for = "";
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-t" || arg == "--trace") {
            trace = true;

        } else if (arg == "-u" || arg == "--uniq") {
            uniq = true;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--flow:")) {
            if (!confirmEnum("flow", flow, arg))
                return false;
        } else if (arg == "-f" || arg == "--flow") {
            return flag_required("flow");

        } else if (arg == "-r" || arg == "--reconcile") {
            // clang-format off
            return usage("the --reconcile option is deprecated, please use statements option instead");  // NOLINT
            // clang-format on

        } else if (startsWith(arg, "-A:") || startsWith(arg, "--account_for:")) {
            account_for = substitute(substitute(arg, "-A:", ""), "--account_for:", "");
            if (!isAddress(account_for))
                return usage("The provided value (" + account_for + ") is not a properly formatted address.");
        } else if (arg == "-A" || arg == "--account_for") {
            return flag_required("account_for");

        } else if (arg == "-o" || arg == "--cache") {
            cache = true;

        } else if (arg == "-s" || arg == "--source") {
            source = true;

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

    if (isRaw) {
        expContext().exportFmt = JSON1;
    }

    if (trace && !isTracingNode()) {
        return usage("Tracing is required for this program to work properly.");
    }

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CReconciliation:encoding,signature", true);
        manageFields("CParameter:strDefault", false);                                              // hide
        manageFields("CTransaction:price", false);                                                 // hide
        manageFields("CTransaction:input", true);                                                  // show
        manageFields("CLogEntry:data,topics", true);                                               // show
        manageFields("CTrace: blockHash, blockNumber, transactionHash, transactionIndex", false);  // hide
        abi_spec.loadAbisFromKnown();
    } else {
        manageFields("CReconciliation:encoding,signature", false);
    }

    // order matters
    if (trace) {
        SHOW_FIELD(CTransaction, "traces");
    } else {
        HIDE_FIELD(CTransaction, "traces");
    }

    if (!account_for.empty()) {
        if (!loadNames())
            return usage("Could not load names database.");
        ledgerManager.accountedFor = account_for;
    }

    // Display formatting
    if (uniq) {
        configureDisplay("getTrans", "CAppearance", STR_DISPLAY_APPEARANCE);
    } else if (!ledgerManager.accountedFor.empty()) {
        string_q fmt = STR_DISPLAY_RECONCILIATION;
        if (!articulate) {
            fmt = substitute(fmt, "[{ENCODING}]\t[{SIGNATURE}]\t", "");
        }
        configureDisplay("getTrans", "CReconciliation", fmt);
    } else {
        string_q fmt = STR_DISPLAY_TRANSACTION + string_q(trace ? "\t[{TRACESCNT}]" : "");
        configureDisplay("getTrans", "CTransaction", fmt);
    }

    if (source) {
        LOG_WARN("The --source option is unsupported. Use at your own risk.");
        if (!fileExists("./whiteList.csv")) {
            return usage("./whiteList.csv file is required for this option.");
        }
        expContext().exportFmt = CSV1;
        expContext().fmtMap["header"] = "";
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_RAW);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    articulate = false;
    trace = false;
    uniq = false;
    flow = "";
    cache = false;
    source = false;
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
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
