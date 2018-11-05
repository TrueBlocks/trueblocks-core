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
    COption("~folder",       "name of the monitor (also the ./folder for the source code)"),
    COption("~address_list", "a list of one or more addresses to monitor (must start with '0x')"),
    COption("-silent",       "suppress all output from chifra (normally chifra is quite verbose)"),
    COption("",              "Interactively creates a QBlocks monitor for the given address.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {

        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-s" || arg == "--silent") {
            verbose = 0;

        } else if (startsWith(arg, '-')) {

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (startsWith(arg, "0x")) {
                if (!isAddress(arg))
                    return usage("Address '" + arg + "' does not appear to be a valid Ethereum adddress. Quitting...");
                addrList += arg + "|";

            } else {
                if (!sourceFolder.empty())
                    return usage("Extranious value '" + arg + ". Specify only a single folder or addresses starting with '0x'. Quitting...");
                CPath path(makeValidName(arg));
                sourceFolder = path.getFullPath();
                if (folderExists(sourceFolder))
                    return usage("Folder '" + sourceFolder + "' exists. Please remove it or use a different folder name. Quitting...");
                monitorName = arg;
            }
        }
    }

    if (sourceFolder.empty()) {
        return usage("You must supply a folder into which to place the monitor.");

    } else {
        // TODO(tjayrush): make this configurable
        CPath path(sourceFolder);
        monitorFolder = substitute(path.getFullPath(), "/src/monitors/", "/monitors/");
        if (verbose)
            cerr << "Monitor folder " << substitute(monitorFolder, getHomeFolder(), "~/") << "\n";
    }

    if (addrList.empty())
        return usage("You must supply at least one Ethereum address to monitor.");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    verbose = 1;
    sourceFolder = "";
    monitorFolder = "";
    addrList = "";
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "address_list", "<address> [address...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        return "[{addresses}] must start with '0x' and be forty characters long.\n";

    }
    return str;
}
