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
    COption("topic", "t", "list<topic>", OPT_FLAG, "filter by one or more log topics (not implemented)"),
    COption("source", "s", "list<addr>", OPT_FLAG, "export only if the given address emitted the event (not implemented)"),  // NOLINT
    COption("articulate", "a", "", OPT_SWITCH, "articulate the retrieved data if ABIs can be found"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve logs for the given transaction(s)."),
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
        } else if (startsWith(arg, "-t:") || startsWith(arg, "--topic:")) {
            arg = substitute(substitute(arg, "-t:", ""), "--topic:", "");
            topic.push_back(arg);
        } else if (arg == "-t" || arg == "--topic") {
            return flag_required("topic");

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--source:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--source:", "");
            if (!parseAddressList(this, source, arg))
                return false;
        } else if (arg == "-s" || arg == "--source") {
            return flag_required("source");

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

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
    LOG_TEST_LIST("topic", topic, topic.empty());
    LOG_TEST_LIST("source", source, source.empty());
    LOG_TEST_BOOL("articulate", articulate);
    // END_DEBUG_DISPLAY

    if (Mocked("logs"))
        return false;

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
            manageFields("CFunction:outputs", true);  // show
        manageFields("CTransaction:input", true);     // show
        manageFields("CLogEntry:topics", true);       // show
        abi_spec.loadAbisFromKnown();
    }

    // Not sure why this is here to be honest, perhaps only to make test cases pass. The test cases could be fixed...
    if (isApiMode() || expContext().exportFmt == API1) {
        manageFields("CLogEntry:all", false);
        manageFields("CLogEntry:address,logIndex,type,compressedLog,topics,data", true);
    }
    // Not sure why this is here to be honest, perhaps only to make test cases pass. The test cases could be fixed...
    if (expContext().exportFmt == JSON1) {
        manageFields(defHide, false);
        manageFields(defShow + "|CLogEntry:data,topics", true);
    }

    // Display formatting
    configureDisplay("getLogs", "CLogEntry", STR_DISPLAY_LOGENTRY);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, OPT_RAW);

    // BEG_CODE_INIT
    topic.clear();
    source.clear();
    articulate = false;
    // END_CODE_INIT

    transList.Init();
    option1 = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The `transactions` list may be one or more space-separated identifiers which are either a transaction hash, | a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.");  // NOLINT
    notes.push_back("This tool checks for valid input syntax, but does not check that the transaction requested actually exists.");  // NOLINT
    notes.push_back("If the queried node does not store historical state, the results for most older transactions are undefined.");  // NOLINT
    notes.push_back("If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is | not found, it will be used as a topic.");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    usageErrs[ERR_PROVIDEONETXID] = "Please specify at least one transaction identifier.";
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
