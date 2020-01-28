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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("all", "a", "", OPT_FLAG, "process all transactions from start of chain to latest block"),
    COption("start", "s", "<uint>", OPT_FLAG, "the first block to process"),
    COption("end", "e", "<uint>", OPT_FLAG, "the last block (less one) to process"),
    COption("nblocks", "n", "<uint>", OPT_FLAG, "the number of blocks to visit (ignored for -a)"),
    COption(
        "", "", "", OPT_DESCRIPTION,
        "Build an account tree listing first transaction, latest transaction, and node balance for each account.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    bool hasN = false;
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--nblocks:")) {
            arg = substitute(substitute(orig, "-n:", ""), "--nblocks:", "");
            nBlocks = str_2_Uint(arg);
            hasN = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--start:")) {
            arg = substitute(substitute(orig, "-s:", ""), "--start:", "");
            startBlock = str_2_Uint(arg);
            if (!isUnsigned(arg))
                return usage("Positive start block number expected: " + orig);

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--end:")) {
            arg = substitute(substitute(orig, "-e:", ""), "--end:", "");
            if (arg == "latest") {
                endBlock = getLatestBlock_client();
            } else {
                endBlock = str_2_Uint(arg);
                if (!isUnsigned(arg))
                    return usage("Positive end block number expected: " + orig);
            }

        } else {
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (hasN && endBlock != NOPOS)
        return usage("You may use either -n or -e, not both. Quitting...");
    if (endBlock != NOPOS)
        nBlocks = (endBlock - startBlock);
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_FMT);

    all = false;
    startBlock = 0;
    endBlock = NOPOS;
    nBlocks = 500;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
