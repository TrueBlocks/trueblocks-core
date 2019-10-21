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
    COption("mode", "m", "[0..2]", OPT_REQUIRED | OPT_POSITIONAL, "a number between 0 and 2 inclusive"),
    COption("", "", "", OPT_DESCRIPTION, "Test 'c' library strings against QBlocks strings.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, '-')) {   // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            testNum = str_2_Uint(arg);
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_FMT);

    testNum = NOPOS;
}
