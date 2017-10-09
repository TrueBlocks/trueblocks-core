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
    CParams("~address_list", "one or more addresses (0x...) from which to retrieve balances"),
    CParams("~!block_list",  "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),
    CParams("-list:<fn>",    "an alternative way to specify an address_list; place one address per line in the file 'fn'"),
    CParams("-noZero",       "suppress the display of zero balance accounts"),
    CParams("-data",         "render results as tab delimited data"),
    CParams("",              "Retrieve the balance (in wei) for one or more addresses at the given block(s).\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLatestBlockFromClient();
    SFString address_list;
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-n" || arg == "--noZero") {
            noZero = true;

        } else if (arg.startsWith("-l:") || arg.startsWith("--list:")) {

            CFilename fileName(arg.Substitute("-l:","").Substitute("--list:",""));
            if (!fileName.isValid())
                return usage("Not a valid filename: " + orig + ". Quitting...");
            if (!fileExists(fileName.getFullPath()))
                return usage("File " + fileName.relativePath() + " not found. Quitting...");
            SFString contents = asciiFileToString(fileName.getFullPath());
            if (contents.empty())
                return usage("No addresses were found in file " + fileName.relativePath() + ". Quitting...");
            while (!contents.empty()) {
                SFString line = nextTokenClear(contents, '\n');
                if (!isAddress(line))
                    return usage(line + " does not appear to be a valid Ethereum address. Quitting...");
                address_list += line + "|";
            }

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (arg.startsWith("0x")) {

            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
            address_list += arg + "|";

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

    if (address_list.empty())
        return usage("You must provide at least one Ethereum address.");
    addrs = address_list;

    if (!blocks.hasBlocks()) {
        // use 'latest'
        blocks.nums[blocks.nNums++] = latestBlock;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    addrs = "";
    asData = false;
    noZero = false;
    blocks.Init();
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
        return
            str.Substitute("address_list block_list", "<address> [address...] [block...]")
                .Substitute("-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        SFString ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "[{block_list}] may be a space-separated list of values, a start-end range, a [{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
