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
    CParams("~num",         "which block (or blocks if more than one) to retreive (or use [start-stop) for range)"),
//    CParams("-check",       "pull the given block(s) from both cache and node, compare results, should be no output"),
    CParams("-fromNode",    "retreive the block(s) directly from the running node with no processing"),
    CParams("-mini",        "output only data fields that exist in miniBlocks and miniTrans"),
    CParams("-binary",      "retrieve only quickBlocks binary blocks if they exist, retrieve nothing otherwise"),
    CParams("-parity",      "mimic parity output using quickBlocks (i.e. quoted hexidecimal for numbers)"),
    CParams("-terse",       "retreive transaction hashes instead of full transactions"),
    CParams("-silent",      "do not print results to screen, used for speed testing"),
    CParams("",             "Returns block(s) from local cache (default) or directly from node.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-c" || arg == "--check") {
            isCheck = true;

        } else if (arg == "-f" || arg == "--fromNode") {
            isRaw = true;

        } else if (arg == "-t" || arg == "--terse") {
            terse = true;

        } else if (arg == "-s" || arg == "--silent") {
            silent = true;

        } else if (arg == "-b" || arg == "--binary") {
            etherlib_init("binaryOnly");

        } else if (arg == "-m" || arg == "--mini") {
            HIDE_ALL_FIELDS(CBlock);
            HIDE_ALL_FIELDS(CTransaction);
            HIDE_ALL_FIELDS(CReceipt);
            UNHIDE_FIELD(CBlock, "blockNumber");
            UNHIDE_FIELD(CBlock, "timestamp");
            UNHIDE_FIELD(CBlock, "transactions");
            UNHIDE_FIELD(CTransaction, "receipt");
            UNHIDE_FIELD(CTransaction, "transactionIndex");
            UNHIDE_FIELD(CTransaction, "gasPrice");
            UNHIDE_FIELD(CTransaction, "gas");
            UNHIDE_FIELD(CTransaction, "isError");
            UNHIDE_FIELD(CTransaction, "from");
            UNHIDE_FIELD(CTransaction, "to");
            UNHIDE_FIELD(CTransaction, "value");
            UNHIDE_FIELD(CReceipt, "gasUsed");

        } else if (arg == "-p" || arg == "--parity") {
            expContext().spcs = 2;
            expContext().hexNums = true;
            expContext().quoteNums = true;

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (arg == "latest") {
                SFUint32 cache, client;
                getLatestBlocks(cache, client);
                cout << "\n\tFrom client: " << asYellow(client)
                        << " inCache: " << asYellow(cache)
                        << " Behind (maybe empty): " << asYellow(client-cache) << "\n\n";
                exit(0);

            } else if (arg.Contains("-")) {

                SFString arg1 = nextTokenClear(arg, '-');

                if (arg1 == "latest")
                    return usage("Cannot start range with 'latest'");

                start = toLong(arg1);
                stop  = toLong(arg);
                if (arg == "latest")
                    stop = getClientLatestBlk();
                if (stop <= start)
                    return usage("'stop' must be strictly larger than 'start'");
                isRange = true;

            } else {
                // only a singe block number is accepted when receipt is requested
                static bool hasReceipt = false;
                if (!hasReceipt) {

                    if (arg.Contains("0x")) {
                        // Attach a request for a receipt with no spaces after the number
                        txHash = arg;
                        txHash.Replace("0x", "|0x");
                        arg = nextTokenClear(txHash, '|');
                        hasReceipt = true;
                    }

                    SFUint32 num = toLong(arg);
                    if (arg == "latest")
                        num = getClientLatestBlk();
                    if (nNums < MAX_NUMS)
                        nums[nNums++] = num;
                    else
                        return usage("Too many blocks in list. Max is " + asString(MAX_NUMS));
                }
            }
        }
    }

    if (isRange) nNums = 0;  // if range is supplied, use the range
    else if (nNums == 0) nNums = 1;  // otherwise, if not list, use 'latest'

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;

    // Mimics python -m json.tool indenting.
    expContext().spcs = 4;
    expContext().hexNums = false;
    expContext().quoteNums = false;

    isCheck    = false;
    isRaw      = false;
    isRange    = false;
    terse      = false;
    silent     = false;
    // txHash = "";
    nums[0]    = -1;
    nNums      = 0;  // we will set this to '1' later if user supplies no values
    start = stop = 0;

    useVerbose = true;
    useTesting = false;
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
bool COptions::isMulti(void) const {
    if (isRange)
        return (stop - start) > 1;
    return nNums > 1;
}
