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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("all", "a", "", OPT_FLAG, "process all transactions from start of chain to latest block"),
    COption("first", "f", "<uint>", OPT_FLAG, "the first block to process"),
    COption("last", "l", "<uint>", OPT_FLAG, "the last block (less one) to process"),
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

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--first:")) {
            arg = substitute(substitute(orig, "-f:", ""), "--first:", "");
            startBlock = str_2_Uint(arg);
            if (!isUnsigned(arg))
                return usage("Positive start block number expected: " + orig);

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--last:")) {
            arg = substitute(substitute(orig, "-l:", ""), "--last:", "");
            if (arg == "latest") {
                endBlock = getBlockProgress(BP_CLIENT).client;
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
        return usage("You may use either -n or -e, not both.");
    if (endBlock != NOPOS)
        nBlocks = (endBlock - startBlock);
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

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
