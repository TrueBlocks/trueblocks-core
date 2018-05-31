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
    CParams("~!trans_list", "a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)"),
    CParams("-raw",         "retrieve raw transaction directly from the running node"),
    CParams("-nTraces",     "report on how many traces the transaction generated and deepest trace"),
    CParams("@trace",       "include the transactions trace after the transaction"),
    CParams("",             "Retrieve an Ethereum transaction from the local cache or a running node."),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-r" || arg == "--raw") {
            isRaw = true;

        } else if (arg == "-t" || arg == "--trace") {
            incTrace = true;

        } else if (arg == "-n" || arg == "--nTraces") {
            nTraces = true;

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            SFString ret = transList.parseTransList(arg);
            if (!ret.empty())
                return usage(ret);

        }
    }

    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

extern const char* STR_DISPLAY_FORMAT;
    format = getGlobalConfig()->getDisplayStr(!verbose, (verbose ? "" : STR_DISPLAY_FORMAT));

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    transList.Init();
    isRaw = false;
    incTrace = false;
    nTraces = false;
    format = "";
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);
    HIDE_FIELD(CTransaction, "cumulativeGasUsed");

    Init();
    // Don't reset these for each command
    nCmds = 0;
    nVisited = 0;
    if (isTestMode())
        UNHIDE_FIELD(CTransaction, "isError");
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "options") {
        return str.Substitute("trans_list","<transID> [transID...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        SFString ret;
        ret += "[{trans_list}] is one or more space-separated identifiers which may be either a transaction hash,|"
                "a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.\n";
        ret += "This tool checks for valid input syntax, but does not check that the transaction requested exists.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        ret += "If the queried node does not store historical state, the results may be undefined.\n";
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
const char* STR_DISPLAY_FORMAT = "[{DATE}][\t{TIMESTAMP}][\t{BLOCKNUMBER}][\t{TRANSACTIONINDEX}][\t{HASH}]\\n";
