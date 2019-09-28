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
    COption("term_list", "", "list<term>", OPT_REQUIRED | OPT_POSITIONAL, "a space separated list of one or more search terms"),
    COption("expand", "e", "", OPT_SWITCH, "expand search to include all fields (default searches name, address, and symbol only)"),
    COption("matchCase", "m", "", OPT_SWITCH, "do case-sensitive search"),
    COption("owned", "o", "", OPT_SWITCH, "Include personal accounts in the search"),
    COption("custom", "c", "", OPT_SWITCH, "Include your custom named accounts"),
    COption("prefund", "p", "", OPT_SWITCH, "Include prefund accounts"),
    COption("named", "n", "", OPT_SWITCH, "Include well know token and airdrop addresses in the search"),
    COption("addr", "a", "", OPT_SWITCH, "display only addresses in the results (useful for scripting)"),
    COption("other", "t", "", OPT_HIDDEN | OPT_SWITCH, "export other addresses if found"),
    COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_HIDDEN | OPT_FLAG, "export format (one of [none|json*|txt|csv|api])"),
    COption("", "", "", OPT_DESCRIPTION, "Query addresses and/or names of well known accounts.\n"),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

string_q shortenFormat(const string_q& fmtIn);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    string_q format;
    bool no_header = false;
    bool deflt = true;
    bool addr_only = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-e" || arg == "--expand") {
            searchFields = STR_DISPLAY_ACCOUNTNAME;
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
            addr_only = true;
            no_header = true;
            format = "[{ADDRESS}]";
            searchFields = "[{ADDRESS}]\t[{NAME}]";

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

    // Display formatting
    switch (exportFmt) {
        case NONE1:
        case TXT1:
        case CSV1:
            format = getGlobalConfig("getAccounts")->getConfigStr("display", "format", format.empty() ? shortenFormat(STR_DISPLAY_ACCOUNTNAME) : format);
            if (verbose && !contains(format, "{SOURCE}"))
                format += "\t[{SOURCE}]";
            break;
        case API1:
        case JSON1:
            format = "";
            break;
    }
    manageFields("CAccountName:" + cleanFmt((format.empty() ? STR_DISPLAY_ACCOUNTNAME : format), exportFmt));
    expContext().fmtMap["meta"] = ", \"namePath\": \"" + (isTestMode() ? "--" : getCachePath("names/")) + "\"";
    if (expContext().asEther)
        format = substitute(format, "{BALANCE}", "{ETHER}");
    if (expContext().asDollars)
        format = substitute(format, "{BALANCE}", "{DOLLARS}");
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (no_header)
        expContext().fmtMap["header"] = "";

    // Collect results for later display
    applyFilter();

    // Data wrangling
    if (addr_only) {
        HIDE_FIELD(CAccountName, "group");
        HIDE_FIELD(CAccountName, "subgroup");
        HIDE_FIELD(CAccountName, "name");
        HIDE_FIELD(CAccountName, "symbol");
        HIDE_FIELD(CAccountName, "description");
        HIDE_FIELD(CAccountName, "source");
        HIDE_FIELD(CAccountName, "logo");
        HIDE_FIELD(CAccountName, "is_contract");
        HIDE_FIELD(CAccountName, "is_private");
        HIDE_FIELD(CAccountName, "is_shared");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT);

    items.clear();
    searches.clear();
    searchFields = shortenFormat(STR_DISPLAY_ACCOUNTNAME);
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
        return substitute(str, "term_list", "<term> [term...]");

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
    if (isZeroAddr(item.address))
        return false;

    address_t key = toLower(item.address);
    if (items[key].address == key) {  // it's already in the map, but we want the last in name to win
        if (!item.name.empty() &&
            (items[key].name != item.name || startsWith(items[key].name, "Owned_") || startsWith(items[key].name, "Prefund_"))) {
            items[key].name = item.name;
        }
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
        items[key] = item;
        return true;
    }
    items.erase(key);
    return false;
}

//-----------------------------------------------------------------------
void COptions::applyFilter() {

    //------------------------
    if (types & OWNED) {
        if (isTestMode()) {
            for (uint32_t i = 1 ; i < 5 ; i++) {
                CAccountName item;
                item.address = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
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
                item.group = "81-Custom";
                item.address = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
                item.name = "Account_" + uint_2_Str(i);
                if (!(i%2))
                    item.symbol = "AC_" + uint_2_Str(i);
                item.source = "Testing";
                addIfUnique(item);
            }
        } else {
            CAccountName item;
            string_q customStr = getGlobalConfig("getAccounts")->getConfigJson("custom", "list", "");
            while (item.parseJson3(customStr)) {
                if (item.group.empty())
                    item.group = "81-Custom";
                item.name = trim(item.name, '\"');
                item.description = trim(item.description, '\"');
                addIfUnique(item);
                item = CAccountName();
            }
        }
    }

    //------------------------
    if (types & NAMED) {
        for (auto item : namedAccounts) {
            if (isTestMode() && items.size() > 200)
                break;
            addIfUnique(item);
        }
    }

    //------------------------
    if (types & PREFUND) {
        uint32_t cnt = 0;
        ASSERT(prefunds.size() == 8893);  // This is a known value

        for (auto prefund : prefundWeiMap) {
            if (isTestMode() && items.size() > 200)
                break;
            CAccountName item;
            item.group = "80-Prefund";
            item.address = prefund.first;
            item.name = "Prefund_" + padNum4(cnt++);
            item.source = "Genesis";
            addIfUnique(item);
        }
    }

    //------------------------
    if (!isTestMode() && (types & OTHER)) {
        string_q contents = asciiFileToString(configPath("names/names_custom.txt"));
        if (!contents.empty()) {
            CStringArray fields;
            fields.push_back("group");
            fields.push_back("subgroup");
            fields.push_back("address");
            fields.push_back("name");
            fields.push_back("description");
            fields.push_back("symbol");
            fields.push_back("source");
            fields.push_back("logo");
            fields.push_back("is_contract");
            fields.push_back("is_private");
            fields.push_back("is_shared");
            CAccountName item;
            while (item.parseText(fields, contents)) {
                addIfUnique(item);
                item = CAccountName();
            }
        }
    }
}

//-----------------------------------------------------------------------
string_q shortenFormat(const string_q& fmtIn) {
    string_q ret = toUpper(fmtIn);
//    replace(ret, "[{GROUP}]", "");
//    replace(ret, "[{SUBGROUP}]", "");
//    replace(ret, "[{ADDRESS}]", "");
//    replace(ret, "[{NAME}]", "");
//    replace(ret, "[{SYMBOL}]", "");
    replace(ret, "[{SOURCE}]", "");
    replace(ret, "[{DESCRIPTION}]", "");
    replace(ret, "[{LOGO}]", "");
    replace(ret, "[{IS_CONTRACT}]", "");
    replace(ret, "[{IS_PRIVATE}]", "");
    replace(ret, "[{IS_SHARED}]", "");
    while (startsWith(ret, "\t"))
        replace(ret, "\t", "");
    while (endsWith(ret, "\t"))
        replaceReverse(ret, "\t", "");
    return ret;
}
