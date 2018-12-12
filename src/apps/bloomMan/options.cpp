/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~lists",        "list of either account address, blocks, or both (if both, show hits statistics)"),
    COption("-stats",        "calculate statistics for blooms in the block range"),
    COption("-bucket:<val>", "optional bucket size of --stats"),
    COption("-c(u)m",        "statistics are cummulative (per bucket otherwise) "),
    COption("-rewrite",      "re-write the given bloom(s) -- works only with block numbers"),
    COption("-check",        "check that a bloom contains all addresses in the given block(s)"),
    COption("-upgrade",      "read, then write, all blooms (effectivly upgrading them)"),
    COption("@raw",          "print blooms from the raw node"),
    COption("",              "Work with QBlocks-style bloom filters and/or present statistics."),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    Init();
    blknum_t latestBlock = getLatestBlockFromCache();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-s" || arg == "--stats") {
            isStats = true;

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--bucket:")) {
            isStats = true;
            nextTokenClear(arg,':');
            if (isUnsigned(arg))
                bucketSize = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for bucketSize. Quitting...");

        } else if (arg == "-u" || arg == "--cum") {
            isCummulative = true;

        } else if (arg == "-r" || arg == "--rewrite") {
            isReWrite = true;

        } else if (arg == "--raw") {
            isRaw = true;

        } else if (arg == "-c" || arg == "--check") {
            isCheck = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (startsWith(arg, "0x") && isAddress(arg)) {
            address_list += arg + "|";

        } else {

            string_q ret = blocks.parseBlockList(arg, latestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (!blocks.hasBlocks() && address_list.empty()) // must have one or the other
        return usage("You must provide either a block number or an Ethereum address. Quitting...");

    if ((isCheck+isReWrite+isStats) > 1) // cannot do both
        return usage("You must choose only one of --check, --rewrite, or --stats. Quitting...");

    else if (!blocks.hasBlocks() || !address_list.empty()) { // has either addresses or no blocks
        if (isCheck || isReWrite || isStats)
            return usage("Provide only block numbers with the " +
                         string_q(isCheck ? "--check" : (isReWrite ? "--rewrite" : "--stats")) +
                         " option. Quitting...");
    }

    if (isRaw && !address_list.empty())
        return usage("--raw only works with blocks, not addresses. Quitting...");

    if (isTestMode() && blocks.numList.size() == 4) {
        // We turn off testing mode in this case because one of our test cases from getBloom
        // needs to insure that the blocks for that test are present
        setenv("TEST_MODE", "false", true);
    }

    bitBound = getGlobalConfig("blockScrape")->getConfigInt("settings", "bitBound", 200);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;

    isStats       = false;
    isReWrite     = false;
    isCheck       = false;
    isCummulative = false;
    isRaw         = false;
    address_list  = "";
    bucketSize    = 10000;
    bitBound      = 200;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
