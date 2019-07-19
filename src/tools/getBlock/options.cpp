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
    COption("~block_list",         "a space-separated list of one or more blocks to retrieve"),
    COption("-hash_o(n)ly",        "display only transaction hashes, default is to display full transaction detail"),
    COption("-check",              "compare results between qblocks and Ethereum node, report differences, if any"),
    COption("-addrs",              "display all addresses included in the block"),
    COption("-uniq",               "display only uniq addresses found per block"),
    COption("-uni(q)Tx",           "display only uniq addresses found per transaction"),
    COption("-nu(m)ber",           "display address counts (alterntively --addrCnt, --uniqTxCnt, or --uniqCnt)"),
    COption("-fi(l)ter:<addr>",    "useful only for --addrs or --uniq, only display this address in results"),
    COption("@latest",             "display the latest blocks at both the node and the cache"),
    COption("@f(o)rce",            "force a re-write of the block to the cache"),
    COption("@quiet",              "do not print results to screen, used for speed testing and data checking"),
    COption("@source:[c|r]",       "either :c(a)che or :(r)aw, source for data retrival. (shortcuts -c = qblocks, -r = node)"),
    COption("@fields:[a|m|c|r]",   "either :(a)ll, (m)ini, (c)ache or :(r)aw; which fields to include in output (all is default)"),
    COption("@normalize",          "normalize (remove un-common fields and sort) for comparison with other results (testing)"),
    COption("",                    "Returns block(s) from local cache or directly from a running node.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        // shortcuts
        if (arg == "-r" || arg == "--raw")   { arg = "--source:raw";   }
        if (arg == "-d" || arg == "--cache") { arg = "--source:cache"; }

        // do not collapse
        if (arg == "-c" || arg == "--check") {
            setenv("TEST_MODE", "true", true);
            isCheck = true;
            quiet++;  // if both --check and --quiet are present, be very quiet...
            expContext().spcs = 2;
            expContext().hexNums = true;
            expContext().quoteNums = true;
            RENAME_FIELD(CBlock, "blockNumber", "number");
            RENAME_FIELD(CBlock, "hash", "blockHash");
            GETRUNTIME_CLASS(CBlock)->sortFieldList();
            GETRUNTIME_CLASS(CTransaction)->sortFieldList();
            GETRUNTIME_CLASS(CReceipt)->sortFieldList();

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--filter:")) {
            string_q orig = arg;
            arg = substitute(substitute(arg, "-l:", ""), "--filter:", "");
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address.\n");
            filters.push_back(str_2_Addr(toLower(arg)));

        } else if (arg == "-o" || arg == "--force") {
            etherlib_init(defaultQuitHandler);
            force = true;

        } else if (arg == "--normalize") {
            normalize = true;

        } else if (arg == "-l" || arg == "--latest") {
            cout << scraperStatus(false);
            return false;

        } else if (startsWith(arg, "--source:")) {
            string_q mode = substitute(arg, "--source:", "");
            if (mode == "r" || mode == "raw") {
                isRaw = true;

            } else if (mode == "c" || mode == "cache") {
                isCache = true;

            } else {
                return usage("Invalide source. Must be either '(r)aw' or '(c)ache'. Quitting...");
            }

        } else if (arg == "-n" || arg == "--hash_only") {
            hashes = true;

        } else if (arg == "-a" || arg == "--addrs") {
            filterType = "addrs";

        } else if (arg == "--addrsCnt") {
            filterType = "addrs";
            counting = true;

        } else if (arg == "-u" || arg == "--uniq") {
            filterType = "uniq";

        } else if (arg == "--uniqCnt") {
            filterType = "uniq";
            counting = true;

        } else if (arg == "-q" || arg == "--uniqTx") {
            filterType = "uniqTx";

        } else if (arg == "--uniqTxCnt") {
            filterType = "uniqTx";
            counting = true;

        } else if (arg == "-m" || arg == "--number") {
            counting = true;

        } else if (arg == "-e" || arg == "--traces") {
            traces = true;

        } else if (arg == "--quiet") {
            quiet++;  // if both --check and --quiet are present, be very quiet...

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fields:")) {
            string_q mode = substitute(substitute(arg, "-f:", ""), "--fields:", "");

            if (mode == "a" || mode == "all") {
                GETRUNTIME_CLASS(CBlock)->showAllFields();
                GETRUNTIME_CLASS(CTransaction)->showAllFields();
                GETRUNTIME_CLASS(CReceipt)->showAllFields();

            } else if (mode == "m" || mode == "mini") {
                GETRUNTIME_CLASS(CBlock)->hideAllFields();
                GETRUNTIME_CLASS(CTransaction)->hideAllFields();
                GETRUNTIME_CLASS(CReceipt)->hideAllFields();
                UNHIDE_FIELD(CBlock, "blockNumber");
                UNHIDE_FIELD(CBlock, "timestamp");
                UNHIDE_FIELD(CBlock, "transactions");
                UNHIDE_FIELD(CTransaction, "receipt");
                UNHIDE_FIELD(CTransaction, "transactionIndex");
                UNHIDE_FIELD(CTransaction, "gasPrice");
                UNHIDE_FIELD(CTransaction, "gas");
                UNHIDE_FIELD(CTransaction, "isError");
                UNHIDE_FIELD(CTransaction, "from");
                UNHIDE_FIELD(CTransaction, "to");
                UNHIDE_FIELD(CTransaction, "value");
                UNHIDE_FIELD(CReceipt, "gasUsed");

            } else if (mode == "r" || mode == "raw") {
            } else if (mode == "c" || mode == "cache") {
                GETRUNTIME_CLASS(CBlock)->hideAllFields();
                GETRUNTIME_CLASS(CTransaction)->hideAllFields();
                GETRUNTIME_CLASS(CReceipt)->hideAllFields();
                UNHIDE_FIELD(CBlock, "blockNumber");
                UNHIDE_FIELD(CBlock, "timestamp");
                UNHIDE_FIELD(CBlock, "transactions");
                UNHIDE_FIELD(CTransaction, "receipt");
                UNHIDE_FIELD(CTransaction, "transactionIndex");
                UNHIDE_FIELD(CTransaction, "gasPrice");
                UNHIDE_FIELD(CTransaction, "gas");
                UNHIDE_FIELD(CTransaction, "isError");
                UNHIDE_FIELD(CTransaction, "from");
                UNHIDE_FIELD(CTransaction, "to");
                UNHIDE_FIELD(CTransaction, "value");
                UNHIDE_FIELD(CReceipt, "gasUsed");
            }

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

    if (expContext().isParity) {
        HIDE_FIELD(CTransaction, "cumulativeGasUsed");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CTransaction, "timestamp");
        RENAME_FIELD(CBlock, "blockNumber", "number");
        GETRUNTIME_CLASS(CBlock)->sortFieldList();
    }

    if (hashes) {
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

    if (counting && (filterType.empty()))
        return usage("--number option is only available with either --addrs or --uniq. Quitting...");

    if (!filterType.empty() && force)
        return usage("The --force option is not available when using one of the address options. Quitting...");

    if (!blocks.hasBlocks())
        return usage("You must specify at least one block. Quitting...");

    format = getGlobalConfig("getBlock")->getDisplayStr(false, "");
    if (contains(format, "{PRICE:CLOSE}")) {
//        priceBlocks = true;
    }

    showZeroTrace = getGlobalConfig("getBlock")->getConfigBool("display", "showZeroTrace", false);
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    optionOn(OPT_RAW);
    registerOptions(nParams, params);

    isCheck       = false;
    isCache       = false;
    hashes        = false;
    filterType    = "";
    counting      = false;
    addrCnt       = 0;
    traces        = false;
    force         = false;
    normalize     = false;
    quiet         = 0;  // quiet has levels
    format        = "";
    priceBlocks   = false;
    showZeroTrace = false;
    // latest.clear(); // use the same latest block for every run
    filters.clear();
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Mimics jq indenting
    expContext().spcs = 2;
    expContext().hexNums = false;
    expContext().quoteNums = false;

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
bool COptions::isMulti(void) const {
    return api_mode || ((blocks.stop - blocks.start) > 1 || blocks.hashList.size() > 1 || blocks.numList.size() > 1);
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
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
