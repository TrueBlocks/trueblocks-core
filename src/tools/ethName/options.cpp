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
static const COption params[] = {
    COption("~terms",       "a space separated list of one or more search terms"),
    COption("-addr",        "export only the associated address (may be used in scripting)"),
    COption("-data",        "export results as tab separated data"),
    COption("-edit",        "open the name database for editing"),
    COption("-fmt:<fmt>",   "export format (one of [json|txt|csv])"),
    COption("-list",        "list all names in the database"),
    COption("-matchCase",   "matches must agree in case (the default is to ignore case)"),
    COption("@source",      "search 'source' field as well name and address (the default)"),
    COption("",             "Query Ethereum addresses and/or names making it easy to remember accounts.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_ALLFIELDS;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    export_t fmt = NONE;
    string_q format = "[{ADDR}]\t[{NAME}]"; //[ ({SYMBOL})]";

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-s" || arg == "--source") {
            format += "\t[{SOURCE}]";

        } else if (arg == "-a" || arg == "--addr") {
            format = "[{ADDR}]";
            fmt = NONE;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {
            arg = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
                 if ( arg == "txt" ) fmt = TXT;
            else if ( arg == "csv" ) fmt = CSV;
            else if ( arg == "json") fmt = JSON;
            else return usage("Export format must be one of [ json | txt | csv ]. Quitting...");

        } else if (arg == "-d" || arg == "--data") {
            asData = true;

        } else if (arg == "-l" || arg == "--list") {
            fmt = JSON;

        } else if (arg == "-m" || arg == "--matchCase") {
            matchCase = true;

        } else if (arg == "-e" || arg == "--edit") {
            isEdit = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (!search1.empty() && !search2.empty() && !search3.empty())
                return usage("You may search for at most three terms: " + arg);
            else if (!search1.empty() && !search2.empty())
                search3 = arg;
            else if (!search1.empty())
                search2 = arg;
            else
                search1 = arg;
        }
    }

    switch (fmt) {
        case NONE: break; //format = "[{ADDR}]\t[{NAME}]"; break; //[ ({SYMBOL})]"; break;
        case JSON: format = ""; break;
        case TXT:
        case CSV:  format = getGlobalConfig()->getConfigStr("display", "format", STR_ALLFIELDS); break;
    }
    expContext().fmtMap["nick"] = cleanFmt(format, fmt);
    applyFilter(format);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    search1 = "";
    search2 = "";
    search3 = "";
    matchCase = false;
    asData = true;
    isEdit = false;
    minArgs = 0;
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
        ret += "Name file: [{" +
                substitute(namesFile.getFullPath(), getHomeFolder(), "~/") +
                    "}] (" + uint_2_Str(fileSize(namesFile.getFullPath())) + ")\n";
        return ret;
    }
    return str;
}

//-----------------------------------------------------------------------
uint64_t COptions::applyFilter(const string_q& fmtIn) {

    ENTER("ethName:applyFilter");
    string_q fmt = fmtIn;
    if (!search3.empty()) fmt = "[ {DESCRIPTION}]" + fmt;
    if (!search2.empty()) fmt = "[ {NAME}][ {SYMBOL}]" + fmt;
    if (!search1.empty()) fmt = "[ {ADDR}]" + fmt;

    for (size_t i = 0 ; i < namedAccounts.size() ; i++) {
        string_q str = namedAccounts[i].Format(fmt);
        if (!matchCase) {
            str = toLower(str);
            search1 = toLower(search1);
            search2 = toLower(search2);
            search3 = toLower(search3);
        }
        if ((search1.empty() || (search1 == "*") || contains(str, search1)) &&
            (search2.empty() || (search2 == "*") || contains(str, search2)) &&
            (search3.empty() || (search3 == "*") || contains(str, search3))) {
            filtered.push_back(namedAccounts[i]);
        }
    }

    EXIT_OK("ethName:applyFilter: " + uint_2_Str(filtered.size()));
    return filtered.size();
}

//-----------------------------------------------------------------------
const char* STR_ALLFIELDS =
"[{ADDR}]\t"
"[{SYMBOL}]\t"
"[{NAME}]\t"
"[{SOURCE}]\t"
"[{DESCRIPTION}]\t"
"[{LOGO}]\t"
"[{VISIBLE}]";
