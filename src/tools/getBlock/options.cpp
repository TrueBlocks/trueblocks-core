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
    COption("block_list", "", "list<blknum>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more blocks to retrieve"),
    COption("hash_only", "s", "", OPT_SWITCH, "display only transaction hashes, default is to display full transaction detail"),
    COption("check", "c", "", OPT_SWITCH, "compare results between qblocks and Ethereum node, report differences, if any"),
    COption("addrs", "a", "", OPT_SWITCH, "display all addresses included in the block"),
    COption("uniq", "u", "", OPT_SWITCH, "display only uniq addresses found per block"),
    COption("uniq_tx", "q", "", OPT_SWITCH, "display only uniq addresses found per transaction"),
    COption("count_only", "n", "", OPT_SWITCH, "display counts of appearances (for --addrs, --uniq, or --uniq_tx only)"),
    COption("filter", "i", "<addr>", OPT_FLAG, "useful only for --addrs or --uniq, only display this address in results"),
    COption("report", "r", "", OPT_HIDDEN | OPT_SWITCH, "display the latest blocks at both the node and the cache"),
    COption("force", "o", "", OPT_HIDDEN | OPT_SWITCH, "force a re-write of the block to the cache"),
    COption("quiet", "t", "", OPT_HIDDEN | OPT_SWITCH, "do not print results to screen, used for speed testing and data checking"),
    COption("cache", "e", "", OPT_HIDDEN | OPT_SWITCH, "use the cache for data retrieval"),
    COption("normalize", "z", "", OPT_HIDDEN | OPT_SWITCH, "normalize (remove un-common fields and sort) for comparison with other results (testing)"),
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
    bool report = false;
// END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        // do not collapse
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
        } else if (arg == "-s" || arg == "--hash_only") {
            hash_only = true;

        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (arg == "-a" || arg == "--addrs") {
            addrs = true;

        } else if (arg == "-u" || arg == "--uniq") {
            uniq = true;

        } else if (arg == "-q" || arg == "--uniq_tx") {
            uniq_tx = true;

        } else if (arg == "-n" || arg == "--count_only") {
            count_only = true;

        } else if (arg == "-r" || arg == "--report") {
            report = true;

        } else if (arg == "-o" || arg == "--force") {
            force = true;

        } else if (arg == "-t" || arg == "--quiet") {
            quiet = true;

        } else if (arg == "-e" || arg == "--cache") {
            cache = true;

        } else if (arg == "-z" || arg == "--normalize") {
            normalize = true;

// END_CODE_AUTO
        } else if (startsWith(arg, "-i:") || startsWith(arg, "--filter:")) {
            arg = substitute(substitute(arg, "-i:", ""), "--filter:", "");
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address.\n");
            filters.push_back(str_2_Addr(toLower(arg)));

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

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

    if (check) {
        quiet = true;
        setenv("TEST_MODE", "true", true);
        expContext().spcs = 2;
        expContext().hexNums = true;
        expContext().quoteNums = true;
        RENAME_FIELD(CBlock, "blockNumber", "number");
        RENAME_FIELD(CBlock, "hash", "blockHash");
        GETRUNTIME_CLASS(CBlock)->sortFieldList();
        GETRUNTIME_CLASS(CTransaction)->sortFieldList();
        GETRUNTIME_CLASS(CReceipt)->sortFieldList();
    }

    if (report) {
        cout << scraperStatus(false);
        return false;
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

    if (hash_only) {
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

    showZeroTrace = getGlobalConfig("getBlock")->getConfigBool("display", "showZeroTrace", false);
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW);

// BEG_CODE_INIT
    hash_only = false;
    check = false;
    count_only = false;
    force = false;
    quiet = false;
    cache = false;
    normalize = false;
// END_CODE_INIT

    secsFinal     = (60 * 5);
    filterType    = "";
    addrCounter   = 0;
    format        = "";
    priceBlocks   = false;
    showZeroTrace = false;
    // latest.clear(); // use the same latest block for every run
    filters.clear();
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));

    // Mimics jq indenting
    expContext().spcs = 2;
    expContext().hexNums = false;
    expContext().quoteNums = false;

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
        return substitute(str, "block_list", "<block> [block...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{block_list}] is a space-separated list of values, a start-end range, a [{special}], "
                    "or any combination.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured "
                    "(see documentation).\n";
        ret += "[{special}] blocks are detailed under [{whenBlock --list}].\n";
        return ret;
    }
    return str;
}
