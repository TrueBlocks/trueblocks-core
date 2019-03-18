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
static const COption params[] = {
    COption("~block_list", "a space-separated list of one or more blocks to search for"),
    COption("-account",    "find an account file, not the block file"),
    COption("-bloom",      "find a bloom file, not the block file"),
    COption("",            "Reports if a block was found in the cache, at a local, or at a remote node.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    // This tool does not fail if the node is not running
    blknum_t latestBlock = isNodeRunning() ? getLastBlock_client() : NOPOS;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (arg == "-a" || arg == "--account") {
            mode = "account";

        } else if (arg == "-b" || arg == "--bloom") {
            mode = "bloom";

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {

            string_q ret = blocks.parseBlockList(arg, latestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    if (!blocks.hasBlocks())
        return usage("You must enter a valid block number. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    mode = "block";
    optionOff(OPT_DENOM);
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
    if (which == "options")
        return substitute(str, "block_list", "<block> [block...]");
    return str;
}
