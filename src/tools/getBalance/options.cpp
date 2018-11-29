/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
    COption("~address_list", "one or more addresses (0x...) from which to retrieve balances"),
    COption("~!block_list",  "an optional list of one or more blocks at which to report balances, "
                                    "defaults to 'latest'"),
    COption("-data",         "render results as tab delimited data"),
    COption("-list:<fn>",    "an alternative way to specify an address_list; place one address per "
                                    "line in the file 'fn'"),
    COption("-noZero",       "suppress the display of zero balance accounts"),
    COption("-total",        "if more than one balance is requested, display a total as well."),
    COption("-changes",      "only report a balance when it changes from one block to the next"),
    COption("",              "Retrieve the balance (in wei) for one or more addresses at the given "
                                    "block(s).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-n" || arg == "--noZero") {
            noZero = true;

        } else if (arg == "-t" || arg == "--total") {
            total = true;

        } else if (arg == "-c" || arg == "--changes") {
            changes = true;

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--list:")) {

            CFilename fileName(substitute(substitute(arg, "-l:", ""), "--list:", ""));
            if (!fileName.isValid())
                return usage("Not a valid filename: " + orig + ". Quitting...");
            if (!fileExists(fileName.getFullPath()))
                return usage("File " + fileName.relativePath() + " not found. Quitting...");
            string_q contents;
            asciiFileToString(fileName.getFullPath(), contents);
            if (contents.empty())
                return usage("No addresses were found in file " + fileName.relativePath() + ". Quitting...");
            while (!contents.empty()) {
                string_q line = nextTokenClear(contents, '\n');
                if (!isAddress(line))
                    return usage(line + " does not appear to be a valid Ethereum address. Quitting...");
                addrs.push_back(toLower(line));
            }

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (isHash(arg)) {
            string_q ret = blocks.parseBlockList(arg, newestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }

        } else if (startsWith(arg, "0x")) {

            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address. Quitting...");
            addrs.push_back(toLower(arg));

        } else {

            string_q ret = blocks.parseBlockList(arg, newestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (asData && total)
        return usage("Totalling is not available when exporting data.");

    if (!addrs.size())
        return usage("You must provide at least one Ethereum address.");

    if (!blocks.hasBlocks())
        blocks.numList.push_back(newestBlock);  // use 'latest'

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    addrs.clear();
    asData = false;
    noZero = false;
    total = false;
    changes = false;
    blocks.Init();
    CHistoryOptions::Init();
    newestBlock = oldestBlock = getLatestBlockFromClient();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : CHistoryOptions() {

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
        return
            substitute(substitute(str, "address_list block_list", "<address> [address...] [block...]"),
                                            "-l|", "-l fn|");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty characters long.\n";
        ret += "[{block_list}] may be a space-separated list of values, a start-end range, a "
                    "[{special}], or any combination.\n";
        ret += "This tool retrieves information from the local node or the ${FALLBACK} node, if "
                    "configured (see documentation).\n";
        ret += "If the queried node does not store historical state, the results are undefined.\n";
        ret += "[{special}] blocks are detailed under " + cTeal + "[{whenBlock --list}]" + cOff + ".\n";
        return ret;
    }
    return str;
}
