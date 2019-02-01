/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~folder",       "name of the monitor (also the ./folder for the source code)"),
    COption("~address_list", "a list of one or more addresses to monitor (must start with '0x')"),
    COption("-silent",       "suppress all output from chifra (normally chifra is quite verbose)"),
    COption("",              "Interactively creates a QBlocks monitor for the given address.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
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
    registerOptions(nParams, params);

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
