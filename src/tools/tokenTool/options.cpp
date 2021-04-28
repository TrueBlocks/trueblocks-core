/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("tokens", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "an ERC20 token addresses"),
    COption("reverse", "r", "", OPT_SWITCH, "present the cap table in reverse order by holding"),
    COption("start", "s", "<blknum>", OPT_FLAG, "block on which to start the analysis"),
    COption("bucket", "b", "<uint64>", OPT_FLAG, "number of block to process between cap table reports (default = 100)"),  // NOLINT
    COption("n_rows", "n", "<uint64>", OPT_FLAG, "show this many rows of the cap table (default = 30)"),
    COption("show_errs", "r", "", OPT_SWITCH, "in verbose mode, print out in-error transactions (never affects accounting)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Show ERC20 token cap tables with various options."),
    // clang-format on
    // END_CODE_OPTIONS
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    blknum_t latest = getBlockProgress(BP_CLIENT).client;
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-r" || arg == "--reverse") {
            reverse = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--start:")) {
            if (!confirmBlockNum("start", start, arg, latest))
                return false;
        } else if (arg == "-s" || arg == "--start") {
            return usage("The --start option requires a value.");

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--bucket:")) {
            if (!confirmUint("bucket", bucket, arg))
                return false;
        } else if (arg == "-b" || arg == "--bucket") {
            return usage("The --bucket option requires a value.");

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--n_rows:")) {
            if (!confirmUint("n_rows", n_rows, arg))
                return false;
        } else if (arg == "-n" || arg == "--n_rows") {
            return usage("The --n_rows option requires a value.");

        } else if (arg == "-r" || arg == "--show_errs") {
            show_errs = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseAddressList(this, tokens, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("tokens", tokens, tokens.empty());
    LOG_TEST_BOOL("reverse", reverse);
    LOG_TEST("start", start, (start == 0));
    LOG_TEST("bucket", bucket, (bucket == 100));
    LOG_TEST("n_rows", n_rows, (n_rows == 30));
    LOG_TEST_BOOL("show_errs", show_errs);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    if (tokens.empty() || tokens.size() > 1)
        return usage("Please supply a single ERC20 token address.");

    // We need a locally running node
    if (!isNodeRunning())
        return usage("This command needs a running Ethereum node.");

#ifndef DEBUGGING
    // We need a locally running >> archive << node
    if (!nodeHasBalances(false))
        return usage("This command only runs if the node has balances.");
#endif

    // We need to find the cache
    CMonitor m;
    cacheFile = m.getMonitorPath(tokens[0]);
    if (!fileExists(cacheFile))
        return usage("Could not find cache file: " + cacheFile);

    // We need to know the cache is not locked
    if (fileExists(cacheFile + ".lck"))
        return usage(
            "The cache lock file is present. The program is either already running or it did not end "
            "cleanly the\n\tlast time it ran. Quit the already running program or, if it is not running, "
            "remove the lock\n\tfile: " +
            cacheFile + ".lck'.");

    // We need an ABI (although we could run without it)
    if (!abi_spec.loadAbiFromEtherscan(tokens[0]))
        return usage("Could not find the ABI. Run grabABI " + tokens[0] + " to retrieve it.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    reverse = false;
    start = 0;
    bucket = 100;
    n_rows = 30;
    show_errs = false;
    // END_CODE_INIT

    cacheFile = "";
    capTable.pOptions = this;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Mimics python -m json.tool indenting.
    expContext().spcs = 4;
    expContext().hexNums = false;
    expContext().quoteNums = false;

    Init();

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
