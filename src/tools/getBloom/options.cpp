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
    CParams("~block_list",       "a space-separated list of one or more blocks for which to retrieve blooms"),
    CParams("-raw",              "pull the bloom filter directly from the running node (the default)"),
    CParams("-eab",              "pull the enhanced adaptive bloom filters (EAB) from the local cache"),
    CParams("-asbits",           "display the bloom filter as bits instead of hex"),
    CParams("-a(s)bars",         "display the bloom filter as bar charts instead of hex"),
    CParams("-recei(p)t",        "display blooms from the transaction receipts as opposed to block (--raw only)"),
    CParams("@force",            "force a re-write of the bloom to the cache"),
    CParams("",                  "Returns bloom filter(s) from running node (the default) or as EAB from local cache.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');

        if (arg == "-o" || arg == "--force") {
            etherlib_init("binary");
            force = true;

        } else if (arg == "-r" || arg == "--raw") {
            isRaw = true;

        } else if (arg == "-e" || arg == "--eab") {
            isRaw = false;

        } else if (arg == "-a" || arg == "--asbits") {
            asBits = true;

        } else if (arg == "-s" || arg == "--asbars") {
            asBars = true;

        } else if (arg == "-p" || arg == "--receipt") {
            UNHIDE_FIELD(CBlock,       "transactions");
            UNHIDE_FIELD(CTransaction, "receipt");
            UNHIDE_FIELD(CTransaction, "transactionIndex");
            UNHIDE_FIELD(CReceipt,     "logsBloom");
            receipt = true;

        } else if (arg.startsWith('-')) {  // do not collapse
            if (!builtInCmd(arg))
                return usage("Invalid option: " + arg);

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
        return usage("You must specify at least one block number or block hash. Quitting...");

    if (asBits && asBars)
        return usage("Please pick on of --asbits and --asbars. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;
    pOptions = this;

    isRaw      = true;
    asBits     = false;
    asBars     = false;
    force      = false;
    receipt    = false;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Mimics python -m json.tool indenting.
    expContext().spcs = 4;
    expContext().hexNums = false;
    expContext().quoteNums = false;

    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    HIDE_ALL_FIELDS(CBlock);
    HIDE_ALL_FIELDS(CTransaction);
    HIDE_ALL_FIELDS(CReceipt);
    UNHIDE_FIELD(CBlock, "blockNumber");
    UNHIDE_FIELD(CBlock, "logsBloom");
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::isMulti(void) const {
    return ((blocks.stop - blocks.start) > 1 || blocks.hashList.getCount() > 1 || blocks.numList.getCount() > 1);
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {

    if (which == "options") {
        return
            str.Substitute("block_list", "<block> [block...]")
                .Substitute("-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        SFString ret;
        ret += "[{block_list}] is a space-separated list of values, a start-end range, a [{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
