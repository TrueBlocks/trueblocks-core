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
#define LOGGING_LEVEL_TEST
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("terms", "", "list<string>", OPT_POSITIONAL, "one or more address, name, block, or transaction identifier"),  // NOLINT
    COption("local", "l", "", OPT_SWITCH, "open the local TrueBlocks explorer"),
    COption("google", "g", "", OPT_SWITCH, "search google excluding popular blockchain explorers"),
    COption("", "", "", OPT_DESCRIPTION, "Open an explorer for one or more addresses, blocks, or transactions."),
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
        } else if (arg == "-l" || arg == "--local") {
            local = true;

        } else if (arg == "-g" || arg == "--google") {
            google = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseStringList2(this, terms, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("terms", terms, terms.empty());
    LOG_TEST_BOOL("local", local);
    LOG_TEST_BOOL("google", google);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    //    configureDisplay("fireStorm", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    if (google) {
        const char* STR_GOOGLE = "https://www.google.com/search?q=";
        CStringArray exclusions = {
            "etherscan", "etherchain", "bloxy",     "bitquery", "ethplorer",
            "tokenview", "blockshain", "anyblocks", "explorer",
        };
        ostringstream addendum;
        for (auto ex : exclusions)
            addendum << "+-" << ex << " ";
        for (auto term : terms) {
            ostringstream query;
            query << "open " << STR_GOOGLE << term << addendum.str();
            if (!system(query.str().c_str()))
                return false;
        }
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, NOOPT, OPT_FMT | OPT_VERBOSE);

    // BEG_CODE_INIT
    local = false;
    google = false;
    // END_CODE_INIT

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::forEveryTerm(CONSTAPPLYFUNC func, void* data) {
    if (!func)
        return true;

    for (auto term : terms) {
        if (!(*func)(term, data)) {
            return false;
        }
    }

    return true;
}
