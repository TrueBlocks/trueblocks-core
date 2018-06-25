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
CParams params[] = {
    CParams("~address_list", "two or more addresses (0x...), the first is an ERC20 token, balances for the "
                                "rest are reported"),
    CParams("~!block_list",  "an optional list of one or more blocks at which to report balances, defaults "
                                "to 'latest'"),
    CParams("-byAcct",       "consider each address an ERC20 token except the last, whose balance is reported "
                                "for each token"),
    CParams("-data",         "render results as tab delimited data (for example, to build a cap table)"),
    CParams("-list:<fn>",    "an alternative way to specify an address_list, place one address per line in "
                                "the file 'fn'"),
    CParams("-noZero",       "suppress the display of zero balance accounts"),
    CParams("-total",        "if more than one balance is requested, display a total as well"),
    CParams("-info",         "retreive standarized information (name, decimals, totalSupply, etc.) about the token"),
    CParams("",              "Retrieve the token balance(s) for one or more addresses at the given (or "
                                "latest) block(s).\n"),
};
size_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    string_q address_list;
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;
        if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-n" || arg == "--noZero") {
            noZero = true;

        } else if (arg == "-t" || arg == "--total") {
            total = true;

        } else if (arg == "-i" || arg == "--info") {
            tokenInfo = true;

        } else if (arg == "-b" || arg == "--byAcct") {
            byAccount = true;

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--list:")) {

            CFilename fileName(substitute(substitute(arg, "-l:", ""), "--list:", ""));
            if (!fileName.isValid())
                return usage("Not a valid filename: " + orig + ". Quitting...");
            if (!fileExists(fileName.getFullPath()))
                return usage("File " + fileName.relativePath() + " not found. Quitting...");
            string_q contents = asciiFileToString(fileName.getFullPath());
            if (contents.empty())
                return usage("No addresses were found in file " + fileName.relativePath() + ". Quitting...");
            while (!contents.empty()) {
                string_q line = nextTokenClear(contents, '\n');
                if (!isAddress(line))
                    return usage(line + " does not appear to be a valid Ethereum address. Quitting...");
                address_list += line + "|";
            }

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (isHash(arg)) {
            string_q ret = blocks.parseBlockList(arg, latestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }

        } else if (startsWith(arg, "0x")) {

            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
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

    if (asData && total)
        return usage("Totalling is not available when exporting data.");

    address_list = trim(address_list, '|');
    if (countOf(address_list, '|') < 1)
        return usage("You must provide both a token contract and an account. Quitting...");

    if (!byAccount) {
        // first item is ERC20 contract, remainder are accounts
        // token holder1 holder2
        holders = address_list;
        tokens = nextTokenClear(holders, '|');

    } else {
        // last item is account, preceeding are ERC20 contracts
        reverse(address_list);
        // holder token2 token1 - reversed
        tokens = address_list;
        holders = nextTokenClear(tokens, '|');
        reverse(tokens); reverse(holders);
    }

    if (!blocks.hasBlocks()) {
        // use 'latest'
        blocks.numList.push_back(latestBlock);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    tokens = "";
    holders = "";
    asData = false;
    noZero = false;
    byAccount = false;
    total = false;
    tokenInfo = false;
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
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
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return
            substitute(substitute(str, "address_list block_list",
                        "<address> <address> [address...] [block...]"), "-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "[{block_list}] may be space-separated list of values, a start-end range, a [{special}], "
                    "or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured "
                    "(see documentation).\n";
        ret += "If the token contract(s) from which you request balances are not ERC20 compliant, the results "
                    "are undefined.\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
