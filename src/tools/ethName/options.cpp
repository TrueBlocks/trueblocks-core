/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"
#include "accountname.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~term [name]", "search terms"),
    CParams("-addrOnly",    "export only the associated address to be used as input to further commands"),
    CParams("-count",       "print only the count of the number of matches"),
    CParams("-open",        "open the name database for editing"),
    CParams("-list",        "list all names in the database"),
    CParams("-matchCase",   "matches must agree in case (the default is to ignore case)"),
    CParams("-source",      "search 'source' field as well name and address (the default)"),
    CParams("",             "Find a name given an Ethereum address, or find an address given a name.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-s" || arg == "--source") {
            all = true;

        } else if (arg == "-c" || arg == "--count") {
            count = true;

        } else if (arg == "-a" || arg == "--addrOnly") {
            addrOnly = true;

        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-m" || arg == "--matchCase") {
            matchCase = true;

        } else if (arg == "-o" || arg == "--open") {
            editFile(configPath("names/names.conf"));
            exit(0);

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (!addr.empty() && !name.empty() && !source.empty())
                return usage("You may search for at most three terms: " + arg);
            else if (!addr.empty() && !name.empty())
                source = arg;
            else if (!addr.empty())
                name = arg;
            else
                addr = arg;
        }
    }

    if (addr.empty() && name.empty() && !list)
        return usage("You must supply at least one of 'addr,' or 'name.' Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {

    CAccountName::registerClass();
    paramsPtr = params;
    nParamsRef = nParams;

    addr = "";
    name = "";
    source = "";
    all = false;
    matchCase = false;
    list = false;
    addrOnly = false;
    count = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}
