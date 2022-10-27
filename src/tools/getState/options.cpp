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
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) from which to retrieve balances"),  // NOLINT
    COption("blocks", "", "list<blknum>", OPT_POSITIONAL, "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),  // NOLINT
    COption("parts", "p", "list<enum[none|some*|all|balance|nonce|code|proxy|deployed|accttype]>", OPT_FLAG, "control which state to export"),  // NOLINT
    COption("changes", "c", "", OPT_SWITCH, "only report a balance when it changes from one block to the next"),
    COption("no_zero", "n", "", OPT_SWITCH, "suppress the display of zero balance accounts"),
    COption("call", "a", "<string>", OPT_HIDDEN | OPT_FLAG, "a bang-separated string consisting of address!4-byte!bytes"),  // NOLINT
    COption("proxy_for", "r", "<address>", OPT_HIDDEN | OPT_FLAG, "for the --call option only, redirects calls to this implementation"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Retrieve account balance(s) for one or more addresses at given block(s)."),
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
    latestBlock = getLatestBlock_client();
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
        } else if (arg == "-p" || arg == "--parts") {
            return flag_required("parts");

        } else if (arg == "-c" || arg == "--changes") {
            changes = true;

        } else if (arg == "-n" || arg == "--no_zero") {
            no_zero = true;

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--call:")) {
            call = substitute(substitute(arg, "-a:", ""), "--call:", "");
        } else if (arg == "-a" || arg == "--call") {
            return flag_required("call");

        } else if (startsWith(arg, "-r:") || startsWith(arg, "--proxy_for:")) {
            proxy_for = substitute(substitute(arg, "-r:", ""), "--proxy_for:", "");
            if (!isAddress(proxy_for))
                return usage("The provided value (" + proxy_for + ") is not a properly formatted address.");
        } else if (arg == "-r" || arg == "--proxy_for") {
            return flag_required("proxy_for");

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

    if (blocks.empty())
        blocks.numList.push_back(isTestMode() ? byzantiumBlock() : newestBlock);  // use 'latest'

    if (!call.empty()) {
        handle_call();
    } else {
        for (auto part : parts) {
            if (part == "none")
                modeBits = ST_NONE;
            if (part == "balance")
                modeBits = ethstate_t(modeBits | ST_BALANCE);
            if (part == "nonce")
                modeBits = ethstate_t(modeBits | ST_NONCE);
            if (part == "code")
                modeBits = ethstate_t(modeBits | ST_CODE);
            if (part == "proxy")
                modeBits = ethstate_t(modeBits | ST_PROXY);
            if (part == "deployed")
                modeBits = ethstate_t(modeBits | ST_DEPLOYED);
            if (part == "accttype")
                modeBits = ethstate_t(modeBits | ST_ACCTTYPE);
            if (part == "some")
                modeBits = ethstate_t(modeBits | ST_SOME);
            if (part == "all")
                modeBits = ethstate_t(modeBits | ST_ALL);
        }

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
        if (!(modeBits & ST_PROXY)) {
            replace(format, "\t[{PROXY}]", "");
        } else {
            UNHIDE_FIELD(CEthState, "proxy");
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
    }

    if (needsHistory() && !isArchiveNode())
        return usage("This request requires historical balances which your RPC server does not provide.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_RAW);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    changes = false;
    no_zero = false;
    call = "";
    proxy_for = "";
    // END_CODE_INIT

    prevBal = 0;
    modeBits = ST_BALANCE;

    addrs.clear();
    current = "";
    blocks.Init();
    CHistoryOptions::Init();
    newestBlock = oldestBlock = getLatestBlock_client();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : CHistoryOptions() {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("An `address` must start with '0x' and be forty-two characters long.");
    notes.push_back("`Blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("If the queried node does not store historical state, the results are undefined.");
    notes.push_back("`Special` blocks are detailed under `chifra when --list`.");
    notes.push_back("`Balance` is the default mode. To select a single mode use `none` first, followed by that mode.");
    notes.push_back("You may specify multiple `modes` on a single line.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
