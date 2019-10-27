/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("blocks", "", "list<blknum>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more blocks for which to retrieve blooms"),
    COption("eab", "e", "", OPT_SWITCH, "pull the enhanced adaptive blooms from QBlocks cache"),
    COption("block_only", "b", "", OPT_SWITCH, "show only the block-level bloom (--raw only)"),
    COption("receipt_only", "r", "", OPT_SWITCH, "show only the receipt-level blooms (--raw only)"),
    COption("bits", "i", "", OPT_SWITCH, "display blooms as bits instead of hex"),
    COption("bars", "a", "", OPT_SWITCH, "display blooms as bar chart instead of hex"),
    COption("bitbars", "s", "", OPT_SWITCH, "display nBits as a bar chart"),
    COption("pctbars", "p", "", OPT_SWITCH, "display nBits as a percentage of bloom space"),
    COption("bitcount", "t", "", OPT_SWITCH, "display the number of bits lit per bloom"),
    COption("force", "f", "", OPT_HIDDEN | OPT_SWITCH, "force a re-write of the bloom to the cache"),
    COption("", "", "", OPT_DESCRIPTION, "Returns bloom filter(s) from running node (the default) or as EAB from QBlocks."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

// BEG_CODE_LOCAL_INIT
    bool eab = false;
    bool force = false;
// END_CODE_LOCAL_INIT

    Init();
    blknum_t latest = getLastBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
        } else if (arg == "-e" || arg == "--eab") {
            eab = true;

        } else if (arg == "-b" || arg == "--block_only") {
            block_only = true;

        } else if (arg == "-r" || arg == "--receipt_only") {
            receipt_only = true;

        } else if (arg == "-i" || arg == "--bits") {
            bits = true;

        } else if (arg == "-a" || arg == "--bars") {
            bars = true;

        } else if (arg == "-s" || arg == "--bitbars") {
            bitbars = true;

        } else if (arg == "-p" || arg == "--pctbars") {
            pctbars = true;

        } else if (arg == "-t" || arg == "--bitcount") {
            bitcount = true;

        } else if (arg == "-f" || arg == "--force") {
            force = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (!parseBlockList2(this, blocks, arg, latest)) {
            return false;

// END_CODE_AUTO
        }
    }

    if (receipt_only && block_only)
        return usage("Please choose either --block_only or --receipt_only, not both. Quitting...");

    if (force) {
        etherlib_init(defaultQuitHandler);
        isRaw = true;
    } else if (eab) {
        isRaw = false;
    }

    if (bits + bars + bitbars + pctbars > 1)
        return usage("Only one of --bits, --bars, --barbits, or --pctbars may be chosen. Quitting...");

    if (!blocks.hasBlocks())
        return usage("You must specify at least one block number or block hash. Quitting...");

    if (block_only && !isRaw)
        return usage("--eab and --block options are not allowed together. Choose one. Quitting...");

    if (receipt_only && !isRaw)
        return usage("--eab and --receipts options are not allowed together. Choose one. Quitting...");

    // Initialize these here (hack alert)
    CBloomReceipt noRemove1; CBloomTrans noRemove2; CBloomBlock noRemove3;
    HIDE_FIELD(CBloomTrans, "hash");
    if (block_only)
        HIDE_FIELD(CBloomBlock, "transactions");

    if (receipt_only)
        HIDE_FIELD(CBloomBlock, "logsBloom");

    if (bitcount || verbose) {
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
    registerOptions(nParams, params);
    optionOn(OPT_RAW);

// BEG_CODE_INIT
    block_only = false;
    receipt_only = false;
    bits = false;
    bars = false;
    bitbars = false;
    pctbars = false;
    bitcount = false;
// END_CODE_INIT

    isRaw        = true; // unusual, but true
    bitBound     = 200;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));

    // Mimics python -m json.tool indenting.
    expContext().spcs = 2;
    expContext().hexNums = false;
    expContext().quoteNums = false;
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
            substitute(substitute(str, "blocks", "<block> [block...]"), "-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{blocks}] is a space-separated list of values, a start-end range, a [{special}], "
                        "or any combination.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if "
                        "configured (see documentation).\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
