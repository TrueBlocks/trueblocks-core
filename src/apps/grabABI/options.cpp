/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~addr",        "the address(es) of the smart contract(s) to grab"),
    CParams("-canoncial",   "convert all types to their canoncial represenation and remove all spaces from display"),
    CParams("-generate",    "generate C++ code into ':dir' for all functions and events found in the ABI"),
    CParams("-encode",      "generate the encodings for the functions / events in the ABI"),
    CParams("-noconst",     "generate encodings for non-constant functions and events only (always true when generating)"), // NOLINT
    CParams("-open",        "open the ABI file for editing, download if not already present"),
    CParams("-silent",      "If ABI cannot be acquired, fail silently (useful for scripting)"),
    CParams("",             "Fetches the ABI for a smart contract. Optionally generates C++ source code "
                            "representing that ABI.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-g" || arg == "--gen" || arg == "--generate") {
            classDir = getCWD();
            prefix = getPrefix(classDir);

        } else if (arg == "-c" || arg == "--canonical") {
            if (parts&SIG_ENCODE)
                parts = (SIG_CANONICAL|SIG_ENCODE);
            else
                parts = SIG_CANONICAL;

        } else if (arg == "-e" || arg == "--encode") {
            parts |= SIG_ENCODE;

        } else if (arg == "-s" || arg == "--silent") {
            silent = true;

        } else if (arg == "-n" || arg == "--noconst") {
            noconst = true;

        } else if (arg == "-o" || arg == "--open") {
            open = true;

        } else if (arg.startsWith('-')) {
            if (arg != "-h" && !arg.Contains("-v") && arg != "-t")
                return usage("Invalid option: " + arg);

        } else {
            if (primaryAddr.empty())
                primaryAddr = arg;
            SFAddress addr = toLower(arg);
            if (!addr.startsWith("0x"))
                addr = "0x" + addr;
            if (addr.length() != 42 && !addr.ContainsI("tokenlib") && !addr.ContainsI("walletlib"))
                return usage(addr + " does not appear to be a valid Ethereum address.\n");
            addrs[nAddrs++] = addr;
            if (nAddrs>10)
                return usage("You may provide at most 10 addresses");
        }
    }

    if (parts == 0)
        parts = SIG_DEFAULT;

    if (parts != SIG_CANONICAL && verbose)
        parts |= SIG_DETAILS;

    if (!nAddrs)
        return usage("Please supply at least one Ethereum address.\n");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    parts = SIG_DEFAULT;
    noconst = false;
    open = false;
    silent = false;
    nAddrs = 0;

    useVerbose = true;
    useTesting = true;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    // header = "";
    // footer = "";
    // seeAlso = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString getPrefix(const SFString& inIn) {

    SFString in = inIn; // for example ./ENS/parselib/
    in.Reverse();
    in.Replace("/", ""); // remove trailing '/'
    in = nextTokenClear(in, '/'); // remove /parselib
    in.Reverse();\
    return in;
}
