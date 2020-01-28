/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
    COption("testNum", "t", "", OPT_REQUIRED | OPT_POSITIONAL, "the number of the test to run"),
    COption("optionalMode", "o", "", OPT_POSITIONAL, "an optional mode with ~! start"),
    COption("bool", "b", "<bool>", OPT_FLAG, "enter a boolean value (either '0', '1', 'false', or 'true')"),
    COption("int", "i", "<int>", OPT_FLAG, "enter any numeric value"),
    COption("uint", "u", "<uint>", OPT_FLAG, "enter any numeric value greater than or equal to zero"),
    COption("string", "s", "<string>", OPT_FLAG, "enter any value"),
    COption("range", "r", "<range>", OPT_FLAG, "enter a range of numeric values"),
    COption("list", "l", "<list>", OPT_FLAG, "enter a list of value separated by commas (no spaces or quoted)"),
    COption("hiddenOption", "d", "", OPT_HIDDEN | OPT_FLAG, "a hidden option with an alternative hot key"),
    COption("", "", "", OPT_DESCRIPTION, "Tests various command line behavior.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (startsWith(arg, "-b:") || startsWith(arg, "--bool:")) {
            arg = substitute(substitute(arg, "-b:", ""), "--bool:", "");
            if (arg == "1" || arg == "true") {
                boolOption = true;
                boolSet = true;
            } else if (arg == "0" || arg == "false") {
                boolOption = false;
                boolSet = true;
            } else {
                usage("Invalid bool: " + orig);
            }

        } else if (startsWith(arg, "-i:") || startsWith(arg, "--int:")) {
            arg = substitute(substitute(arg, "-i:", ""), "--int:", "");
            if (arg.empty() || (arg[0] != '-' && arg[0] != '+' && !isdigit(arg[0])))
                return usage("--int requires a number. Quitting");
            numOption = str_2_Int(arg);

        } else if (startsWith(arg, "-u:") || startsWith(arg, "--uint:")) {
            arg = substitute(substitute(arg, "-u:", ""), "--uint:", "");
            if (arg.empty() || (arg[0] != '+' && !isdigit(arg[0]))) {
                // return usage("--uint requires a non-negative number. Quitting");
            } else {
                numOption = str_2_Int(arg);
            }

        } else if (startsWith(arg, "-d") || startsWith(arg, "--hiddenOption")) {
            boolOption = !boolOption;
            stringOption = "Flipped by hidden option";

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            testNum = (int32_t)str_2_Int(arg);
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_FMT);

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
