/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
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
    COption("list", "l", "", OPT_SWITCH, "list the index and bloom filter hashes from local manifest or pinning service"),  // NOLINT
    COption("init", "i", "", OPT_SWITCH, "initialize the local index by downloading bloom filters from the pinning service"),  // NOLINT
    COption("compare", "c", "", OPT_SWITCH, "report differences (if any) between the manifest and pinning service"),
    COption("freshen", "f", "", OPT_SWITCH, "freshen the manifest from the hash found at the smart contract"),
    COption("sleep", "s", "<double>", OPT_HIDDEN | OPT_FLAG, "the number of seconds to sleep between requests during init (default .25)"),  // NOLINT
    COption("remote", "r", "", OPT_HIDDEN | OPT_SWITCH, "applicable only to --list mode, recover hash list from pinning service (local otherwise)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Manage pinned index of appearances and associated bloom filters."),
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
    bool remote = false;
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

        } else if (arg == "-c" || arg == "--compare") {
            compare = true;

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sleep:")) {
            if (!confirmDouble("sleep", sleep, arg))
                return false;
        } else if (arg == "-s" || arg == "--sleep") {
            return flag_required("sleep");

        } else if (arg == "-r" || arg == "--remote") {
            remote = true;

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
    LOG_TEST_BOOL("compare", compare);
    LOG_TEST_BOOL("freshen", freshen);
    LOG_TEST("sleep", sleep, (sleep == .25));
    LOG_TEST_BOOL("remote", remote);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    LOG_INFO("hashToIndexFormatFile:\t", cGreen, hashToIndexFormatFile, cOff);
    LOG_INFO("hashToBloomFormatFile:\t", cGreen, hashToBloomFormatFile, cOff);
    LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
    LOG_INFO("manifestHashEncoding:\t", cGreen, manifestHashEncoding, cOff);

    if (list + compare + init > 1)
        return usage("Please choose either only a single option.");

    if (!list && !compare && !init && !freshen)
        return usage("You must choose at least one option.");

    configureDisplay("pinMan", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    init = false;
    compare = false;
    freshen = false;
    sleep = .25;
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    firstOut = true;

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("One of --list, --init, --compare, or --freshen is required.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
