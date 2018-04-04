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
    CParams("~address_list", "two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported"),
    CParams("~!block_list",  "an optional list of one or more blocks at which to report balances, defaults to 'latest'"),
    CParams("-byAcct",       "consider each address an ERC20 token except the last, whose balance is reported for each token"),
    CParams("-data",         "render results as tab delimited data (for example, to build a cap table)"),
    CParams("-list:<fn>",    "an alternative way to specify an address_list, place one address per line in the file 'fn'"),
    CParams("-noZero",       "suppress the display of zero balance accounts"),
    CParams("-total",        "if more than one balance is requested, display a total as well"),
    CParams("-info",         "retreive standarized information (name, decimals, totalSupply, etc.) about the token"),
    CParams("",              "Retrieve the token balance(s) for one or more addresses at the given (or latest) block(s).\n"),
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

        } else if (arg == "-t" || arg == "--total") {
            total = true;

        } else if (arg == "-i" || arg == "--info") {
            tokenInfo = true;
            
        } else if (arg == "-b" || arg == "--byAcct") {
            byAccount = true;

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

        } else if (isHash(arg)) {
            SFString ret = blocks.parseBlockList(arg, latestBlock);
            if (ret.endsWith("\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
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

    if (asData && total)
        return usage("Totalling is not available when exporting data.");

    address_list = Strip(address_list, '|');
    if (countOf('|', address_list) < 1)
        return usage("You must provide both a token contract and an account. Quitting...");

    if (!byAccount) {
        // first item is ERC20 contract, remainder are accounts
        // token holder1 holder2
        holders = address_list;
        tokens = nextTokenClear(holders, '|');

    } else {
        // last item is account, preceeding are ERC20 contracts
        address_list.Reverse();
        // holder token2 token1 - reversed
        tokens = address_list;
        holders = nextTokenClear(tokens,'|');
        tokens.Reverse(); holders.Reverse();
    }

    if (!blocks.hasBlocks()) {
        // use 'latest'
        blocks.numList[blocks.numList.getCount()] = latestBlock;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    tokens = "";
    holders = "";
    asData = false;
    noZero = false;
    byAccount = false;
    total = false;
    tokenInfo = false;
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
            str.Substitute("address_list block_list", "<address> <address> [address...] [block...]")
                .Substitute("-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        SFString ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "[{block_list}] may be space-separated list of values, a start-end range, a [{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).\n";
        ret += "If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
