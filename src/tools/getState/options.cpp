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
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) from which to retrieve balances"),  // NOLINT
    COption("blocks", "", "list<blknum>", OPT_POSITIONAL, "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),  // NOLINT
    COption("parts", "p", "list<enum[none|some*|all|balance|nonce|code|storage|deployed|accttype]>", OPT_FLAG, "control which state to export"),  // NOLINT
    COption("changes", "c", "", OPT_SWITCH, "only report a balance when it changes from one block to the next"),
    COption("no_zero", "n", "", OPT_SWITCH, "suppress the display of zero balance accounts"),
    COption("call", "a", "<string>", OPT_HIDDEN | OPT_FLAG, "a bang-separated string consisting of address!4-byte!bytes"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Retrieve the balance of one or more address at the given block(s)."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    replace(command, "*", " --mode:");
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray parts;
    // END_CODE_LOCAL_INIT

    Init();
    latestBlock = getBlockProgress(BP_CLIENT).client;
    blknum_t latest = latestBlock;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-p:") || startsWith(arg, "--parts:")) {
            string_q parts_tmp;
            if (!confirmEnum("parts", parts_tmp, arg))
                return false;
            parts.push_back(parts_tmp);

        } else if (arg == "-c" || arg == "--changes") {
            changes = true;

        } else if (arg == "-n" || arg == "--no_zero") {
            no_zero = true;

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--call:")) {
            call = substitute(substitute(arg, "-a:", ""), "--call:", "");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
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

    // BEG_DEBUG_DISPLAY
    // LOG_TEST("addrs", addrs, (addrs == NOPOS));
    // LOG_TEST("blocks", blocks, (blocks == NOPOS));
    // LOG_TEST("parts", parts, (parts == ""));
    LOG_TEST_BOOL("changes", changes);
    LOG_TEST_BOOL("no_zero", no_zero);
    LOG_TEST("call", call, (call == ""));
    // END_DEBUG_DISPLAY

    if (Mocked())
        return false;

    // Data wrangling
    if (!blocks.hasBlocks())
        blocks.numList.push_back(newestBlock);  // use 'latest'

    if (!call.empty() && !parts.empty())
        return usage("The --parts option is not available with the --call option.");

    if (!call.empty())
        return handle_call();

    if (!addrs.size())
        return usage("You must provide at least one Ethereum address.");

    for (auto part : parts) {
        if (part == "none")
            modeBits = ST_NONE;
        if (part == "balance")
            modeBits = ethstate_t(modeBits | ST_BALANCE);
        if (part == "nonce")
            modeBits = ethstate_t(modeBits | ST_NONCE);
        if (part == "code")
            modeBits = ethstate_t(modeBits | ST_CODE);
        if (part == "storage")
            modeBits = ethstate_t(modeBits | ST_STORAGE);
        if (part == "deployed")
            modeBits = ethstate_t(modeBits | ST_DEPLOYED);
        if (part == "accttype")
            modeBits = ethstate_t(modeBits | ST_ACCTTYPE);
        if (part == "some")
            modeBits = ethstate_t(modeBits | ST_SOME);
        if (part == "all")
            modeBits = ethstate_t(modeBits | ST_ALL);
    }

    deminimus = str_2_Wei(getGlobalConfig("getState")->getConfigStr("settings", "deminimus", "0"));

    UNHIDE_FIELD(CEthState, "address");
    string_q format = STR_DISPLAY_ETHSTATE;
    if (!(modeBits & ST_BALANCE)) {
        replace(format, "\t[{BALANCE}]", "");
    } else {
        UNHIDE_FIELD(CEthState, "balance");
        UNHIDE_FIELD(CEthState, "ether");
    }
    if (!(modeBits & ST_NONCE)) {
        replace(format, "\t[{NONCE}]", "");
    } else {
        UNHIDE_FIELD(CEthState, "nonce");
    }
    if (!(modeBits & ST_CODE)) {
        replace(format, "\t[{CODE}]", "");
    } else {
        UNHIDE_FIELD(CEthState, "code");
    }
    if (!(modeBits & ST_STORAGE)) {
        replace(format, "\t[{STORAGE}]", "");
    } else {
        UNHIDE_FIELD(CEthState, "storage");
    }
    if (!(modeBits & ST_DEPLOYED)) {
        replace(format, "\t[{DEPLOYED}]", "");
    } else {
        UNHIDE_FIELD(CEthState, "deployed");
    }
    if (!(modeBits & ST_ACCTTYPE)) {
        replace(format, "\t[{ACCTTYPE}]", "");
    } else {
        UNHIDE_FIELD(CEthState, "accttype");
    }

    // Display formatting
    configureDisplay("getState", "CEthState", format.empty() ? STR_DISPLAY_ETHSTATE : format);

    if (!requestsHistory())  // if the user did not request historical state, we can return safely
        return true;

    if (!nodeHasBalances(false)) {
        // User requested history, but we have none. Try something else if available
        string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
        string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);
        if (rpcProvider == balanceProvider || balanceProvider.empty())
            return usage("The RPC server does not have historical state.");

        setRpcProvider(balanceProvider);
        if (!nodeHasBalances(false))
            return usage("balanceServer set but it does not have historical state.");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW);

    // BEG_CODE_INIT
    changes = false;
    no_zero = false;
    call = "";
    // END_CODE_INIT

    prevBal = 0;
    modeBits = ST_BALANCE;

    addrs.clear();
    current = "";
    blocks.Init();
    CHistoryOptions::Init();
    newestBlock = oldestBlock = getBlockProgress(BP_CLIENT).client;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : CHistoryOptions() {
    Init();
    first = true;
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`addresses` must start with '0x' and be forty two characters long.");
    notes.push_back("`blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("If the queried node does not store historical state, the results are undefined.");
    notes.push_back("`special` blocks are detailed under `whenBlock --list`.");
    notes.push_back("`balance` is the default mode. To select a single mode use `none` first, followed by that mode.");
    notes.push_back("You may specify multiple `modes` on a single line.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::Mocked(void) {
    return false;
}
