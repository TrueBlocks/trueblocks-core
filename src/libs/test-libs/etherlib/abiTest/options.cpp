/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
    COption("modes", "m", "list<enum[encoding|generation]>", OPT_REQUIRED | OPT_POSITIONAL, "mode of operation"),
    COption("sub", "s", "<num>", OPT_FLAG, "sub mode"),
    COption("", "", "", OPT_DESCRIPTION, "Simple program to illustrate how to encode function and event signatures."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "encoding" || arg == "generation" || arg == "old_bug" || arg == "eth_test") {
            mode += (arg + "|");
        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sub:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--sub:", "");
            if (!isUnsigned(arg))
                return usage("--sub must be a non-negative number.");
            sub = str_2_Uint(arg);

        } else {
            return usage("Invalid argument " + arg + ". Please enter either 'encoding' or 'generation'.");
        }
    }
    if (mode == "")
        return usage("You must enter either 'encoding' or 'generation'.");
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    mode = "";
    sub = 0;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
