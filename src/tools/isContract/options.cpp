/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~address_list", "a space-separated list of one or more Ethereum addresses"),
    COption("-data",         "display results as data (addr <tab> is_contract)"),
    COption("-bytes",        "display the byte code at the address(es)"),
    COption("-nodiff",       "return 'true' if (exactly) two Ethereum addresses have identical code"),
    COption("",              "Returns 'true' or 'false' if the given address(es) holds byte code "
                             "(optionally displays the code).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {

        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-n" || arg == "--nodiff") {
            diff = true;

        } else if (arg == "-b" || arg == "--bytes") {
            showBytes = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

         } else {

             if (nAddrs >= MAX_ADDRS)
                 return usage("You may query at most " + int_2_Str(MAX_ADDRS) + " addresses. Quitting...");
             if (!isAddress(arg))
                 return usage(arg + " does not appear to be a valid Ethereum address.\n");
            addrs[nAddrs++] = str_2_Addr(toLower(arg));

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

    for (size_t i = 0 ; i < MAX_ADDRS ; i++) {
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
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "address_list", "<address> [address...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured.\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
