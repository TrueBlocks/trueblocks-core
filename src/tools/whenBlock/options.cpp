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
    COption("check", "c", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, checks the validity of the timestamp data"),  // NOLINT
    COption("fix", "f", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, fixes incorrect timestamps if any"),  // NOLINT
    COption("no_update", "n", "", OPT_HIDDEN | OPT_SWITCH, "do not update timestamps database prior to completing the task at hand"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Find block(s) based on date, blockNum, timestamp, or 'special'."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& commandIn) {
    string_q command = commandIn;
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    commandIn = substitute(commandIn, "--timestamps", "");

    getBlockHeader(latest, "latest");
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (arg == "-f" || arg == "--fix") {
            fix = true;

        } else if (arg == "-n" || arg == "--no_update") {
            no_update = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    if (isTestMode() && !check && !fix)
        return usage("--timestamp option not tested in testMode");
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, NOOPT, OPT_DENOM | OPT_CHECKLATEST);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    check = false;
    fix = false;
    no_update = false;
    // END_CODE_INIT

    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.");
    notes.push_back("Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    usageErrs[ERR_OPENINGTIMESTAMPS] = "Could not open timestamp file.";
    usageErrs[ERR_ONLYTS] = "The --check, --fix, and --count options are only available with the --timestamps option.";
    // END_ERROR_STRINGS

    // Differnt default for this software, but only change it if user hasn't already therefor not in Init
    if (!isApiMode())
        expContext().exportFmt = TXT1;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
