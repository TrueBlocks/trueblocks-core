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
    COption("-owned",     "Show owned addresses"),
    COption("-custom",    "Show custom addresses (see below)"),
    COption("-prefund",   "Show prefunded addresses"),
    COption("-named",     "Show named addresses (see ethName)"),
    COption("-addr_only", "export only addresses, no names"),
    COption("@fmt:<fmt>", "export format (one of [none|json|txt|csv|api])"),
    COption("",           "List known accounts ('owned' are shown by default)."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    string_q format = STR_DISPLAY;
    bool deflt = true, noHeader = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-n" || arg == "--named") {
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

        } else if (arg == "-a" || arg == "--addr_only") {
            noHeader = true;
            format = "[{ADDR}]";

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            return usage("Invalid option '" + arg + "'. Quitting...");
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
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (noHeader)
        expContext().fmtMap["header"] = "";

    applyFilter();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    items.clear();
    types = OWNED;

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
    if (which == "notes" && (verbose || COptions::isReadme))
        return "To customize this list add an [{custom}] section to the config file (see documentation).\n";
    return str;
}

//-----------------------------------------------------------------------
bool COptions::addIfUnique(const CAccountName& item) {
    if (isZeroAddr(item.addr))
        return false;

    address_t l = toLower(item.addr);
    if (items[l].addr == l) {
        if (!item.name.empty() && items[l].name != item.name) // last in wins
            items[l].name = item.name;
        return false;
    }
    items[l] = item;
    return true;
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
            for (auto addr : addrs) {
                CAccountName item(addr);
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
        CStringArray prefunds;
        asciiFileToLines(configPath("prefunds.txt"), prefunds);
        for (auto prefund : prefunds) {
            string_q addr = nextTokenClear(prefund,'\t');
            CAccountName item(addr);
            addIfUnique(item);
        }
    }
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY =
"[{ADDR}][\t{NAME}][\t{SYMBOL}]";
