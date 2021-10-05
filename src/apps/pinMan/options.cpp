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
    COption("init", "i", "", OPT_SWITCH, "download the blooms or index chunks from IPFS"),
    COption("freshen", "f", "", OPT_SWITCH, "check for new bloom or index chunks and download if available"),
    COption("all", "a", "", OPT_SWITCH, "in addition to Bloom filters, download full index chunks"),
    COption("share", "S", "", OPT_SWITCH, "share downloaded data by pinning it to IPFS (the IPFS daemon must be running)"),  // NOLINT
    COption("remote", "r", "", OPT_SWITCH, "for --list mode only, recover the manifest from IPFS via UnchainedIndex smart contract"),  // NOLINT
    COption("sleep", "s", "<double>", OPT_FLAG, "throttle requests by this many seconds (.25 seconds delay between requests by default)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Manage pinned index of appearances and associated blooms."),
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

        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg == "-S" || arg == "--share") {
            share = true;

        } else if (arg == "-r" || arg == "--remote") {
            remote = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sleep:")) {
            if (!confirmDouble("sleep", sleep, arg))
                return false;
        } else if (arg == "-s" || arg == "--sleep") {
            return flag_required("sleep");

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
    LOG_TEST_BOOL("all", all);
    LOG_TEST_BOOL("share", share);
    LOG_TEST_BOOL("remote", remote);
    LOG_TEST("sleep", sleep, (sleep == .25));
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    if (list && (init || freshen)) {
        return usage("Please choose only a single option.");
    }

    if (!list && !init && !freshen) {
        return usage("You must choose at least one of --list, --init, or --freshen.");
    }

    if (remote && !list) {
        return usage("The --remote option is only available with the --list option");
    }

    if (remote) {
        return usage("The --remote option is not yet implemented");
    }

    if (all && !init) {
        return usage("Use the --all option only with the --init or --freshen options.");
    }

    // if (init_all) {
    //     return usage("Flag --init_all has been deprecated, use --init --all instead")
    // }

    if (share) {
        string_q res = doCommand("which ipfs");
        if (res.empty())
            return usage("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
    }

    if (freshen)
        init = true;

    LOG_INFO("hashToIndexFormatFile:\t", cGreen, hashToIndexFormatFile, cOff);
    LOG_INFO("hashToBloomFormatFile:\t", cGreen, hashToBloomFormatFile, cOff);
    LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
    LOG_INFO("manifestHashEncoding:\t", cGreen, manifestHashEncoding, cOff);

    configureDisplay("pinMan", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    init = false;
    freshen = false;
    all = false;
    share = false;
    remote = false;
    sleep = .25;
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The --freshen option is similar to --init, but checks UnchainedIndex first.");
    notes.push_back("One of `--list`, `--init`, or `--freshen` is required.");
    notes.push_back("The `--share` option only works if the IPFS daemon is running.");
    notes.push_back("Re-run `chifra init` as you wish. It will repair or freshen the index.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
