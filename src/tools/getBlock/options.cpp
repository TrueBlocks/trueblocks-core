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
    CParams("~block_list",       "a space-separated list of one or more blocks to retrieve"),
    CParams("-raw",              "pull the block data from the running Ethereum node (no cache)"),
    CParams("-terse",            "display only transaction hashes, default is to display full transaction details"),
    CParams("-check",            "compare results between qblocks and Ethereum node, report differences, if any"),
    CParams("@force",            "force a re-write of the block to the cache"),
    CParams("@quiet",            "do not print results to screen, used for speed testing and data checking"),
    CParams("@source:[c|r]",     "either :c(a)che or :(r)aw, source for data retrival. (shortcuts -c = qblocks, -r = node)"),
    CParams("@fields:[a|m|c|r]", "either :(a)ll, (m)ini, (c)ache or :(r)aw; which fields to include in output (all is default)"),
    CParams("@normalize",        "normalize (remove un-common fields and sort) for comparison with other results (testing)"),
    CParams("",                  "Returns block(s) from local cache or directly from a running node.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    while (!command.empty()) {

        SFString arg = nextTokenClear(command, ' ');

        // shortcuts
        if (arg == "-r" || arg == "--raw")   { arg = "--source:raw";   }
        if (arg == "-a" || arg == "--cache") { arg = "--source:cache"; }

        // do not collapse
        if (arg == "-c" || arg == "--check") {
            setenv("TEST_MODE", "true", true);
            isCheck = true;
            quiet++; // if both --check and --quiet are present, be very quiet...
            expContext().spcs = 4;
            expContext().hexNums = true;
            expContext().quoteNums = true;
            CRuntimeClass *pClass = GETRUNTIME_CLASS(CBlock);
            if (pClass) {
                CFieldData *pField = pClass->FindField("blockNumber");
                if (pField)
                    pField->setName("number");
            }
            pClass = GETRUNTIME_CLASS(CBlock);
            if (pClass) {
                CFieldData *pField = pClass->FindField("hash");
                if (pField)
                    pField->setName("blockHash");
            }
            GETRUNTIME_CLASS(CBlock)->sortFieldList();
            GETRUNTIME_CLASS(CTransaction)->sortFieldList();
            GETRUNTIME_CLASS(CReceipt)->sortFieldList();

        } else if (arg == "-o" || arg == "--force") {
            etherlib_init("binary");
            force = true;

        } else if (arg == "--normalize") {
            normalize = true;

        } else if (arg.startsWith("-s:") || arg.startsWith("--source:")) {
            SFString mode = arg.Substitute("-s:","").Substitute("--source:","");
            if (mode == "r" || mode == "raw") {
                isRaw = true;

            } else if (mode == "c" || mode == "cache") {
                etherlib_init("binaryOnly");
                asks4Cache = true;

            } else {
                return usage("Invalide source. Must be either '(r)aw' or '(c)ache'. Quitting...");
            }

        } else if (arg == "-t" || arg == "--terse") {
            terse = true;

        } else if (arg == "-q" || arg == "--quiet") {
            quiet++; // if both --check and --quiet are present, be very quiet...

        } else if (arg.startsWith("-f:") || arg.startsWith("--fields:")) {
            SFString mode = arg.Substitute("-f:","").Substitute("--fields:","");

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
            }

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            SFString ret = blocks.parseBlockList(arg, latestBlock);
            if (ret.endsWith("\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (expContext().isParity) {
        HIDE_FIELD(CTransaction, "cumulativeGasUsed");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CTransaction, "timestamp");
        CRuntimeClass *pClass = GETRUNTIME_CLASS(CBlock);
        if (pClass) {
            CFieldData *pField = pClass->FindField("blockNumber");
            if (pField)
                pField->setName("number");
            pClass->sortFieldList();
        }
    }

    if (terse && !isRaw)
        return usage("The --terse option works only with --raw. Quitting...");

    if (!blocks.hasBlocks())
        return usage("You must specify at least one block.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;
    pOptions = this;

    isCheck    = false;
    isRaw      = false;
    terse      = false;
    force      = false;
    normalize  = false;
    asks4Cache = false;
    quiet      = 0; // quiet has levels
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Mimics python -m json.tool indenting.
    expContext().spcs = 4;
    expContext().hexNums = false;
    expContext().quoteNums = false;

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
bool COptions::isMulti(void) const {
    return ((blocks.stop - blocks.start) > 1 || blocks.hashList.getCount() > 1 || blocks.numList.getCount() > 1);
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {

    if (which == "options") {
        return
            str.Substitute("block_list", "<block> [block...]")
                .Substitute("-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        SFString ret;
        ret += "[{block_list}] is a space-separated list of values, a start-end range, a [{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
SFString COptions::getBlockNumList(void) const {
    SFString ret;
    SFString list = blocks.toString();
    while (!list.empty()) {
        SFString val = nextTokenClear(list, '|');
        blknum_t bn = toLongU(val);
        if (isHash(val)) {
            CBlock block;
            if (!getBlock(block, val)) {
                cerr << "Block hash '" << val << "' does not appear to be a valid block hash. Quitting...";
                exit(0);
            }
            bn = block.blockNumber;
        }
        ret += asStringU(bn) + "|";
    }
    return ret;
}
