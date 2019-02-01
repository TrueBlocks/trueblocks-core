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
static COption params[] = {
    COption("~address_list", "one or more addresses (0x...) from which to retrieve balances"),
    COption("~!block_list",  "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),
    COption("-data",         "display results as data (addr <tab> is_contract)"),
    COption("-bytes",        "display the byte code at the address(es)"),
    COption("-nodiff",       "return 'true' if (exactly) two Ethereum addresses have identical code"),
    COption("-whenDep",      "for smart contracts only, return the first block when the address had code"),
        COption("",              "Returns 'true' or 'false' if the given address(es) holds byte code "
                             "(optionally displays the code).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    blknum_t latestBlock = getLatestBlockFromClient();
    Init();
    explode(arguments, command, ' ');
    bool hasExplicitBlocks = false;
    for (auto arg : arguments) {
        if (arg == "-n" || arg == "--nodiff") {
            diff = true;

        } else if (arg == "-b" || arg == "--bytes") {
            showBytes = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-w" || arg == "--whenDep") {
            when = true;

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
            addrs.push_back(toLower(arg));

        } else {

            string_q ret = blocks.parseBlockList(arg, latestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
            hasExplicitBlocks = true;
        }
    }

    if (!blocks.hasBlocks())
        blocks.parseBlockList("latest", latestBlock);

    if (addrs.size() == 0)
        return usage("Please supply valid Ethereum addresses.\n");

    if (diff && (addrs.size() != 2))
        return usage("--nodiff command requires exactly two addresses.\n");

    if (diff && showBytes)
        return usage("Choose only one of --nodiff and --display.\n");

    if (when) {
        if (!nodeHasBalances())
            return usage("--whenDep option requires a full archive node. Quitting...");
        if (hasExplicitBlocks)
            return usage("You may not use the block_list option when using --whenBlock. Quitting...");

        // check to make sure all the addresses have code first
        for (auto addr : addrs) {
            if (!isContractAt(addr))
                return usage("Address " + addr + " is not a smart contract, you may not use --whenDep. Quitting...");
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    addrs.clear();
    diff = false;
    asData = false;
    when = false;
    showBytes = false;
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
        return substitute(str, "address_list block_list", "<address> [address...] [block...]");


    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "[{block_list}] may be a space-separated list of values, a start-end range, a "
                "[{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured.\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
