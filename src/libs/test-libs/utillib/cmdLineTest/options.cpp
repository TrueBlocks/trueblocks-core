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
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~testNum",         "the number of the test to run"),
    CParams("-bool:<bool>",     "enter a boolean value (either `0`, `1`, `false`, or `true`)"),
    CParams("-int:<int>",       "enter any numeric value"),
    CParams("-uint:<uint>",     "enter any numeric value greater than or equal to zero"),
    CParams("-string:<string>", "enter any value"),
    CParams("-range:<range>",   "enter a range of numeric values"),
    CParams("-list:<list>",     "enter a list of value separated by commas (no spaces or quoted)"),
    CParams("",                 "Tests various command line behavior.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;
        if (arg.startsWith("-b:") || arg.startsWith("--bool:")) {
            arg = arg.Substitute("-b:","").Substitute("--bool:","");
            if (arg == "1" || arg == "true") {
                boolOption = true;
                boolSet = true;
            } else if (arg == "0" || arg == "false") {
                boolOption = false;
                boolSet = true;
            } else
                usage("Invalid bool: " + orig);

        } else if (arg.startsWith("-i:") || arg.startsWith("--int:")) {
            arg = arg.Substitute("-i:","").Substitute("--int:","");
            if (arg.empty() || (arg[0] != '-' && arg[0] != '+' && !isdigit(arg[0])))
                return usage("--int requires a number. Quitting");
            numOption = toLong(arg);

        } else if (arg.startsWith("-u:") || arg.startsWith("--uint:")) {
            arg = arg.Substitute("-u:","").Substitute("--uint:","");
            if (arg.empty() || (arg[0] != '+' && !isdigit(arg[0]))) {
                //return usage("--uint requires a non-negative number. Quitting");
            } else {
                numOption = toLong(arg);
            }

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            testNum = (int32_t)toLong(arg);
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    boolOption = false;
    boolSet = false;
    numOption = -1;
    stringOption = "";
    testNum = -1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
