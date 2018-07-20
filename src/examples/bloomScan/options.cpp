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
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~source",      "source for the data (either 'remote', 'pairty', 'binary')"),
    COption("~begin",       "block to start with"),
    COption("~end",         "block to end on"),
    COption("~!skip",       "optional skip step (default 100)"),
    COption("-mode:<mode>", "if not present, display in 'short' mode, otherwise 'full' mode"),
    COption("-data",        "show results as data instead of displaying bloom filters"),
    COption("",             "Scans blocks looking for saturated bloomFilters.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    string_q source;
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;
        if (startsWith(arg, "-m:") || startsWith(arg, "--mode:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--mode:", "");
            if (arg != "short" && arg != "full")
                return usage("Mode must be either 'full' or 'short'. Quitting...");
            mode = arg;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!isUnsigned(arg)) {
                if (arg != "remote" && arg != "binary" && arg != "local")
                    return usage("Incorrect value (" + orig + ") for mode. Quitting.");
                etherlib_init(arg);
                source = arg;
            } else {
                if (start == NOPOS)
                    start = str_2_Uint(arg);
                else if (stop == NOPOS)
                    stop = str_2_Uint(arg);
                else if (skip == NOPOS)
                    skip = str_2_Uint(arg);
                else
                    return usage("Too many parameters");
            }
        }
    }

    if (source.empty())
        return usage("You must provide a source of either 'local', 'remote', or 'binary'. Quitting...");
    if (start == NOPOS || stop == NOPOS)
        return usage("Please provide both a start block and an end block. Quitting...");
    if (start >= stop)
        return usage("The end block must be greater than the start block. Quitting...");
    if (skip == NOPOS)
        skip = 100;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;
    start = NOPOS;
    stop  = NOPOS;
    skip  = NOPOS;
    mode  = "short";
    asData = false;
    optionOff(OPT_VERBOSE);
    minArgs = 2;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
