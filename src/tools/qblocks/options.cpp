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
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~cmd",       "one of {cmds}"),
    COption("~option(s)", "options sent to <cmd>. Use 'qblock cmd --help' for help on <cmd>"),
    COption("-info",      "Ignore <cmd> and display info about the QBlocks system"),
    COption("",           "Overarching command for all QBlocks tools.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (cmd.empty()) {
            if (arg == "-i" || arg == "--info") {
                cmd = "block";
                args = "--latest";
                command = "";
            } else {
                cmd = arg;
            }
        } else {
            args += (arg + " ");
        }
    }

    if (!findToolName(toolName, cmd))
        return usage("Command must be one of [" + toolNicknames() + "]. Quitting...");
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    minArgs = 0;
    optionOff(OPT_VERBOSE|OPT_HELP);
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "cmd options", "<cmd> <options>");
    } else if (which == "usage") {
        return substitute(str, "[-i|]", "[-i]");
    } else if (which == "oneDescription") {
        string_q list = toolNicknames();
        list = substitute(substitute(str, " (required)", ""), "{cmds}", "[" + list + "]");
        list = substitute(list, "|]", "]");
        list = substitute(list, "[-i|]", "[-i]");
        return list;
    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        return "You may add you own commands by entering a value in the config file.";
    }
    return str;
}
