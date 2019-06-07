/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_data(void) {

    ENTER4("handle_" + mode);
    LOG4("tool_flags: " + tool_flags);
    nodeNotRequired();

    if (!contains(tool_flags, "--edit")) // weird cleanup of --edit/edit confusion
        tool_flags = substitute(tool_flags, "edit", "--edit");

    ostringstream os;
    if (contains(tool_flags, "--when")) {
        replaceAll(tool_flags, "--when", "");
        os << "whenBlock --list --data " << tool_flags << (api_mode ? " --fmt json" : "") << " ; "; // order matters, last in wins

    } else if (contains(tool_flags, "--abi")) {
        replaceAll(tool_flags, "--abi", "");
        for (auto addr : addrs)
            os << "grabABI " << addr << " " << (api_mode ? substitute(tool_flags, ",", " ") + " --encode --data" : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--balance") || contains(tool_flags, "--code") || contains(tool_flags, "--nonce")) {
        replaceAll(tool_flags, "--balance", "");
        replaceAll(tool_flags, "--code", "");
        replaceAll(tool_flags, "--nonce", "");
        for (auto addr : addrs)
            os << "getState " << addr << " " << (api_mode ? substitute(tool_flags, ",", " ") + " " : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--accounts")) {
        replaceAll(tool_flags, "--accounts", "");
        for (auto addr : addrs)
           os << "getState " << addr << " " << (api_mode ? substitute(tool_flags, ",", " ") + " --mode some" : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--blocks")) {
        replaceAll(tool_flags, "--blocks", "");
        os << "getBlock " << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--trans")) {
        replaceAll(tool_flags, "--trans", "");
        os << "getTrans " << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--receipts")) {
        replaceAll(tool_flags, "--receipts", "");
        os << "getReceipt " << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--logs")) {
        replaceAll(tool_flags, "--logs", "");
        os << "getLogs "  << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--traces")) {
        replaceAll(tool_flags, "--traces", "");
        os << "getTrace " << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--slurp")) {
        replaceAll(tool_flags, "--slurp", "");
        return handle_slurp();

    } else if (contains(tool_flags, "--quotes")) {
        replaceAll(tool_flags, "--quotes", "");
        return handle_quotes();

    } else {
        EXIT_FAIL("Invalid option: " + tool_flags);

    }

    LOG4("command: " + os.str());
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    EXIT_NOMSG4(true);
}
