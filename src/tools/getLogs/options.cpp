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
CParams params[] = {
    CParams("~!trans_list",    "a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)"),
//    CParams("-address:<addr>", "retrieve raw transaction for a given Ethereum address"),
    CParams("-raw",            "retrieve raw transaction directly from the running node"),
    CParams("",                "Retrieve a transaction's logs from the local cache or a running node."),
};
size_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;
        if (arg == "-r" || arg == "--raw") {
            isRaw = true;

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--address:")) {
            arg = substitute(substitute(arg, "-a:", ""), "--address:", "");
            if (!isAddress(arg))
                return usage(orig + " does not appear to be a valid Ethereum address. Quitting...");
            address_list += arg + "|";

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            string_q ret = transList.parseTransList(arg);
            if (!ret.empty())
                return usage(ret);

        }
    }

    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    transList.Init();
    address_list = "";
    isRaw = false;
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
        return substitute(str, "trans_list", "<transID> [transID...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{trans_list}] is one or more space-separated identifiers which may be either a transaction hash,|"
                "a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.\n";
        ret += "This tool checks for valid input syntax, but does not check that the transaction requested exists.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        ret += "If the queried node does not store historical state, the results may be undefined.\n";
        return ret;
    }
    return str;
}

