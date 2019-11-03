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
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) from which to retrieve balances"),
    COption("blocks", "", "list<blknum>", OPT_POSITIONAL, "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),
    COption("mode", "m", "enum[none|some*|all|balance|nonce|code|storage|deployed|accttype]", OPT_FLAG, "control which state to export"),
    COption("changes", "c", "", OPT_SWITCH, "only report a balance when it changes from one block to the next"),
    COption("no_zero", "n", "", OPT_SWITCH, "suppress the display of zero balance accounts"),
    COption("no_history", "s", "", OPT_HIDDEN | OPT_SWITCH, "for testing only, hide the server's historical state"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve the balance (in wei) for one or more addresses at the given block(s)."),
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    replace(command, "*", " --mode:");
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    string_q mode = "";
    bool no_history = false;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-m:") || startsWith(arg, "--mode:")) {
            if (!confirmEnum("mode", mode, arg))
                return false;

        } else if (arg == "-c" || arg == "--changes") {
            changes = true;

        } else if (arg == "-n" || arg == "--no_zero") {
            no_zero = true;

        } else if (arg == "-s" || arg == "--no_history") {
            no_history = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

            // END_CODE_AUTO
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

        if (mode == "none") modes = ST_NONE;
        if (mode == "balance") modes = ethstate_t(modes|ST_BALANCE);
        if (mode == "nonce") modes = ethstate_t(modes|ST_NONCE);
        if (mode == "code") modes = ethstate_t(modes|ST_CODE);
        if (mode == "storage") modes = ethstate_t(modes|ST_STORAGE);
        if (mode == "deployed") modes = ethstate_t(modes|ST_DEPLOYED);
        if (mode == "accttype") modes = ethstate_t(modes|ST_ACCTTYPE);
        if (mode == "some") modes = ethstate_t(modes|ST_SOME);
        if (mode == "all") modes = ethstate_t(modes|ST_ALL);
    }

    // Data wrangling
    if (!blocks.hasBlocks())
        blocks.numList.push_back(newestBlock);  // use 'latest'

    if (!addrs.size())
        return usage("You must provide at least one Ethereum address.");

    deminimus = str_2_Wei(getGlobalConfig("getState")->getConfigStr("settings", "deminimus", "0"));

    UNHIDE_FIELD(CEthState, "address");
    string_q format = STR_DISPLAY_ETHSTATE;
    if (!(modes & ST_BALANCE))  { replace(format, "\t[{BALANCE}]",  ""); } else { UNHIDE_FIELD(CEthState, "balance"); UNHIDE_FIELD(CEthState, "ether"); }
    if (!(modes & ST_NONCE))    { replace(format, "\t[{NONCE}]",    ""); } else { UNHIDE_FIELD(CEthState, "nonce");    }
    if (!(modes & ST_CODE))     { replace(format, "\t[{CODE}]",     ""); } else { UNHIDE_FIELD(CEthState, "code");     }
    if (!(modes & ST_STORAGE))  { replace(format, "\t[{STORAGE}]",  ""); } else { UNHIDE_FIELD(CEthState, "storage");  }
    if (!(modes & ST_DEPLOYED)) { replace(format, "\t[{DEPLOYED}]", ""); } else { UNHIDE_FIELD(CEthState, "deployed"); }
    if (!(modes & ST_ACCTTYPE)) { replace(format, "\t[{ACCTTYPE}]", ""); } else { UNHIDE_FIELD(CEthState, "accttype"); }

    // Display formatting
    switch (exportFmt) {
    case NONE1:
    case TXT1:
    case CSV1:
        format = getGlobalConfig("getState")->getConfigStr("display", "format", format.empty() ? STR_DISPLAY_ETHSTATE : format);
        manageFields("CEthState:" + cleanFmt(format, exportFmt));
        break;
    case API1:
    case JSON1:
        format = "";
        break;
    }
    if (expContext().asEther)
        format = substitute(format, "{BALANCE}", "{ETHER}");
    if (expContext().asDollars)
        format = substitute(format, "{BALANCE}", "{DOLLARS}");
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (isNoHeader)
        expContext().fmtMap["header"] = "";

    if (!requestsHistory()) // if the user did not request historical state, we can return safely
        return true;

    if ((isTestMode() && no_history) || !nodeHasBalances(false)) {
        // The user requested history, so try to get a different server. Fail silently. The user will be warned in the response
        string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
        string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);
        if ((isTestMode() && no_history) || (rpcProvider == balanceProvider || balanceProvider.empty()))
            return usage("Request asks for historical state, but the RPC server does not have historical state. Quitting...");

        getCurlContext()->baseURL = balanceProvider;
        getCurlContext()->releaseCurl(); // We release the curl context so we can set it to the new context.
        getCurlContext()->getCurl();
        if (!nodeHasBalances(false))
            return usage("Request asks for historical state and has 'balanceServer' set, but that server does not have history state. Quitting...");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    // BEG_CODE_INIT
    changes = false;
    no_zero = false;
    // END_CODE_INIT

    prevBal = 0;
    modes = ST_BALANCE;

    addrs.clear();
    current = "";
    blocks.Init();
    CHistoryOptions::Init();
    newestBlock = oldestBlock = getLastBlock_client();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : CHistoryOptions() {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    first = true;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "addrs blocks", "<address> [address...] [block...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty two characters long.\n";
        ret += "[{blocks}] may be a space-separated list of values, a start-end range, a [{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured (see documentation).\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        ret += "[{balance}] is the default mode. To select a single mode use [{none}] first, followed by that mode.\n";
        ret += "You may specify multiple modes on a single line.\n";
        return ret;
    }
    return str;
}
