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
    COption("", "", "", OPT_DESCRIPTION, "Retrieve a transaction's logs from the cache or the node."),
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
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    // BEG_CODE_INIT
    articulate = false;
    // END_CODE_INIT

    transList.Init();
    option1 = false;
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
    notes.push_back("If the queried node does not store historical state, the results may be undefined.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
