#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
    CParams("~block_list", "a space-separated list of one or more blocks to search for"),
    CParams("-account",    "find an account file, not the block file"),
    CParams("-bloom",      "find a bloom file, not the block file"),
    CParams("",            "Reports if a block was found in the cache, at a local, or at a remote node.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = isNodeRunning() ? getLatestBlockFromClient() : 7000000;
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;

        if (arg == "-a" || arg == "--account") {
            mode = "account";

        } else if (arg == "-b" || arg == "--bloom") {
            mode = "bloom";

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {

            SFString ret = blocks.parseBlockList(arg, latestBlock);
            if (ret.endsWith("\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (!blocks.hasBlocks())
        return usage("You must enter a valid block number. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    mode = "block";
    optionOff(OPT_DENOM);
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "options")
        return str.Substitute("block_list", "<block> [block...]");
    return str;
}
