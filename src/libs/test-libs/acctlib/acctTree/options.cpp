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
    COption("-all",            "process all transactions from start of chain to latest block"),
    COption("-start:<uint>",   "the first block to process"),
    COption("-end:<uint>",     "the last block (less one) to process"),
    COption("-nblocks:<uint>", "the number of blocks to visit (ignored for -a)"),
    COption("",                "Build an account tree listing first transaction, latest transaction, and "
                               "node balance for each account.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

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
                endBlock = getLatestBlockFromClient();
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
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;

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
