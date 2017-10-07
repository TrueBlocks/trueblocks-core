/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~terms",       "a space separated list of one or more search terms"),
    CParams("-alone",       "export only the associated address (may be used in scripting)"),
    CParams("-count",       "print only the count of the number of matches"),
    CParams("-open",        "open the name database for editing"),
    CParams("-list",        "list all names in the database"),
    CParams("-matchCase",   "matches must agree in case (the default is to ignore case)"),
    CParams("-source",      "search 'source' field as well name and address (the default)"),
    CParams("",             "Query Ethereum addresses and/or names making it easy to remember accounts.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if (arg == "-s" || arg == "--source") {
            all = true;

        } else if (arg == "-c" || arg == "--count") {
            count = true;

        } else if (arg == "-a" || arg == "--alone") {
            alone = true;

        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-m" || arg == "--matchCase") {
            matchCase = true;

        } else if (arg == "-o" || arg == "--open") {
            isEdit = true;

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

    if (addr.empty() && name.empty() && !list && !isEdit)
        return usage("You must supply at least one of 'addr,' or 'name.' Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {

    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    addr = "";
    name = "";
    source = "";
    all = false;
    matchCase = false;
    list = false;
    alone = false;
    count = false;
    isEdit = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // If you need the names file, you have to add it in the constructor
    namesFile = CFilename(blockCachePath("names/names.txt"));
    establishFolder(namesFile.getPath());
    if (!fileExists(namesFile.getFullPath()))
        stringToAsciiFile(namesFile.getFullPath(), SFString(STR_DEFAULT_DATA).Substitute(" |","|").Substitute("|","\t"));
    loadNames();
    Init();
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "options") {
        return str.Substitute("terms", "<term> [term...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        SFString ret;
        ret += "With a single search term, the tool searches both [{name}] and [{address}].\n";
        ret += "With two search terms, the first term must match the [{address}] field, and the "
                "second term must match the [{name}] field.\n";
        ret += "When there are two search terms, both must match.\n";
        ret += "If one mixes options, the [{--edit}] option predominates (i.e. the program opens the database and then quits).\n";
        ret += "The [{--list}] option predominates otherwise. If present, the tool displays a list of stored names and addresses and then quits.\n";
        ret += "The [{--count}] option works with any other option and will simply display the number of matches.\n";
        ret += "The [{--matchCase}] option requires case sensitive matching. It works with all other options.\n";
        ret += "The [{--addrOnly}] option modifies the display output and therefore works with any other options.\n";
        return ret;
    }
    return str;
}
