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
    COption("list", "l", "", OPT_SWITCH, "list the bloom and index hashes from local cache or IPFS"),
    COption("check", "c", "", OPT_SWITCH, "for each chunk, put it to IPFS and verify its manifest hash"),
    COption("", "", "", OPT_DESCRIPTION, "Manage chunks and bloom filters."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool list = false;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_BOOL("list", list);
    LOG_TEST_BOOL("check", check);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    // if (list && (init || freshen)) {
    //     return usage("Please choose only a single option.");
    // }

    // if (!list && !init && !freshen) {
    //     return usage("You must choose at least one of --list, --init, or --freshen.");
    // }

    // if (remote && !list) {
    //     return usage("The --remote option is only available with the --list option");
    // }

    // if (remote) {
    //     return usage("The --remote option is not yet implemented");
    // }

    // if (all && !init) {
    //     return usage("Use the --all option only with the --init or --freshen options.");
    // }

    // // if (init_all) {
    // //     return usage("Flag --init_all has been deprecated, use --init --all instead")
    // // }

    // if (share) {
    //     string_q res = doCommand("which ipfs");
    //     if (res.empty())
    //         return usage("Could not find ipfs in your $PATH. You must install ipfs for the --share command to
    //         work.");
    // }

    // if (freshen)
    //     init = true;

    // LOG_INFO("hashToIndexFormatFile:\t", cGreen, hashToIndexFormatFile, cOff);
    // LOG_INFO("hashToBloomFormatFile:\t", cGreen, hashToBloomFormatFile, cOff);
    // LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
    // LOG_INFO("manifestHashEncoding:\t", cGreen, manifestHashEncoding, cOff);

    configureDisplay("chunkMan", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    check = false;
    // END_CODE_INIT
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
