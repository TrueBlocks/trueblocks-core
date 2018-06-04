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
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    bool hasN = false;
    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg.startsWith("-n:") || arg.startsWith("--nblocks:")) {
            arg = orig.Substitute("-n:","").Substitute("--nblocks:","");
            nBlocks = newUnsigned32(arg);
            hasN = true;

        } else if (arg.startsWith("-s:") || arg.startsWith("--start:")) {
            arg = orig.Substitute("-s:","").Substitute("--start:","");
            startBlock = newUnsigned32(arg);
            if (!isUnsigned(arg))
                return usage("Positive start block number expected: " + orig);

        } else if (arg.startsWith("-e:") || arg.startsWith("--end:")) {
            arg = orig.Substitute("-e:","").Substitute("--end:","");
            if (arg == "latest") {
                endBlock = getLatestBlockFromClient();
            } else {
                endBlock = newUnsigned32(arg);
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
