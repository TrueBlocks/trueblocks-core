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
    CParams("~addr[s]",  "a space-separated list of one or more Ethereum addresses"),
    CParams("-display",  "display the byte code at the address(es)"),
    CParams("-nodiff",   "return 'true' if (exactly) two Ethereum addresses have identical code"),
    CParams("",          "Returns 'true' or 'false' if the given address(es) holds byte code "
                         "(optionally displays the code).\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-n" || arg == "--nodiff") {
            diff = true;

        } else if (arg == "-d" || arg == "--display") {
            display = true;

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

         } else {

             if (nAddrs >= MAX_ADDRS)
                 return usage("You may query at most " + asString(MAX_ADDRS) + " addresses. Quitting...");
            SFString addr = fixAddress(toLower(arg));
            if (!isAddress(addr))
                return usage(arg + " does not appear to be a valid Ethereum address.\n");
            addrs[nAddrs++] = addr;

        }
    }

    if (nAddrs == 0)
        return usage("Please supply valid Ethereum addresses.\n");

    if (diff && nAddrs != 2)
        return usage("--nodiff command requires exactly two addresses.\n");

    if (diff && display)
        return usage("Choose only one of --nodiff and --display.\n");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    for (uint32_t i = 0 ; i < MAX_ADDRS ; i++) {
        addrs[i] = "";
    }
    nAddrs = 0;
    diff = false;
    display = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
