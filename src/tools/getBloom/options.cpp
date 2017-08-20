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
    CParams("~num",      "which bloom (or blooms if more than one) to retreive (or [start-stop) range)"),
    CParams("-enhanced", "retrieve the enhanced bloom filter from the quickBlocks cache"),
    CParams("-raw",      "retrieve the bloom filter directly from the running node (includes tx blooms)"),
    CParams("-quiet",    "do not print results to screen (useful for performance measurements)"),
    CParams("",          "Returns bloom filter(s) from local cache (the default) or directly from a running node.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');

        // shortcuts
        if (arg == "-r") arg = "--source:raw";
        if (arg == "-c") arg = "--source:cache";

        if (arg == "-c" || arg == "--check") {
            isCheck = true;

        } else if (arg == "-o" || arg == "--force") {
            etherlib_init("binary");

        } else if (arg.startsWith("-s:") || arg.startsWith("--source:")) {
            SFString mode = arg;
            mode.Replace("-s:","");
            mode.Replace("--source:","");
            if (mode == "r" || mode == "raw") {
                isRaw = true;

            } else if (mode == "c" || mode == "cache") {
                etherlib_init("binaryOnly");

            } else {
                return usage("Invalide source. Must be either '(r)aw' or '(c)ache'. Quitting...");
            }

        } else if (arg == "-t" || arg == "--terse") {
            terse = true;

        } else if (arg == "-q" || arg == "--quiet") {
            quiet = true;

        } else if (arg.startsWith("-f:") || arg.startsWith("--fields:")) {
            SFString mode = arg;
            mode.Replace("-f:","");
            mode.Replace("--fields:","");

            if (mode == "a" || mode == "all") {
                SHOW_ALL_FIELDS(CBlock);
                SHOW_ALL_FIELDS(CTransaction);
                SHOW_ALL_FIELDS(CReceipt);

            } else if (mode == "m" || mode == "mini") {
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

            } else if (mode == "r" || mode == "raw") {
            } else if (mode == "c" || mode == "cache") {
                SHOW_ALL_FIELDS(CBlock);
                SHOW_ALL_FIELDS(CTransaction);
                SHOW_ALL_FIELDS(CReceipt);
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
            }

        } else if (arg == "-p" || arg == "--parity") {
            expContext().spcs = 4;
            expContext().hexNums = true;
            expContext().quoteNums = true;
            GETRUNTIME_CLASS(CBlock)->sortFieldList();
            GETRUNTIME_CLASS(CTransaction)->sortFieldList();
            GETRUNTIME_CLASS(CReceipt)->sortFieldList();

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

                start = toUnsigned(arg1);
                stop  = toUnsigned(arg);
                if (arg == "latest")
                    stop = getLatestBlockFromClient();
                if (stop <= start)
                    return usage("'stop' must be strictly larger than 'start'");
                isRange = true;

            } else {
                SFUint32 num = toUnsigned(arg);
                if (arg == "latest")
                    num = getLatestBlockFromClient();
                if (nNums < MAX_NUMS)
                    nums[nNums++] = num;
                else
                    return usage("Too many blocks in list. Max is " + asString(MAX_NUMS));
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

    isCheck = false;
    isRaw   = false;
    isRange = false;
    terse   = false;
    quiet   = false;
    nums[0]    = -1;
    nNums      = 0;  // we will set this to '1' later if user supplies no values
    start = stop = 0;

    useVerbose = true;
    useTesting = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
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
