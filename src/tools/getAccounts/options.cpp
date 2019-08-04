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
// BEG_CODE_OPTIONS
    COption2("terms", "", "list<term>", OPT_REQUIRED | OPT_POSITIONAL, "a space separated list of one or more search terms"),
    COption2("expand", "e", "", OPT_SWITCH, "expand search to include all fields (default searches name, address, and symbol only)"),
    COption2("matchCase", "m", "", OPT_SWITCH, "do case-sensitive search"),
    COption2("owned", "o", "", OPT_SWITCH, "Include personal accounts in the search"),
    COption2("custom", "c", "", OPT_SWITCH, "Include your custom named accounts"),
    COption2("prefund", "p", "", OPT_SWITCH, "Include prefund accounts"),
    COption2("named", "n", "", OPT_SWITCH, "Include well know token and airdrop addresses in the search"),
    COption2("addr", "a", "", OPT_SWITCH, "display only addresses in the results (useful for scripting)"),
    COption2("other", "t", "", OPT_HIDDEN | OPT_SWITCH, "export other addresses if found"),
    COption2("fmt", "x", "<fmt>", OPT_HIDDEN | OPT_FLAG, "export format (one of [none|json|txt|csv|api])"),
    COption2("", "", "", 0, "Query addresses and/or names well known accounts.\n"),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY;
extern const char* STR_DISPLAY_ALL;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool noHeader = false;
    string_q format;
    bool deflt = true;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-e" || arg == "--expand") {
            searchFields = STR_DISPLAY_ALL;
            format = searchFields;

        } else if (arg == "-m" || arg == "--matchCase") {
            matchCase = true;

        } else if (arg == "-n" || arg == "--named") {
            if (deflt) { types = 0; deflt = false; }
            types |= NAMED;

        } else if (arg == "-p" || arg == "--prefund") {
            if (deflt) { types = 0; deflt = false; }
            types |= PREFUND;

        } else if (arg == "-c" || arg == "--custom") {
            if (deflt) { types = 0; deflt = false; }
            types |= CUSTOM;

        } else if (arg == "-o" || arg == "--owned") {
            if (deflt) { types = 0; deflt = false; }
            types |= OWNED;

        } else if (arg == "-t" || arg == "--other") {
            if (deflt) { types = 0; deflt = false; }
            types |= OTHER;

        } else if (arg == "-a" || arg == "--addr") {
            noHeader = true;
            format = "[{ADDR}]";
            searchFields = "[{ADDR}]\t[{NAME}]";

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            searches.push_back(arg);

        }
    }
    if (verbose)
        searchFields += "\t[{SOURCE}]";

    // Data wrangling
    // None

    // Display formatting
    switch (exportFmt) {
        case NONE1:
        case TXT1:
        case CSV1:
            format = getGlobalConfig()->getConfigStr("display", "format", format.empty() ? STR_DISPLAY : format);
            if (verbose && !contains(format, "{SOURCE}"))
                format += "\t[{SOURCE}]";
            break;
        case API1:
        case JSON1:
            format = "";
            break;
    }
    manageFields("CAccountName:" + cleanFmt((format.empty() ? STR_DISPLAY_ALL : format), exportFmt));
    expContext().fmtMap["meta"] = ", \"namePath\": \"" + (isTestMode() ? "--" : getCachePath("names/")) + "\"";
    if (expContext().asEther)
        format = substitute(format, "{BALANCE}", "{ETHER}");
    if (expContext().asDollars)
        format = substitute(format, "{BALANCE}", "{DOLLARS}");
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (noHeader)
        expContext().fmtMap["header"] = "";

    // Collect results for later display
    applyFilter();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);

    items.clear();
    searches.clear();
    searchFields = STR_DISPLAY;
    matchCase = false;
    types = NAMED;
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
        ret += "With two search terms, the first term must match the [{address}] field, and the second term must match the [{name}] field.\n";
        ret += "When there are two search terms, both must match.\n";
        ret += "The [{--matchCase}] option requires case sensitive matching. It works with all other options.\n";
        ret += "To customize the list of names add a [{custom}] section to the config file (see documentation).\n";
        ret += "Name file: [{" + substitute(namesFile.getFullPath(), getHomeFolder(), "~/") + "}] (" + uint_2_Str(fileSize(namesFile.getFullPath())) + ")\n";
        return ret;
    }
    return str;
}

