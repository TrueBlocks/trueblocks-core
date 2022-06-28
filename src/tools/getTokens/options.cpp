/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs2", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported"),  // NOLINT
    COption("blocks", "", "list<blknum>", OPT_POSITIONAL, "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),  // NOLINT
    COption("parts", "p", "list<enum[name|symbol|decimals|totalSupply|version|none|all*]>", OPT_FLAG, "which parts of the token information to retrieve"),  // NOLINT
    COption("by_acct", "b", "", OPT_SWITCH, "consider each address an ERC20 token except the last, whose balance is reported for each token"),  // NOLINT
    COption("no_zero", "n", "", OPT_SWITCH, "suppress the display of zero balance accounts"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve token balance(s) for one or more addresses at given block(s)."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray parts;
    // END_CODE_LOCAL_INIT

    Init();
    blknum_t latest = getLatestBlock_client();
    latestBlock = latest;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-p:") || startsWith(arg, "--parts:")) {
            string_q parts_tmp;
            if (!confirmEnum("parts", parts_tmp, arg))
                return false;
            parts.push_back(parts_tmp);
        } else if (arg == "-p" || arg == "--parts") {
            return flag_required("parts");

        } else if (arg == "-b" || arg == "--by_acct") {
            by_acct = true;

        } else if (arg == "-n" || arg == "--no_zero") {
            no_zero = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else if (isAddress(arg)) {
            if (!parseAddressList(this, addrs, arg))
                return false;

        } else {
            if (!parseBlockList2(this, blocks, arg, latest))
                return false;

            // END_CODE_AUTO
        }
    }

    if (!loadNames())
        return usage("Could not load names database.");

    bool userBlocks = true;
    if (blocks.empty()) {
        blocks.numList.push_back(newestBlock);  // use 'latest'
        userBlocks = false;
    }

    if (parts.empty() && addrs.size() < 2)
        return usage("Use either --parts or provide at least one token and one other account.");

    string_q format = STR_DISPLAY_TOKENBALANCERECORD;
    if (parts.size() > 0) {
        for (auto part : parts) {
            if (part == "none")
                modeBits = TOK_NONE;
            if (part == "name")
                modeBits = tokstate_t(modeBits | TOK_NAME);
            if (part == "address")
                modeBits = tokstate_t(modeBits | TOK_ADDRESS);
            if (part == "decimals")
                modeBits = tokstate_t(modeBits | TOK_DECIMALS);
            if (part == "totalSupply")
                modeBits = tokstate_t(modeBits | TOK_TOTALSUPPLY);
            if (part == "symbol")
                modeBits = tokstate_t(modeBits | TOK_SYMBOL);
            if (part == "all")
                modeBits = tokstate_t(modeBits | TOK_PARTS);
        }
    } else {
        modeBits = TOK_BALRECORD;
    }

    // We're going to turn some of these fields back on
    manageFields("CAccountName:isCustom,isPrefund,tags,name,symbol,source,decimals,description", false);
    manageFields("CMonitor:all", false);
    manageFields("CTokenBalanceRecord:all", false);

    UNHIDE_FIELD(CMonitor, "address");
    if (userBlocks) {
        UNHIDE_FIELD(CTokenBalanceRecord, "blockNumber");
    } else {
        replace(format, "[{BLOCKNUMBER}]", "");
    }

    if (!(modeBits & TOK_NAME)) {
        replace(format, "[{NAME}]", "");
    } else {
        UNHIDE_FIELD(CAccountName, "name");
    }

    if (!(modeBits & TOK_DECIMALS)) {
        replace(format, "[{DECIMALS}]", "");
    } else {
        UNHIDE_FIELD(CAccountName, "decimals");
    }

    if (!(modeBits & TOK_TOTALSUPPLY)) {
        replace(format, "[{TOTALSUPPLY}]", "");
    } else {
        UNHIDE_FIELD(CTokenBalanceRecord, "totalSupply");
    }

    if (!(modeBits & TOK_SYMBOL)) {
        replace(format, "[{SYMBOL}]", "");
    } else {
        UNHIDE_FIELD(CAccountName, "symbol");
    }

    if (!(modeBits & TOK_HOLDER)) {
        replace(format, "[{HOLDER}]", "");
    } else {
        UNHIDE_FIELD(CTokenBalanceRecord, "holder");
    }

    if (!(modeBits & TOK_BALANCE)) {
        replace(format, "[{BALANCE}]", "");
    } else {
        UNHIDE_FIELD(CTokenBalanceRecord, "balance");
    }

    replaceAll(format, "\t\t", "\t");
    format = trim(format, '\t');

    // Display formatting
    configureDisplay("getTokens", "CTokenBalanceRecord", format);

    if (modeBits != TOK_BALRECORD) {
        holders.push_back("0x0");  // dummy, so the main function loops
        for (auto addr : addrs)
            tokens.push_back(addr);

    } else if (by_acct) {
        // All user-provided addresses are assumed to be tokens, except the last one which is the holder
        holders.push_back(addrs[addrs.size() - 1]);
        CAddressArray::iterator it;
        it = prev(addrs.end());
        addrs.erase(it);
        for (auto addr : addrs) {
            if (!isContractAt(addr, latestBlock))
                errors.push_back("Address '" + addr + "' is not a token contract.");
            else
                tokens.push_back(addr);
        }
    } else {
        // The first user-provided address is the token, the remainder are holders
        bool first = true;
        for (auto addr : addrs) {
            if (first) {
                if (!isContractAt(addr, latestBlock))
                    return usage("Address '" + addr +
                                 "' is not a token contract. You must provide at least one valid token address.");
                else
                    tokens.push_back(addr);
            } else {
                holders.push_back(addr);
            }
            first = false;
        }
    }

    if (tokens.size() == 0)
        return usage("You must provide at least one valid token address.");

    abi_spec.loadAbisFromKnown(true);

    if (!getTimestampAt(1))  // loads the timestamp file and returns non-zero on success
        return usage("Could not open timestamp file.");

    if (isTestMode())
        return true;

    if (needsHistory() && !isArchiveNode())
        return usage("This request requires historical balances which your RPC server does not provide.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_RAW, OPT_DOLLARS | OPT_ETHER);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    by_acct = false;
    no_zero = false;
    // END_CODE_INIT

    // TODO(tjayrush): This should be inside of CODE_INIT as it's part of the auto-generated data
    addrs.clear();

    tokens.clear();
    holders.clear();
    modeBits = TOK_NONE;

    blocks.Init();
    CHistoryOptions::Init();
    newestBlock = oldestBlock = getLatestBlock_client();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : CHistoryOptions() {
    CTokenBalanceRecord::registerClass();

    Init();
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("An `address` must start with '0x' and be forty-two characters long.");
    notes.push_back("`Blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.");  // NOLINT
    notes.push_back("If the queried node does not store historical state, the results are undefined.");
    notes.push_back("`Special` blocks are detailed under `chifra when --list`.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
