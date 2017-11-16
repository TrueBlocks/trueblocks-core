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
    CParams("~address_list", "a space-separated list of one or more Ethereum addresses"),
    CParams("-data",         "display results as data (addr <tab> is_contract)"),
    CParams("-bytes",        "display the byte code at the address(es)"),
    CParams("-nodiff",       "return 'true' if (exactly) two Ethereum addresses have identical code"),
    CParams("",              "Returns 'true' or 'false' if the given address(es) holds byte code "
                             "(optionally displays the code).\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-n" || arg == "--nodiff") {
            diff = true;

        } else if (arg == "-b" || arg == "--bytes") {
            showBytes = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;

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

    if (diff && showBytes)
        return usage("Choose only one of --nodiff and --display.\n");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    for (uint32_t i = 0 ; i < MAX_ADDRS ; i++) {
        addrs[i] = "";
    }
    nAddrs = 0;
    diff = false;
    asData = false;
    showBytes = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "options") {
        return str.Substitute("address_list", "<address> [address...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        SFString ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured.\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
