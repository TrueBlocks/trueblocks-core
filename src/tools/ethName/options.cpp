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
    COption("~terms",       "a space separated list of one or more search terms"),
    COption("-addr",        "export only the associated address (may be used in scripting)"),
    COption("-count",       "print only the count of the number of matches"),
    COption("-data",        "export results as tab separated data"),
    COption("-open",        "open the name database for editing"),
    COption("-list",        "list all names in the database"),
    COption("-matchCase",   "matches must agree in case (the default is to ignore case)"),
    COption("-source",      "search 'source' field as well name and address (the default)"),
    COption("",             "Query Ethereum addresses and/or names making it easy to remember accounts.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-s" || arg == "--source") {
            all = true;

        } else if (arg == "-c" || arg == "--count") {
            count = true;

        } else if (arg == "-a" || arg == "--addr") {
            addrOnly = true;

        } else if (arg == "-d" || arg == "--data") {
            data = true;

        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-m" || arg == "--matchCase") {
            matchCase = true;

        } else if (arg == "-o" || arg == "--open") {
            isEdit = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

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
    addrOnly = false;
    data = false;
    count = false;
    isEdit = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    // If you need the names file, you have to add it in the constructor
    namesFile = CFilename(configPath("names/names.txt"));
    establishFolder(namesFile.getPath());
    if (!fileExists(namesFile.getFullPath()))
        stringToAsciiFile(namesFile.getFullPath(),
                          substitute(
                          substitute(string_q(STR_DEFAULT_DATA), " |", "|"), "|", "\t"));
    loadNames();
    Init();
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "terms", "<term> [term...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        string_q ret;
        ret += "With a single search term, the tool searches both [{name}] and [{address}].\n";
        ret += "With two search terms, the first term must match the [{address}] field, and the "
                "second term must match the [{name}] field.\n";
        ret += "When there are two search terms, both must match.\n";
        ret += "If one mixes options, the [{--edit}] option predominates (i.e. the program opens the "
                    "database and then quits).\n";
        ret += "The [{--list}] option predominates otherwise. If present, the tool displays a list of "
                    "stored names and addresses and then quits.\n";
        ret += "The [{--count}] option works with any other option and will simply display the number of matches.\n";
        ret += "The [{--matchCase}] option requires case sensitive matching. It works with all other options.\n";
        ret += "The [{--addrOnly}] option modifies the display output and therefore works with any other options.\n";
        ret += "Name file: [{" +
                substitute(namesFile.getFullPath(), getHomeFolder(), "~/") +
                    "}] (" + uint_2_Str(fileSize(namesFile.getFullPath())) + ")\n";
        return ret;
    }
    return str;
}
