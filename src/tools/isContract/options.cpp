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
    CParams("~addr[s]", "a list of Ethereum addresses from which to check for byte code"),
    CParams("-display", "display the actual code at the address(es)"),
    CParams("-nodiff",  "return 'true' if (exactly) two Ethereum addresses have identical code"),
    CParams("",         "Returns 'true' or 'false' if the given address(es) hold byte code (or displays the code).\n"),
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

            SFString addr = toLower(arg);
            if (!addr.startsWith("0x"))
                addr = "0x" + addr;
            if (addr.length() != 42)
                return usage(arg + " does not appear to be a valid Ethereum address (must start "
                                        "with '0x' and be 40 hex chars long).\n");
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

    // addrs = [];
    nAddrs = 0;
    diff = false;
    display = false;

    useVerbose = true;
    useTesting = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    header = "This simple program may be used to query an Ethereum address to determine if it is a smart "
        "contract or simply a 'regular' account. It may also be used to retrieve the actual byte-code from "
        "an address (if code is present). Finally, it may be used to compare two addresses to determine if "
        "they hold identical code. You may specify multiple addresses on a line.\n";
    // footer = "";
    seeAlso = "See Also: This command-line tool implements the [eth_getCode](https:/""/github.com/paritytech"
        "/parity/wiki/JSONRPC-eth-module#eth_getcode) RPC interface.\n";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
