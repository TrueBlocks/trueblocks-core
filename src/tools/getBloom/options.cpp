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
static COption params[] = {
    COption("~block_list",       "a space-separated list of one or more blocks for which to retrieve blooms"),
    COption("-raw",              "pull the bloom filter directly from the running node (the default)"),
    COption("-eab",              "pull the enhanced adaptive bloom filters (EAB) from the local cache"),
    COption("-asbits",           "display the bloom filter as bits instead of hex"),
    COption("-a(s)bars",         "display the bloom filter as bar charts instead of hex"),
    COption("-recei(p)t",        "display blooms from the transaction receipts as opposed to block (--raw only)"),
    COption("@force",            "force a re-write of the bloom to the cache"),
    COption("",                  "Returns bloom filter(s) from running node (the default) or as EAB "
                                    "from local cache.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    while (!command.empty()) {

        string_q arg = nextTokenClear(command, ' ');

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

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg))
                return usage("Invalid option: " + arg);

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
    sorts[0] = GETRUNTIME_CLASS(CBlock);       sorts[0]->hideAllFields();
    sorts[1] = GETRUNTIME_CLASS(CTransaction); sorts[1]->hideAllFields();
    sorts[2] = GETRUNTIME_CLASS(CReceipt);     sorts[2]->hideAllFields();
    UNHIDE_FIELD(CBlock, "blockNumber");
    UNHIDE_FIELD(CBlock, "logsBloom");
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::isMulti(void) const {
    return ((blocks.stop - blocks.start) > 1 || blocks.hashList.size() > 1 || blocks.numList.size() > 1);
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return
            substitute(substitute(str, "block_list", "<block> [block...]"), "-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{block_list}] is a space-separated list of values, a start-end range, a [{special}], "
                        "or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if "
                        "configured (see documentation).\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
