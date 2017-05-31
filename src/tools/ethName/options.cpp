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
    CParams("-edit",        "edit the name database"),
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

        } else if (arg == "-e" || arg == "--edit") {
            // open command stuff
            SFString command = ("nano -I " + configPath("configs/names.conf"));
            if (system(command.c_str())) {}; // do not remove. The test just silences compiler warnings
            exit(0);

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!addr.empty() && !name.empty() && !source.empty())
                return usage("Invalid option: " + arg);
            else if (!addr.empty() && !name.empty())
                source = arg;
            else if (!addr.empty())
                name = arg;
            else
                addr = arg;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {

    CAccountName::registerClass();
    paramsPtr = params;
    nParamsRef = nParams;

    // addr = "";
    // name = "";
    // source = "";
    all = false;
    matchCase = false;
    list = false;
    addrOnly = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    COptionsBase::useVerbose = true;
    COptionsBase::useTesting = false;
    Init();
    header = "`ethName` provides a very simple, poorly-done method by which to keep track of 'real life' names and "
            "their associated Ethereum addresses. While the tools needs a lot of work, it has come in handy as we've "
            "been writing **quickBlocks**. Eventually a feature such as this will be replaced with ENS (or an "
            "equivalent), but in the mean time, `ethname` works well to help you remember who is associated with "
            "which addresses.\n"
            "\n"
            "A very useful way to use of this tools is to feed its output into another command-line tool's input. "
            "For example, you could make a command such as:\n"
            "\n"
            "    ethname -a 'FredJones' | ethscan\n"
            "\n"
            "which would open the [etscan](http://etherscan.io) website with the address associated with "
            "Fred's address(es).\n";
    // footer = "";
    seeAlso = "Notes: With one search term, search in both `name` and `address` fields. With two terms, the first "
            "must match the `address` field, while the second must match the `name` field. When there are "
            "two search terms, both must match.\n";
}
