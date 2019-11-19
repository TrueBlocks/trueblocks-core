/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern string_q toPrintable_force(const string_q& inHex);
//------------------------------------------------------------------------------------------------
bool COptions::handle_data(void) {
    ENTER8("handle_" + mode);
    nodeNotRequired();

    if (!contains(tool_flags, "--edit"))  // weird cleanup of --edit/edit confusion
        tool_flags = substitute(tool_flags, "edit", "--edit");

    string_q addrList;
    for (auto addr : addrs)
        addrList += (" " + addr);

    ostringstream os;
    if (contains(tool_flags, "--when")) {
        replaceAll(tool_flags, "--when", "");
        os << "whenBlock --list --data " << tool_flags
           << (isApiMode() ? " --fmt json" : "");  // order matters, last in wins

    } else if (contains(tool_flags, "--abi")) {
        replaceAll(tool_flags, "--abi", "");
        if (addrs.size() == 0 && !contains(tool_flags, "help"))
            return usage("Input" + (tool_flags.empty() ? "" : (" (" + trim(tool_flags) + ")")) +
                         " does not include a valid Ethereum address. Quitting...");
        os << "grabABI " << (isApiMode() ? substitute(tool_flags, ",", " ") + " " : tool_flags) << addrList;

    } else if (contains(tool_flags, "--state")) {
        replaceAll(tool_flags, "--state", "");
        if (addrs.size() == 0 && !contains(tool_flags, "help"))
            return usage("Input" + (tool_flags.empty() ? "" : (" (" + trim(tool_flags) + ")")) +
                         " does not include a valid Ethereum address. Quitting...");

        replaceAll(tool_flags, "--balance", "--mode balance");
        replaceAll(tool_flags, "--code", "--mode code");
        replaceAll(tool_flags, "--nonce", "--mode nonce");
        os << "getState " << (isApiMode() ? substitute(tool_flags, ",", " ") + " " : tool_flags) << addrList;

    } else if (contains(tool_flags, "--names")) {
        replaceAll(tool_flags, "--names", "");
        os << "ethNames " << (isApiMode() ? substitute(tool_flags, ",", " ") : tool_flags) << addrList;

    } else if (contains(tool_flags, "--blocks")) {
        replaceAll(tool_flags, "--blocks", "");
        os << "getBlock " << (isApiMode() ? substitute(tool_flags, ",", " ") : tool_flags) << addrList;

    } else if (contains(tool_flags, "--transactions")) {
        replaceAll(tool_flags, "--transactions", "");
        os << "getTrans " << (isApiMode() ? substitute(tool_flags, ",", " ") : tool_flags);

    } else if (contains(tool_flags, "--receipts")) {
        replaceAll(tool_flags, "--receipts", "");
        os << "getReceipt " << (isApiMode() ? substitute(tool_flags, ",", " ") : tool_flags);

    } else if (contains(tool_flags, "--logs")) {
        replaceAll(tool_flags, "--logs", "");
        os << "getLogs " << (isApiMode() ? substitute(tool_flags, ",", " ") : tool_flags);

    } else if (contains(tool_flags, "--traces")) {
        replaceAll(tool_flags, "--traces", "");
        os << "getTrace " << (isApiMode() ? substitute(tool_flags, ",", " ") : tool_flags);

    } else if (contains(tool_flags, "--slurp")) {
        replaceAll(tool_flags, "--slurp", "");
        return handle_slurp();

    } else if (contains(tool_flags, "--quotes")) {
        replaceAll(tool_flags, "--quotes", "");
        return handle_quotes();

    } else if (contains(tool_flags, "--message")) {
        tool_flags =
            trim(substitute(substitute(substitute(tool_flags, "--message", ""), "--chars", ""), "--bytes", ""));
        if (contains(tool_flags, "0x"))
            cout << "message: " << toPrintable_force(tool_flags);
        else
            cout << "message: " << chr_2_HexStr(tool_flags);
        return true;

    } else {
        EXIT_FAIL("Invalid option: data " + tool_flags + " " + addrList);
    }

    LOG_CALL(os.str());
    // clang-format off
    if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on

    EXIT_NOMSG8(true);
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
