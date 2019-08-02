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
// BEG_CODE_OPTIONS
static const COption params[] = {
//#define NEW_CODE
#ifdef NEW_CODE
    COption2("block_list", "", "", OPT_REQUIRED | OPT_FLAG, "a space-separated list of one or more blocks for which to retrieve blooms"),
    COption2("eab", "", "", OPT_FLAG, "pull the enhanced adaptive blooms from QBlocks cache"),
    COption2("block", "", "", OPT_FLAG, "show only the block-level bloom (--raw only)"),
    COption2("receipts", "", "", OPT_FLAG, "show only the receipt-level blooms (--raw only)"),
    COption2("bits", "", "", OPT_FLAG, "display blooms as bits instead of hex"),
    COption2("bars", "", "", OPT_FLAG, "display blooms as bar chart instead of hex"),
    COption2("bitbars", "", "", OPT_FLAG, "display nBits as a bar chart"),
    COption2("pctbars", "", "", OPT_FLAG, "display nBits as a percentage of bloom space"),
    COption2("bitcount", "", "", OPT_FLAG, "display the number of bits lit per bloom"),
    COption2("force", "", "", OPT_HIDDEN | OPT_FLAG, "force a re-write of the bloom to the cache"),
    COption2("", "", "", 0, "Returns bloom filter(s) from running node (the default) or as EAB from QBlocks."),
#else // NEW_CODE
    COption("~block_list",  "a space-separated list of one or more blocks for which to retrieve blooms"),
    COption("-eab",         "pull the enhanced adaptive blooms from QBlocks cache"),
    COption("-block",       "show only the block-level bloom (--raw only)"),
    COption("-re(c)eipts",  "show only the receipt-level blooms (--raw only)"),
    COption("-b(i)ts",      "display blooms as bits instead of hex"),
    COption("-b(a)rs",      "display blooms as bar chart instead of hex"),
    COption("-bitbar(s)",   "display nBits as a bar chart"),
    COption("-pctbars",     "display nBits as a percentage of bloom space"),
    COption("-bitcou(n)t",  "display the number of bits lit per bloom"),
    COption("@force",       "force a re-write of the bloom to the cache"),
    COption("",             "Returns bloom filter(s) from running node (the default) or as EAB from QBlocks.\n"),
#endif
};
static const size_t nParams = sizeof(params) / sizeof(COption);
// END_CODE_OPTIONS

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLastBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-o" || arg == "--force") {
            etherlib_init(defaultQuitHandler);
            force = true;
            isRaw = false;

        } else if (arg == "-r" || arg == "--raw") {
            isRaw = true;  // last in wins

        } else if (arg == "-e" || arg == "--eab") {
            isRaw = false;  // last in wins

        } else if (arg == "-n" || arg == "--bitcount") {
            bitCount = true;

        } else if (arg == "-b" || arg == "--block") {
            blockOnly = true;
            if (receiptsOnly)
                return usage("Please choose either --block or --receipt, not both. Quitting...");

        } else if (arg == "-c" || arg == "--receipts") {
            receiptsOnly = true;
            if (blockOnly)
                return usage("Please choose either --block or --receipt, not both. Quitting...");

        } else if (arg == "-a" || arg == "--bars") {
            asBars = true;

        } else if (arg == "-i" || arg == "--bits") {
            asBits = true;

        } else if (arg == "-s" || arg == "--bitbars") {
            asBitBars = true;

        } else if (arg == "-p" || arg == "--pctbars") {
            asPctBars = true;

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

    if (asBits + asBars + asBitBars + asPctBars > 1)
        return usage("Only one of --bits, --bars, --barbits, or --pctbars may be chosen. Quitting...");

    if (!blocks.hasBlocks())
        return usage("You must specify at least one block number or block hash. Quitting...");

    if (blockOnly && !isRaw)
        return usage("--eab and --block options are not allowed together. Choose one. Quitting...");

    if (receiptsOnly && !isRaw)
        return usage("--eab and --receipts options are not allowed together. Choose one. Quitting...");

    // Initialize these here (hack alert)
    CBloomReceipt noRemove1; CBloomTrans noRemove2; CBloomBlock noRemove3;
    HIDE_FIELD(CBloomTrans, "hash");
    if (blockOnly)
        HIDE_FIELD(CBloomBlock, "transactions");

    if (receiptsOnly)
        HIDE_FIELD(CBloomBlock, "logsBloom");

    if (bitCount || verbose) {
        UNHIDE_FIELD(CBloomReceipt, "bitCount");
        UNHIDE_FIELD(CBloomBlock, "bitCount");
        if (verbose)
            UNHIDE_FIELD(CBloomBlock, "sizeInBytes");
    }

    // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
    bitBound = getGlobalConfig("blockScrape")->getConfigInt("settings", "bitBound", 200);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    optionOn(OPT_RAW);
    registerOptions(nParams, params);

    isRaw        = true; // unusual, but true
    asBits       = false;
    asBars       = false;
    asBitBars    = false;
    asPctBars    = false;
    bitCount     = false;
    force        = false;
    receiptsOnly = false;
    blockOnly    = false;
    bitBound     = 200;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Mimics python -m json.tool indenting.
    expContext().spcs = 2;
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
        ret += "This tool retrieves information from the local node or rpcProvider if "
                        "configured (see documentation).\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
