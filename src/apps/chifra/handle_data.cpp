/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

extern string_q toPrintable_force(const string_q& inHex);
//------------------------------------------------------------------------------------------------
bool COptions::handle_data(void) {

    ENTER4("handle_" + mode);
    LOG4("tool_flags: " + tool_flags);
    nodeNotRequired();

    if (!contains(tool_flags, "--edit")) // weird cleanup of --edit/edit confusion
        tool_flags = substitute(tool_flags, "edit", "--edit");

    string_q addr_list;
    for (auto addr : addrs)
        addr_list += (addr + " ");

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
        os << "getAccounts " << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--blocks")) {
        replaceAll(tool_flags, "--blocks", "");
        os << "getBlock " << (api_mode ? substitute(tool_flags, ",", " ") : tool_flags) << " ; ";

    } else if (contains(tool_flags, "--transactions")) {
        replaceAll(tool_flags, "--transactions", "");
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

    } else if (contains(tool_flags, "--message")) {
        tool_flags = trim(substitute(substitute(substitute(tool_flags, "--message", ""), "--chars", ""), "--bytes", ""));
        if (contains(tool_flags, "0x"))
            cout << "message: " << toPrintable_force(tool_flags);
        else
            cout << "message: " << chr_2_HexStr(tool_flags);
        return true;

    } else {
        EXIT_FAIL("Invalid option: data " + tool_flags + " " + addr_list);
    }

    LOG4("command: " + os.str());
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    EXIT_NOMSG4(true);
}

//----------------------------------------------------------------------------
// If we can reasonably convert this byte input into a string, do so, otherwise bail out
string_q toPrintable_force(const string_q& inHex) {
    string_q res;
    string_q in = substitute(inHex, "0x", "");
    while (!in.empty() && in.size() >= 2) {
        string_q nibble = extract(in, 0, 2);
        in = extract(in, 2);
        char ch = (char)hex_2_Ascii(nibble[0], nibble[1]);  // NOLINT
        if (isprint(ch) || isspace(ch))
            res += ch;
        else
            res += ".";
    }
    return res;
}