//-----------------------------------------------------------------------
bool COptions::addIfUnique(const CAccountName& item) {
    if (isZeroAddr(item.addr))
        return false;

    address_t l = toLower(item.addr);
    if (items[l].addr == l) {
        if (!item.name.empty() && (items[l].name != item.name || startsWith(items[l].name, "Owned_") || startsWith(items[l].name, "Prefund_"))) // last in wins
            items[l].name = item.name;
        return false;
    }

    if (!matchCase)
        for (size_t i = 0 ; i < searches.size() ; i++)
            searches[i] = toLower(searches[i]);

    string_q search1 = searches.size() > 0 ? searches[0] : "";
    string_q search2 = searches.size() > 1 ? searches[1] : "";
    string_q search3 = searches.size() > 2 ? searches[2] : "";

    string_q str = item.Format(searchFields);
    if (!matchCase)
        str = toLower(str);

    if ((search1.empty() || search1 == "*" || contains(str, search1)) &&
        (search2.empty() || search2 == "*" || contains(str, search2)) &&
        (search3.empty() || search3 == "*" || contains(str, search3))) {
        items[l] = item;
        return true;
    }
    items.erase(l);
    return false;
}

//-----------------------------------------------------------------------
void COptions::applyFilter() {

    //------------------------
    if (types & OWNED) {
        if (isTestMode()) {
            for (uint32_t i = 1 ; i < 5 ; i++) {
                CAccountName item;
                item.addr = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
                addIfUnique(item);
            }
        } else {
            CStringArray addrs;
            getAccounts(addrs);
            uint32_t cnt = 0;
            for (auto addr : addrs) {
                CAccountName item("00-Active\t" + addr + "\tOwned_" + padNum4(cnt++));
                addIfUnique(item);
            }
        }
    }

    //------------------------
    if (types & CUSTOM) {
        if (isTestMode()) {
            for (uint32_t i = 1 ; i < 5 ; i++) {
                CAccountName item;
                item.addr = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
                item.name = "Account_" + uint_2_Str(i);
                if (!(i%2))
                    item.symbol = "AC_" + uint_2_Str(i);
                addIfUnique(item);
            }
        } else {
            CAccountName item;
            string_q customStr = getGlobalConfig("getAccounts")->getConfigJson("custom", "list", "");
            while (item.parseJson3(customStr)) {
                addIfUnique(item);
                item = CAccountName();
            }
        }
    }

    //------------------------
    if (types & NAMED) {
        for (auto item : namedAccounts) {
            addIfUnique(item);
        }
    }

    //------------------------
    if (types & PREFUND) {
        uint32_t cnt = 0;
        ASSERT(prefunds.size() == 8893);  // This is a known value
        for (auto prefund : prefunds) {
            string_q addr = nextTokenClear(prefund,'\t');
            CAccountName item("80-Prefund\t" + addr + "\tPrefund_" + padNum4(cnt++));
            addIfUnique(item);
        }
    }

    //------------------------
    if (types & OTHER) {
        string_q contents = asciiFileToString("../src/tools/getAccounts/tests/other_names.txt");
        if (!contents.empty()) {
            CStringArray fields;
            fields.push_back("addr");
            fields.push_back("name");
            fields.push_back("source");
            CAccountName item;
            while (item.parseText(fields, contents)) {
                addIfUnique(item);
                item = CAccountName();
            }
        }
    }
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY =
"[{ADDR}]\t"
"[{NAME}]\t"
"[{SYMBOL}]";

//-----------------------------------------------------------------------
const char* STR_DISPLAY_ALL =
"[{GROUP}]\t"
"[{ADDR}]\t"
"[{NAME}]\t"
"[{SYMBOL}]\t"
"[{SOURCE}]\t"
"[{DESCRIPTION}]\t"
"[{LOGO}]\t"
"[{ISCONTRACT}]\t"
"[{CUSTOM}]\t"
"[{SHARED}]";
