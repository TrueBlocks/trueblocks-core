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
    COption("blocks", "", "list<blknum>", OPT_POSITIONAL, "an optional list of blocks to process"),
    COption("list", "l", "", OPT_SWITCH, "list the bloom and index hashes from local cache or IPFS"),
    COption("check", "c", "", OPT_SWITCH, "check the validity of the chunk or bloom"),
    COption("extract", "e", "enum[header|addr_table|app_table|chunks|blooms]", OPT_FLAG, "show some or all of the contents of the chunk or bloom filters"),  // NOLINT
    COption("stats", "s", "", OPT_SWITCH, "for the --list option only, display statistics about each chunk or bloom"),
    COption("save", "a", "", OPT_SWITCH, "for the --extract option only, save the entire chunk to a similarly named file as well as display"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Manage and investigate chunks and bloom filters."),
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
    blknum_t latest = getBlockProgress(BP_CLIENT).client;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--extract:")) {
            if (!confirmEnum("extract", extract, arg))
                return false;
        } else if (arg == "-e" || arg == "--extract") {
            return flag_required("extract");

        } else if (arg == "-s" || arg == "--stats") {
            stats = true;

        } else if (arg == "-a" || arg == "--save") {
            save = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseBlockList2(this, blocks, arg, latest))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("blocks", blocks, blocks.empty());
    LOG_TEST_BOOL("list", list);
    LOG_TEST_BOOL("check", check);
    LOG_TEST("extract", extract, (extract == ""));
    LOG_TEST_BOOL("stats", stats);
    LOG_TEST_BOOL("save", save);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;
    
    if (blocks.empty()) {
        blocks.start = NOPOS;
    } else if (blocks.size() > 0) {
        blocks.start = blocks[0];
    }

    configureDisplay("chunkMan", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    check = false;
    extract = "";
    stats = false;
    save = false;
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("Only a single block in a given chunk needs to be supplied.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
