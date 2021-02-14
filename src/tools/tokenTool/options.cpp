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
    COption("~token_address", "an ERC20 token addresses"),
    COption("-reverse", "present the cap table in reverse order by holding"),
    COption("-start:<num>", "block on which to start the analysis"),
    COption("-bucket:<num>", "number of block to process between cap table reports"),
    COption("-nRows:<num>", "show this many rows of the cap table (default = 30)"),
    COption("-showErrors", "in verbose mode, print out in-error transactions (never effects accounting)"),
    COption("", "Show ERC20 token cap tables with various options.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (arg == "-s" || arg == "--showErrors") {
            showErrors = true;

        } else if (arg == "-r" || arg == "--reverse") {
            reverse = true;

        } else if (startsWith(arg, "-s") || startsWith(arg, "--s tart")) {
            arg = substitute(substitute(arg, "-s:", ""), "--s tart:", "");
            if (!isNumeral(arg))
                return usage("Start option must be a numeral.");
            start = str_2_Uint(arg);

        } else if (startsWith(arg, "-n") || startsWith(arg, "--nRows")) {
            arg = substitute(substitute(arg, "-n:", ""), "--nRows:", "");
            if (!isNumeral(arg))
                return usage("nRows option must be a numeral.");
            nRows = str_2_Uint(arg);

        } else if (startsWith(arg, "-b") || startsWith(arg, "--bucket")) {
            arg = substitute(substitute(arg, "-b:", ""), "--bucket:", "");
            if (!isNumeral(arg))
                return usage("Please specify a numeral with the --bucket option.");
            bucketSize = str_2_Uint(arg);

        } else if (startsWith(arg, "0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address.");
            token = toLower(arg);

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            return usage("Invalid option: " + arg);
        }
    }

    if (token.empty())
        return usage("Please supply a single ERC20 token address.");

    // We need a locally running node
    if (!isNodeRunning())
        return usage("This command needs a running Ethereum node.");

#ifndef DEBUGGING
    // We need a locally running >> archive << node
    if (!nodeHasBalances())
        return usage("This command only runs if the node has balances.");
#endif

    // We need to find the cache
    cacheFile = getMonitorPath(token);
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
    if (!abi_spec.loadAbiFromEtherscan(token))
        return usage("Could not find the ABI. Run grabABI " + token + " to retrieve it.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    getBlock(latest, "latest");
    showErrors = false;
    reverse = false;
    start = 0;
    cacheFile = "";
    bucketSize = 100;
    nRows = 30;
    capTable.pOptions = this;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Mimics python -m json.tool indenting.
    expContext().spcs = 4;
    expContext().hexNums = false;
    expContext().quoteNums = false;

    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
