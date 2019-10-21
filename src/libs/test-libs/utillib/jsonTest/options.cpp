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
    COption("files", "s", "list<fn>", OPT_REQUIRED | OPT_POSITIONAL, "One or more files to parse"),
    COption("", "", "", OPT_DESCRIPTION, "Test the json parsing facility in QBlocks.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            string_q fn = "tests/" + arg + ".json";
            if (!fileExists(fn))
                return usage("File : " + fn + " does not exists. Quitting...");
            if (!fileName.empty())
                return usage("Only specify one file name per command. Quitting...");
            fileName = arg;
        }
    }

    if (fileName.empty())
        return usage("You must supply a filename to test. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_FMT);

    fileName = "";
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
