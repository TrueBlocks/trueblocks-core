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
    COption("uniq", "u", "", OPT_SWITCH, "display a list of uniq addresses found in the transaction instead of the underlying data"),  // NOLINT
    COption("reconcile", "r", "<address>", OPT_FLAG, "reconcile the transaction as per the provided address"),
    COption("cache", "o", "", OPT_SWITCH, "force the results of the query into the tx cache (and the trace cache if applicable)"),  // NOLINT
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

        } else if (startsWith(arg, "-r:") || startsWith(arg, "--reconcile:")) {
            reconcile = substitute(substitute(arg, "-r:", ""), "--reconcile:", "");
            if (!isAddress(reconcile))
                return usage("The provided value (" + reconcile + ") is not a properly formatted address.");
        } else if (arg == "-r" || arg == "--reconcile") {
            return flag_required("reconcile");

        } else if (arg == "-o" || arg == "--cache") {
            cache = true;

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

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("transList", transList, transList.empty());
    LOG_TEST_BOOL("articulate", articulate);
    LOG_TEST_BOOL("trace", trace);
    LOG_TEST_BOOL("uniq", uniq);
    LOG_TEST("reconcile", reconcile, (reconcile == ""));
    LOG_TEST_BOOL("cache", cache);
    // END_DEBUG_DISPLAY

    if (Mocked("transactions"))
        return false;

    bool isReconcile = !reconcile.empty();
    if (isReconcile && (cache || trace || articulate))
        return usage("Do not use other options with the --reconcile option.");

    // Data wrangling
    if (transList.empty())
        return usage(usageErrs[ERR_PROVIDEONETXID]);

    if (isRaw)
        expContext().exportFmt = JSON1;

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CParameter:str_default", false);  // hide
        manageFields("CTransaction:price", false);      // hide
        if (!useDict())
            manageFields("CFunction:outputs", true);                                               // show
        manageFields("CTransaction:input", true);                                                  // show
        manageFields("CLogEntry:data,topics", true);                                               // show
        manageFields("CTrace: blockHash, blockNumber, transactionHash, transactionIndex", false);  // hide
        abi_spec.loadAbisFromKnown();
    }

    // order matters
    if (trace) {
        SHOW_FIELD(CTransaction, "traces");
    } else {
        HIDE_FIELD(CTransaction, "traces");
    }

    // Display formatting
    if (uniq) {
        configureDisplay("getTrans", "CAppearance", STR_DISPLAY_APPEARANCE);
    } else if (!reconcile.empty()) {
        string_q fmt = STR_DISPLAY_RECONCILIATION;
        configureDisplay("getTrans", "CReconciliation", fmt);
    } else {
        string_q fmt = STR_DISPLAY_TRANSACTION + string_q(trace ? "\t[{TRACESCNT}]" : "");
        configureDisplay("getTrans", "CTransaction", fmt);
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
    reconcile = "";
    cache = false;
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
    usageErrs[ERR_PROVIDEONETXID] = "Please specify at least one transaction identifier.";
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
