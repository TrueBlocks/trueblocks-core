/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~block_list", "a space-separated list of one or more blocks to search for"),
    CParams("-alone",      "if the block(s) is/are found in the cache, display the path(s) to those blocks"),
    CParams("",            "Reports if a block was found in the cache or at a local or remote node.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    COptionsBlockList block_list;
    blknum_t latestBlock = isNodeRunning() ? getLatestBlockFromClient() : 7000000;
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;

        if (arg == "-a" || arg == "--alone") {
            alone = true;

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {

            SFString ret = block_list.parseBlockList(arg, latestBlock);
            if (ret.endsWith("\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    SFString theList = block_list.toString();
    while (!theList.empty()) {
        SFString val = nextTokenClear(theList, '|');
        blocks[blocks.getCount()] = toUnsigned(val);
    }

    if (!blocks.getCount())
        return usage("You must enter a valid block number. Quitting...");

    if (alone && !isNodeRunning())
        alone = false;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    blocks.Clear();
    alone = false;
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
