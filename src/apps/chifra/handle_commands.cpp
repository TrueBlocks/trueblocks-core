/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_commands(void) {
    ENTER("handle_" + mode);
    nodeRequired();

    if (cmdMap[mode].empty()) {
        LOG_ERR("Should not happen. cmd[", mode, "] is empty.");
        return EXIT_FAILURE;
    }

    // URLs require key/value pairs, command lines don't so we remove unneeded keys
    CStringArray removes = {"--names", "--terms", "--addrs"};
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
map<string, string> cmdMap = {{"slurp", "ethslurp"},        {"collections", "ethNames --collections"},
                              {"names", "ethNames"},        {"tags", "ethNames --tags"},
                              {"abis", "grabABI"},          {"blocks", "getBlock"},
                              {"transactions", "getTrans"}, {"receipts", "getReceipt"},
                              {"logs", "getLogs"},          {"traces", "getTrace"},
                              {"quotes", "ethQuote"},       {"state", "getState"},
                              {"tokens", "getTokenInfo"},   {"when", "whenBlock"},
                              {"where", "whereBlock"},      {"status", "cacheStatus"},
                              {"rm", "acctScrape --rm"},    {"list", "acctScrape"},
                              {"export", "acctExport"},     {"scrape", "blockScrape"},
                              {"dive", "turboDive"}};
