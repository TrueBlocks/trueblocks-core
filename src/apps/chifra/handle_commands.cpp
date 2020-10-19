/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
extern map<string, string> cmdMap;
bool COptions::handle_commands(void) {
    ENTER("handle_" + mode);
    nodeRequired();

    if (cmdMap[mode].empty()) {
        LOG_ERR("Should not happen. cmd[", mode, "] is empty.");
        return EXIT_FAILURE;
    }

    if (mode == "rm" && contains(tool_flags, "--remove"))  // order matters
        setenv("TB_EDITCMD", "remove", true);

    CStringArray removes = {"--names", "--terms", "--addrs", "--remove"};
    for (auto remove : removes)
        if (remove != "--addrs" || mode != "blocks")
            tool_flags = substitute(tool_flags, remove, "");

    ostringstream os;
    os << cmdMap[mode] << " " << tool_flags;
    for (auto addr : addrs)
        os << " " << addr;

    // clang-format off
    LOG_CALL(os.str());
    return system(os.str().c_str());
    // clang-format on
}

//------------------------------------------------------------------------------------------------
map<string, string> cmdMap = {
    {"slurp", "ethSlurp"},        {"collections", "ethNames --collections"},
    {"names", "ethNames"},        {"tags", "ethNames --tags"},
    {"abis", "grabABI"},          {"blocks", "getBlock"},
    {"transactions", "getTrans"}, {"receipts", "getReceipt"},
    {"logs", "getLogs"},          {"traces", "getTrace"},
    {"quotes", "ethQuote"},       {"state", "getState"},
    {"tokens", "getTokenInfo"},   {"when", "whenBlock"},
    {"where", "whereBlock"},      {"status", "cacheStatus"},
    {"rm", "acctScrape --rm"},
};
