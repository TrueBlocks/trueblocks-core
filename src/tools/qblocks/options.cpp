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
    COption("~cmd",        "one of {cmds}"),
    COption("~sub_cmd(s)", "sub-command[s] for <cmd>. Use qblock cmd --help for help on the cmd"),
    COption("",            "Overarching command for all QBlocks tools.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

//    if (!standardOptions(command))
//        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (cmd.empty())
            cmd = arg;
        else
            args += (arg + " ");
    }

    CStringArray allowed;
    explode(allowed, substitute(allowable, " ", ""), '|');
    for (auto allow : allowed) {
        if (allow == cmd) {
            cmd = convert(cmd);
            return true;
        }
    }
    return usage("Sub-command must be one of [" + trim(allowable,'|') + "]. Quitting...");
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    minArgs = 0;
    optionOff(OPT_VERBOSE);
    allowable  = "blocks|txs|receipts|logs|traces|names|when|where|";
    allowable += getGlobalConfig()->getConfigStr("settings", "add_cmds", "");
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
//    namesFile = CFilename(configPath("names/names.txt"));
//    establishFolder(namesFile.getPath());
//    if (!fileExists(namesFile.getFullPath()))
//        stringToAsciiFile(namesFile.getFullPath(),
//                          substitute(
//                          substitute(string_q(STR_DEFAULT_DATA), " |", "|"), "|", "\t"));
//    loadNames();
    Init();
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "cmd sub_cmds", "<cmd> <sub_cmds>");
    } else if (which == "oneDescription") {
        return substitute(str, "{cmds}", "[ " + substitute(allowable, "|", " | ") + " ]");
    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        return "You may add you own commands by entering a value of [{add_cmds}] to the config file.";
    }
    return str;
}
