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
    COption("~mode",      "either 'list', 'at, or 'freshen'"),
    COption("-at:<key>",  "return the value stored at address 'val'"),
    COption("-list",      "list all accounts in the account database (list mode)"),
    COption("-freshen",   "starting with the last block visited, update the account tree (freshen mode)"),
    COption("-refreshen", "remove the existing account tree and re-create it from scratch with freshen mode"),
    COption("",           "Build a database of accounts with arbirtrary data attached.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    bool isRefreshen = false;
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-l" || arg == "--list") {
            mode += "list|";

        } else if (startsWith(arg, "-a") || startsWith(arg, "--at")) {
            arg = substitute(substitute(arg, "-a:", ""), "--at:", "");
            if (!isAddress(arg))
                return usage("--at requires an following address. Quitting...");
            address_list += (arg + "|");

        } else if (arg == "-f" || arg == "--freshen") {
            mode += "freshen|";

        } else if (arg == "-r" || arg == "--refreshen") {
            if (fileExists(accountIndex)) {
                cerr << "Are you sure you want to remove the account index? (y=yes)\n> ";
                int ch = getchar();
                if (ch == 'y' || ch == 'Y') {
                    remove(accountIndex.c_str());
                } else {
                    cerr << "Not removed. Quitting.\n";
                    cerr.flush();
                    exit(0);
                }
            }
            isRefreshen = true;
            mode += "freshen|";

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            mode += (arg + "|");
        }
    }

    bool hasFresh = contains(mode, "freshen");
    bool hasList  = contains(mode, "list");

    // fixup so things happen in a sensible order
    mode  = (hasFresh ? "freshen|" : "");
    mode += (hasList  ? "list|"    : "");
    if (mode.empty() && address_list.empty())
        return usage("Mode must be one of --list, --freshen, --at, or --refreshen.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    address_list = "";
    mode = "";
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

