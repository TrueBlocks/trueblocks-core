/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
CParams params[] = {
    CParams("-all",            "process all transactions from start of chain to latest block"),
    CParams("-start:<uint>",   "the first block to process"),
    CParams("-end:<uint>",     "the last block (less one) to process"),
    CParams("-nblocks:<uint>", "the number of blocks to visit (ignored for -a)"),
    CParams("",                "Build an account tree listing first transaction, latest transaction, and "
                               "node balance for each account.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool hasN = false;
    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;
        if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--nblocks:")) {
            arg = orig.Substitute("-n:","").Substitute("--nblocks:","");
            nBlocks = toLongU(arg);
            hasN = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--start:")) {
            arg = orig.Substitute("-s:","").Substitute("--start:","");
            startBlock = toLongU(arg);
            if (!isUnsigned(arg))
                return usage("Positive start block number expected: " + orig);

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--end:")) {
            arg = orig.Substitute("-e:","").Substitute("--end:","");
            if (arg == "latest") {
                endBlock = getLatestBlockFromClient();
            } else {
                endBlock = toLongU(arg);
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
