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
    COption("list", "l", "", OPT_SWITCH, "list the index and Bloom filter hashes from local manifest or pinning service"),  // NOLINT
    COption("init", "i", "", OPT_SWITCH, "initialize index of appearances by downloading Bloom filters from pinning service"),  // NOLINT
    COption("freshen", "f", "", OPT_SWITCH, "freshen index of appearances using the same mode from most recent --init"),
    COption("remote", "r", "", OPT_HIDDEN | OPT_SWITCH, "for --list mode only, recover the manifest from pinning service"),  // NOLINT
    COption("all", "a", "", OPT_SWITCH, "for --init and --freshen modes only, download full index chunks as well as Bloom filter"),  // NOLINT
    COption("sleep", "s", "<double>", OPT_HIDDEN | OPT_FLAG, "the number of seconds to sleep between requests during download (default .25)"),  // NOLINT
    COption("share", "S", "", OPT_SWITCH, "pin downloaded files to your local IPFS store, that is, share them (requires IPFS)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Manage pinned index of appearances and associated Bloom filters."),
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

        } else if (arg == "-i" || arg == "--init") {
            init = true;

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg == "-r" || arg == "--remote") {
            remote = true;

        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sleep:")) {
            if (!confirmDouble("sleep", sleep, arg))
                return false;
        } else if (arg == "-s" || arg == "--sleep") {
            return flag_required("sleep");

        } else if (arg == "-S" || arg == "--share") {
            share = true;

        } else if (arg == "-n" || arg == "--init_all") {
            // clang-format off
            return usage("the --init_all option is deprecated, use --init --all instead");  // NOLINT
            // clang-format on

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_BOOL("list", list);
    LOG_TEST_BOOL("init", init);
    LOG_TEST_BOOL("freshen", freshen);
    LOG_TEST_BOOL("remote", remote);
    LOG_TEST_BOOL("all", all);
    LOG_TEST("sleep", sleep, (sleep == .25));
    LOG_TEST_BOOL("share", share);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    LOG_INFO("hashToIndexFormatFile:\t", cGreen, hashToIndexFormatFile, cOff);
    LOG_INFO("hashToBloomFormatFile:\t", cGreen, hashToBloomFormatFile, cOff);
    LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
    LOG_INFO("manifestHashEncoding:\t", cGreen, manifestHashEncoding, cOff);

    if (list && (init || freshen))
        return usage("Please choose only a single option.");

    if (!list && !init && !freshen)
        return usage("You must choose at least one of --list, --init, or --freshen.");

    if (freshen)
        init = true;

    if (share) {
        string_q res = doCommand("which ipfs");
        if (res.empty())
            return usage("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
    }

    configureDisplay("pinMan", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    init = false;
    freshen = false;
    remote = false;
    all = false;
    sleep = .25;
    share = false;
    // END_CODE_INIT

    freshenAll = false;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("One of `--list`, `--init`, or `--freshen` is required.");
    notes.push_back("the `--share` option only works if the IPFS executable is in your path.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
