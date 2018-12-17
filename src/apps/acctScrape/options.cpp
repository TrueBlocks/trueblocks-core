/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-maxBlocks:<val>",  "the maximum number of blocks to visit during this run"),
    COption("-oneBlock:<val>",   "check if the block would be a hit"),
    COption("-oneTra(n)s:<val>", "check if the block and transaction would be a hit"),
    COption("-writeBlocks",      "write binary blocks to cache (default: do not write blocks)"),
    COption("-maxBlocks:<val>",  "scan at most --maxBlocks blocks ('all' implies scan to end of chain)"),
    COption("-noBloom(s)",       "do not use adaptive enhanced blooms (much faster if you use them)"),
    COption("@checkAddrs",       "use the per-block address lists (disabled)"),
    COption("@logLevel:<val>",   "specify the log level (default 1)"),
    COption("",                  "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isAll = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "--all" || arg == "-a") arg = "--maxBlocks:all";
        if (startsWith(arg, "-m:") || startsWith(arg, "--maxBlocks:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--maxBlocks:", "");
            if (arg == "all") {
                isAll = true;
            } else {
                if (isUnsigned(arg))
                    maxBlocks = str_2_Uint(arg);
                else
                    return usage("Please provide an integer value of maxBlocks. Quitting...");
            }

        } else if (startsWith(arg, "-o:") || startsWith(arg, "--oneBlock:")) {
            arg = substitute(substitute(arg, "-o:", ""), "--oneBlock:", "");
            if (isUnsigned(arg))
                oneBlock = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for oneBlock. Quitting...");

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--oneTrans:")) {
            arg = substitute(substitute(arg, "-n:", ""), "--oneTrans:", "");
            if (isUnsigned(arg))
                oneTrans = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for oneTrans. Quitting...");

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--level:")) {
            arg = substitute(substitute(arg, "-l:", ""), "--level:", "");
            if (isUnsigned(arg))
                logLevel = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for --logLevel. Quitting...");

        } else if (arg == "-c" || arg == "--checkAddrs") {
            checkAddrs = true;

        } else if (arg == "-s" || arg == "--noBlooms") {
            ignoreBlooms = true;

        } else if (arg == "-w" || arg == "--writeBlocks") {
            writeBlocks = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            return usage("Invalid option: " + arg);
        }
    }

//#define WHICH_BLOCK 4759050
//#define WHICH_TRANS 199
#ifdef WHICH_BLOCK
    visitor.opts.oneBlock = WHICH_BLOCK;
#ifdef WHICH_TRANS
    visitor.opts.oneTrans = WHICH_TRANS;
#endif
#endif

    if (!fileExists("./config.toml"))
        return usage("The config.toml file was not found. Are you in the right folder? Quitting...\n");

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);

    CToml toml("./config.toml");
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    manageFields(toml.getConfigStr("fields", "show", ""), true );

    if (oneTrans && !oneBlock)
        return usage("If you specify oneTrans, you must specify oneBlock. Quitting...");

    if (isAll && oneBlock)
        return usage("Choose either --all or --oneBlock, not both. Quitting...");

    CBlock latest;
    getBlock(latest, "latest");
    lastTimestamp = latest.timestamp;

    if (isAll)
        maxBlocks = INT_MAX;

    // Note: Pick up these defaults from the blockScrape application settings.

    // If writeBlocks is true, then the user told us what to do on the command line and we obey them. If it's false
    // then we check to see if the config file for blockScrape is set to write blocks. If the blockScraper is writing
    // blocks, we don't have to. If not, then we do want to write blocks here (therefore we negate the config value).
    if (!writeBlocks)
        writeBlocks = !getGlobalConfig("blockScrape")->getConfigBool("settings", "writeBlocks", true);

    // Exclusions are always picked up from the blockScraper
    exclusions = toLower(getGlobalConfig("blockScrape")->getConfigStr("exclusions", "list", ""));

    if (!isParity() || !nodeHasTraces())
        return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;
    optionOn(OPT_RUNONCE);

    lastBlock     = 0;
    minWatchBlock = 0;
    maxWatchBlock = UINT32_MAX;
    maxBlocks     = 10000;
    minArgs       = 0;
    oneBlock      = 0;
    oneTrans      = 0;
    lastTimestamp = 0;
    writeBlocks   = false;
    checkAddrs    = false;
    ignoreBlooms  = false;
    firstBlock    = 0;
    nBlocks       = 0;
    blockCounted  = false;
    debugging     = 0;
    logLevel      = 1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : blkStats(), addrStats(), transStats(), traceStats(), txCache(WRITING_ARCHIVE) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
