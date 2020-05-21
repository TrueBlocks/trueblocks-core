/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses to process"),
    COption("list", "l", "", OPT_SWITCH, "list the transactions in the cache (not listing is the default)"),
    COption("no_fix", "n", "", OPT_SWITCH, "do not fix the cache (fixing is the default behaviour)"),
    COption("", "", "", OPT_DESCRIPTION, "Show the contents of an account cache and/or fix it by removing duplicate records."),  // NOLINT
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-n" || arg == "--no_fix") {
            no_fix = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseAddressList2(this, addrs, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (addrs.size() == 0)
        return usage("You must provide at least one filename. Quitting...");

    for (auto addr : addrs) {
        CMonitor monitor(addr, addr);
        monitors.push_back(monitor);
    }

    // Display formatting
    string_q format = "[{BN}]\t[{TX}]";
    configureDisplay("cacheMan", "CAppearance", format);
    HIDE_FIELD(CAppearance, "tc");
    HIDE_FIELD(CAppearance, "addr");
    HIDE_FIELD(CAppearance, "reason");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    // BEG_CODE_INIT
    list = false;
    no_fix = false;
    // END_CODE_INIT

    monitors.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
