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
    COption("addrs2", "", "list<addr2>", OPT_REQUIRED | OPT_POSITIONAL, "two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported"),
    COption("blocks", "", "list<blknum>", OPT_POSITIONAL, "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),
    COption("info", "i", "enum[name|decimals|totalSupply|version|symbol|all]", OPT_FLAG, "retreive information about the token"),
    COption("by_acct", "b", "", OPT_SWITCH, "consider each address an ERC20 token except the last, whose balance is reported for each token"),
    COption("no_zero", "n", "", OPT_SWITCH, "suppress the display of zero balance accounts"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve the token balance(s) for one or more addresses at the given (or latest) block(s)."),
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool isTokenContract(const address_t& addr);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    CAddressArray addrs;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-b" || arg == "--by_acct") {
            by_acct = true;

        } else if (arg == "-n" || arg == "--no_zero") {
            no_zero = true;

            // END_CODE_AUTO
        } else if (startsWith(arg, "-i:") || startsWith(arg, "--info:")) {
            arg = substitute(substitute(arg, "-i:", ""), "--info:", "");
            string_q unused;
            if (!isValidInfo(arg, unused))
                return usage(arg + " does not appear to be a valid tokenInfo option.\n");
            tokenInfo = arg;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (isAddress(arg)) {
            addrs.push_back(toLower(arg));

        } else {

            string_q ret = blocks.parseBlockList(arg, newestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (!blocks.hasBlocks())
        blocks.numList.push_back(newestBlock);  // use 'latest'

    if ((tokenInfo.empty() || tokenInfo == "balanceOf") && addrs.size() < 2)
        return usage("You must provide both a token contract and an account. Quitting...");

    if (!tokenInfo.empty()) {
        // if tokenInfo is not empty, all addresses are tokens
        by_acct = true;
    }

    address_t lastItem;
    for (auto addr : addrs) {
        if (by_acct) {
            // all items but the last are tokens, the last item is the account <token> [tokens...] <holder>
            CTokenState_erc20 watch;
            watch.address = addr;
            watch.abi_spec.loadAbiByAddress(addr);
            watches.push_back(watch);
            lastItem = addr;
        } else {
            // first item is ERC20 contract, remainder are accounts <token> <holder1> [holder2...]
            if (watches.empty()) {
                CTokenState_erc20 watch;
                watch.address = addr;
                watch.abi_spec.loadAbiByAddress(addr);
                watches.push_back(watch);
            } else
                holders.push_back(addr);
        }
    }

    // if tokenInfo is not empty, all addresses are tokens
    if (by_acct && (tokenInfo.empty() || tokenInfo == "balanceOf")) {
        // remove the last one and push it on the holders array
        watches.pop_back();
        holders.push_back(lastItem);
    }

    for (auto watch : watches) {
        if (!isTokenContract(watch.address))
            return usage("Address '" + watch.address + "' does not appear to be a token smart contract. Quitting...");
    }

    if ((!isTestMode() && !requestsHistory()) || nodeHasBalances(true))
        return true;

    // We need history, so try to get a different server. Fail silently. The user will be warned in the response
    string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
    string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);
    if (rpcProvider == balanceProvider || balanceProvider.empty())
        return true;

    // We release the curl context so we can set it to the new context.
    getCurlContext()->baseURL = balanceProvider;
    getCurlContext()->releaseCurl();
    getCurlContext()->getCurl();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW);
    optionOff(OPT_FMT);

    // BEG_CODE_INIT
    by_acct = false;
    no_zero = false;
    // END_CODE_INIT

    watches.clear();
    holders.clear();

    optionOff(OPT_DOLLARS|OPT_ETHER);
    tokenInfo = "";
    blocks.Init();
    CHistoryOptions::Init();
    newestBlock = oldestBlock = getLastBlock_client();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : CHistoryOptions() {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    notes2.push_back("`addresses` must start with '0x' and be forty two characters long.");
    notes2.push_back("`blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.");
    notes2.push_back("This tool retrieves information from the local node or rpcProvider if configured (see documentation).");
    notes2.push_back("If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.");
    notes2.push_back("If the queried node does not store historical state, the results are undefined.");
    notes2.push_back("`special` blocks are detailed under `whenBlock --list`.");
    // END_CODE_NOTES
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool isTokenContract(const address_t& addr) {
    if (!isContractAt(addr))
        return false;
    return true;
}
