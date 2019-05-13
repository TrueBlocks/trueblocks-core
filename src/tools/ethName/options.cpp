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
static const COption params[] = {
    COption("~terms",       "a space separated list of one or more search terms"),
    COption("-allFiel(d)s", "search all fields (default searches name, address, and symbol only)"),
    COption("-matchCase",   "case-sensitive search"),
    COption("@fmt:<fmt>",   "export format (one of [none|json|txt|csv|api])"),
    COption("@a(d)d",       "add a record to the database"),
    COption("@edit",        "open the name database for editing"),
    COption("",             "Query Ethereum addresses and/or names making it easy to remember accounts.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    string_q format;
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-e" || arg == "--edit") {
            editFile(namesFile.getFullPath());
            return false;

        } else if (arg == "-d" || arg == "--allFields") {
const char* STR_ALLFIELDS =
"[{ADDR}]\t"
"[{SYMBOL}]\t"
"[{NAME}]\t"
"[{SOURCE}]\t"
"[{DESCRIPTION}]\t"
"[{LOGO}]\t"
"[{VISIBLE}]";
            searchFields = STR_ALLFIELDS;
            format = searchFields;

        } else if (arg == "-s" || arg == "--source") {
            searchFields += "\t[{SOURCE}]";
            format = searchFields;

        } else if (arg == "-a" || arg == "--addr") {
            searchFields = "[{ADDR}]\t[{NAME}]";
            format = "[{ADDR}]";

        } else if (arg == "-l" || arg == "--list") {
            exportFmt = JSON1;

        } else if (arg == "-m" || arg == "--matchCase") {
            matchCase = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            searches.push_back(arg);

        }
    }

    switch (exportFmt) {
        case NONE1: format = "[{ADDR}]\t[{NAME}]\t[{SYMBOL}]"; break;
        case API1:
        case JSON1: format = ""; break;
        case TXT1:
        case CSV1:
            format = getGlobalConfig()->getConfigStr("display", "format", format.empty() ? STR_DISPLAY : format);
            manageFields("CAccountName:" + cleanFmt(format, exportFmt));
            break;
    }
    expContext().fmtMap["nick"] = cleanFmt(format, exportFmt);

    applyFilter();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    items.clear();
    searches.clear();
    searchFields = STR_DISPLAY;
    matchCase = false;
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
COptions::~COptions(void) {
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
void COptions::applyFilter() {

    if (!matchCase)
        for (size_t i = 0 ; i < searches.size() ; i++)
            searches[i] = toLower(searches[i]);

    string_q format = searchFields;
    string_q search1 = searches.size() > 0 ? searches.at(0) : "";
    string_q search2 = searches.size() > 1 ? searches.at(1) : "";
    string_q search3 = searches.size() > 2 ? searches.at(2) : "";

    for (size_t i = 0 ; i < namedAccounts.size() ; i++) {
        string_q str = namedAccounts[i].Format(format);
        if (!matchCase)
            str = toLower(str);
        if ((search1.empty() || search1 == "*" || contains(str, search1)) &&
            (search2.empty() || search2 == "*" || contains(str, search2)) &&
            (search3.empty() || search3 == "*" || contains(str, search3))) {
            items.push_back(namedAccounts[i]);
        }
    }
}

const char* STR_DISPLAY =
"[{ADDR}]\t"
"[{SYMBOL}]\t"
"[{NAME}]";
