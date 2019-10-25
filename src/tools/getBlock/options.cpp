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
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("blocks", "", "list<blknum>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more blocks to retrieve"),
    COption("hashes_only", "s", "", OPT_SWITCH, "display only transaction hashes, default is to display full transaction detail"),
    COption("addrs", "a", "", OPT_SWITCH, "display all addresses included in the block"),
    COption("uniq", "u", "", OPT_SWITCH, "display only uniq addresses found per block"),
    COption("uniq_tx", "n", "", OPT_SWITCH, "display only uniq addresses found per transaction"),
    COption("count_only", "o", "", OPT_SWITCH, "display counts of appearances (for --addrs, --uniq, or --uniq_tx only)"),
    COption("force", "e", "", OPT_HIDDEN | OPT_SWITCH, "force a re-write of the block to the cache"),
    COption("", "", "", OPT_DESCRIPTION, "Returns block(s) from local cache or directly from a running node."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

// BEG_CODE_LOCAL_INIT
    bool addrs = false;
    bool uniq = false;
    bool uniq_tx = false;
// END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        // do not collapse
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
        } else if (arg == "-s" || arg == "--hashes_only") {
            hashes_only = true;

        } else if (arg == "-a" || arg == "--addrs") {
            addrs = true;

        } else if (arg == "-u" || arg == "--uniq") {
            uniq = true;

        } else if (arg == "-n" || arg == "--uniq_tx") {
            uniq_tx = true;

        } else if (arg == "-o" || arg == "--count_only") {
            count_only = true;

        } else if (arg == "-e" || arg == "--force") {
            force = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

// END_CODE_AUTO
        } else {

            string_q ret = blocks.parseBlockList(arg, latest.blockNumber);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (force)
        etherlib_init(defaultQuitHandler);

    if (expContext().isParity) {
        HIDE_FIELD(CTransaction, "cumulativeGasUsed");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CTransaction, "timestamp");
        RENAME_FIELD(CBlock, "blockNumber", "number");
        GETRUNTIME_CLASS(CBlock)->sortFieldList();
    }

    if (hashes_only) {
        HIDE_FIELD(CTransaction, "blockHash");
        HIDE_FIELD(CTransaction, "blockNumber");
        HIDE_FIELD(CTransaction, "transactionIndex");
        HIDE_FIELD(CTransaction, "nonce");
        HIDE_FIELD(CTransaction, "timestamp");
        HIDE_FIELD(CTransaction, "from");
        HIDE_FIELD(CTransaction, "to");
        HIDE_FIELD(CTransaction, "value");
        HIDE_FIELD(CTransaction, "gas");
        HIDE_FIELD(CTransaction, "gasPrice");
        HIDE_FIELD(CTransaction, "input");
        HIDE_FIELD(CTransaction, "isError");
        HIDE_FIELD(CTransaction, "receipt");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CReceipt,     "contractAddress");
        HIDE_FIELD(CReceipt,     "gasUsed");
        HIDE_FIELD(CReceipt,     "logs");
        HIDE_FIELD(CReceipt,     "status");
        HIDE_FIELD(CBlock,       "gasLimit");
        HIDE_FIELD(CBlock,       "gasUsed");
        HIDE_FIELD(CBlock,       "parentHash");
        HIDE_FIELD(CBlock,       "miner");
        HIDE_FIELD(CBlock,       "difficulty");
        HIDE_FIELD(CBlock,       "price");
        HIDE_FIELD(CBlock,       "finalized");
    }

    filterType = (uniq_tx ? "uniq_tx" : (uniq ? "uniq" : (addrs ? "addrs" : "")));
    if (count_only && (filterType.empty()))
        return usage("--count_only option is only available with either --addrs, --uniq, or --uniq_tx. Quitting...");

    if (!filterType.empty() && force)
        return usage("The --force option is not available when using one of the address options. Quitting...");

    if (!blocks.hasBlocks())
        return usage("You must specify at least one block. Quitting...");

    format = getGlobalConfig("getBlock")->getConfigStr("display", "format", STR_DISPLAY_BLOCK);

    secsFinal = (timestamp_t)getGlobalConfig("getBlock")->getConfigInt("settings", "secs_when_final", (uint64_t)secsFinal);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW);

// BEG_CODE_INIT
    hashes_only = false;
    count_only = false;
    force = false;
// END_CODE_INIT

    filterType    = "";
    format        = "";
    secsFinal     = (60 * 5);
    addrCounter   = 0;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::isMulti(void) const {
    return isApiMode() || ((blocks.stop - blocks.start) > 1 || blocks.hashList.size() > 1 || blocks.numList.size() > 1);
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "blocks", "<block> [block...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{blocks}] is a space-separated list of values, a start-end range, a [{special}], "
                    "or any combination.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured "
                    "(see documentation).\n";
        ret += "[{special}] blocks are detailed under [{whenBlock --list}].\n";
        return ret;
    }
    return str;
}
