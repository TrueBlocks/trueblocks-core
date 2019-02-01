/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~lists",        "list of block_nums or block_nums and addrs (if both, show hit stats for addrs)"),
    COption("-stats",        "calculate statistics for blooms in the block range"),
    COption("-rewrite",      "re-write the given bloom(s) -- works only with block numbers"),
    COption("-check",        "check that a bloom contains all addresses in the given block(s)"),
    COption("-upgrade",      "read, then write, all blooms (effectivly upgrading them)"),
    COption("-bucket:<val>", "for --stats only, optional bucket size"),
    COption("-c(u)m",        "for --stats only, stats are cummulative (per bucket otherwise) "),
    COption("@raw",          "print blooms from the raw node"),
    COption("",              "Work with EABs, raw blooms and/or present statistics."),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

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
            stats.cummulative = true;

        } else if (arg == "-r" || arg == "--rewrite") {
            isRewrite = true;
            registerQuitHandler(defaultQuitHandler);  // we want to protect writes

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

    hasAddrs  = !address_list.empty();
    hasBlocks =  blocks.hasBlocks();

    if (!hasBlocks)  // must have one or the other
        return usage("You must provide at least one block number to process. Quitting...");

    if ((isCheck + isRewrite + isStats) > 1) // cannot do more than one option
        return usage("You must choose only one of --check, --rewrite, or --stats. Quitting...");

    if (isRaw && hasAddrs)
        return usage("--raw only works with blocks, not addresses. Quitting...");

    if (isTestMode() && blocks.numList.size() == 4) {
        // We turn off testing mode in this case because one of our test cases from getBloom
        // needs to insure that the blocks for that test are present
        setenv("TEST_MODE", "false", true);
    }

    // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
    bitBound = getGlobalConfig("blockScrape")->getConfigInt("settings", "bitBound", 200);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    isStats           = false;
    isRewrite         = false;
    isCheck           = false;
    isRaw             = false;
    stats.cummulative = false;
    address_list      = "";
    bucketSize        = 10000;
    bitBound          = 200;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : stats(this) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    if (isStats && stats.curBucket != stats.lastReport && stats.nVisits > 1)
        cout << stats.report(stats.lastReport+stats.nVisits+1) << endl;
}
