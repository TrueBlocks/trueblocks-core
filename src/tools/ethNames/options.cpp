/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"
#include "exportcontext.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("terms", "", "list<string>", OPT_REQUIRED | OPT_POSITIONAL, "a space separated list of one or more search terms"),  // NOLINT
    COption("expand", "e", "", OPT_SWITCH, "expand search to include all fields (search name, address, and symbol otherwise)"),  // NOLINT
    COption("match_case", "m", "", OPT_SWITCH, "do case-sensitive search"),
    COption("all", "l", "", OPT_SWITCH, "include all accounts in the search"),
    COption("custom", "c", "", OPT_SWITCH, "include your custom named accounts"),
    COption("prefund", "p", "", OPT_SWITCH, "include prefund accounts"),
    COption("named", "n", "", OPT_SWITCH, "include well know token and airdrop addresses in the search"),
    COption("addr", "a", "", OPT_SWITCH, "display only addresses in the results (useful for scripting)"),
    COption("tags", "g", "", OPT_SWITCH, "export the list of tags and subtags only"),
    COption("to_custom", "u", "", OPT_HIDDEN | OPT_SWITCH, "for editCmd only, is the edited name a custom name or not"),
    COption("clean", "C", "", OPT_HIDDEN | OPT_SWITCH, "clean the data (addrs to lower case, sort by addr)"),
    COption("autoname", "A", "<string>", OPT_HIDDEN | OPT_FLAG, "an address assumed to be a token, added automatically to names database if true"),  // NOLINT
    COption("create", "", "", OPT_HIDDEN | OPT_SWITCH, "create a new name record"),
    COption("update", "", "", OPT_HIDDEN | OPT_SWITCH, "edit an existing name"),
    COption("delete", "", "", OPT_HIDDEN | OPT_SWITCH, "delete a name, but do not remove it"),
    COption("undelete", "", "", OPT_HIDDEN | OPT_SWITCH, "undelete a previously deleted name"),
    COption("remove", "", "", OPT_HIDDEN | OPT_SWITCH, "remove a previously deleted name"),
    COption("", "", "", OPT_DESCRIPTION, "Query addresses or names of well known accounts."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

string_q shortenFormat(const string_q& fmtIn);
string_q getSearchFields(const string_q& fmtIn);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    replaceAll(command, "--delete", "--deleteMe");

    // BEG_CODE_LOCAL_INIT
    bool expand = false;
    bool all = false;
    bool custom = false;
    bool named = false;
    bool addr = false;
    bool clean = false;
    string_q autoname = "";
    bool create = false;
    bool update = false;
    bool deleteMe = false;
    bool undelete = false;
    bool remove = false;
    // END_CODE_LOCAL_INIT

    string_q format;
    bool deflt = true;
    bool addr_only = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-e" || arg == "--expand") {
            expand = true;

        } else if (arg == "-m" || arg == "--match_case") {
            match_case = true;

        } else if (arg == "-l" || arg == "--all") {
            all = true;

        } else if (arg == "-c" || arg == "--custom") {
            custom = true;

        } else if (arg == "-p" || arg == "--prefund") {
            prefund = true;

        } else if (arg == "-n" || arg == "--named") {
            named = true;

        } else if (arg == "-a" || arg == "--addr") {
            addr = true;

        } else if (arg == "-g" || arg == "--tags") {
            tags = true;

        } else if (arg == "-u" || arg == "--to_custom") {
            to_custom = true;

        } else if (arg == "-C" || arg == "--clean") {
            clean = true;

        } else if (startsWith(arg, "-A:") || startsWith(arg, "--autoname:")) {
            autoname = substitute(substitute(arg, "-A:", ""), "--autoname:", "");
        } else if (arg == "-A" || arg == "--autoname") {
            return flag_required("autoname");

        } else if (arg == "--create") {
            create = true;

        } else if (arg == "--update") {
            update = true;

        } else if (arg == "--deleteMe") {
            deleteMe = true;

        } else if (arg == "--undelete") {
            undelete = true;

        } else if (arg == "--remove") {
            remove = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseStringList2(this, terms, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (create)
        crudCommands.push_back("create");

    if (update)
        crudCommands.push_back("update");

    if (deleteMe)
        crudCommands.push_back("delete");

    if (undelete)
        crudCommands.push_back("undelete");

    if (remove)
        crudCommands.push_back("remove");

    if (prefund) {
        if (!loadNamesWithPrefunds())
            return usage("Could not load names database.");
    } else {
        if (!loadNames())
            return usage("Could not load names database.");
    }

    if (!autoname.empty() && (!isAddress(autoname) || isZeroAddr(autoname)))
        return usage("You must provide an address to the --autoname option.");

    // for (auto& term : terms)
    //     if (endsWith(term, ".eth"))
    //         term = addressFromENSName(term);

    if (clean) {
        latestBlock = isTestMode() ? 10800000 : getLatestBlock_client();
        abi_spec.loadAbisFromKnown(true);
        return handle_clean();
    }

    if (isCrudCommand()) {
        if (prefund)
            return usage("You may not use the --prefund option when editing names.");
        abi_spec.loadAbisFromKnown(true);
        address_t address = toLower(trim(getEnvStr("TB_NAME_ADDRESS"), '\"'));
        if (address.empty() && !terms.empty())
            address = terms[0];
        if (!isAddress(address) || isZeroAddr(address))
            return usage("You must provide an address to crud commands.");
        if (!handle_editcmds(false))  // returns true on success
            return false;

    } else if (!autoname.empty()) {
        abi_spec.loadAbisFromKnown(true);
        crudCommands.push_back("create");
        terms.push_back(autoname);
        // ::setenv("TB_NAME_ADDRESS", autoname.c_str(), true);
        ::setenv("TB_NAME_NAME", addr_2_Petname(autoname, '-').c_str(), true);
        ::setenv("TB_NAME_TAG", "50-Tokens:ERC20", true);
        ::setenv("TB_NAME_SOURCE", "TrueBlocks.io", true);
        ::setenv("TB_NAME_SYMBOL", "", true);
        ::setenv("TB_NAME_DECIMALS", "18", true);
        ::setenv("TB_NAME_CUSTOM", "false", true);
        if (!handle_editcmds(true))  // returns true on success
            return false;
    }

    for (auto term : terms)
        searches.push_back(term);

#define anyBase() (match_case || expand || all || prefund || named || addr || to_custom || clean)
    if (expand) {
        searchFields = STR_DISPLAY_NAME;
        format = searchFields;
    }

    if (all)
        types = ALL;
    if (named) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= NAMED;
    }
    if (prefund) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= PREFUND;
    }
    if (custom) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= CUSTOM;
    }

    if (addr) {
        addr_only = true;
        noHeader = true;
        format = "[{ADDRESS}]";
        searchFields = "[{ADDRESS}]\t[{NAME}]";
    }

    if (verbose)
        searchFields += "\t[{SOURCE}]";

    if (tags) {
        if (anyBase())
            return usage("Do not use the --tags option with any other option.");
        manageFields("CName:all", false);
        manageFields("CName:tags", true);
        format = "[{TAGS}]";
        addr_only = false;
        if (custom) {
            types = 0;
            types |= CUSTOM;
        } else {
            types |= NAMED;
            types |= PREFUND;
            types |= CUSTOM;
        }
    }

    // Prepare formatting
    string_q str = (format.empty() ? shortenFormat(STR_DISPLAY_NAME) : format);
    if (verbose && !contains(format, "{SOURCE}"))
        str += "\t[{SOURCE}]";
    string_q meta = ", \"namePath\": \"" + (isTestMode() ? "--" : cacheFolder_names) + "\"";

    // Display formatting
    configureDisplay("ethNames", "CName", str, meta);
    if (!tags && expContext().exportFmt == JSON1)
        manageFields("CName:" + cleanFmt(STR_DISPLAY_NAME));
    if (!expand) {
        HIDE_FIELD(CName, "deleted");
        HIDE_FIELD(CName, "isCustom");
        HIDE_FIELD(CName, "isPrefund");
        HIDE_FIELD(CName, "isContract");
        HIDE_FIELD(CName, "isErc20");
        HIDE_FIELD(CName, "isErc721");
    }

    // Collect results for later display
    filterNames();

    // Data wrangling
    if (addr_only) {
        HIDE_FIELD(CName, "deleted");
        HIDE_FIELD(CName, "tags");
        HIDE_FIELD(CName, "name");
        HIDE_FIELD(CName, "symbol");
        HIDE_FIELD(CName, "petname");
        HIDE_FIELD(CName, "source");
        HIDE_FIELD(CName, "decimal");
        HIDE_FIELD(CName, "isCustom");
        HIDE_FIELD(CName, "isPrefund");
        HIDE_FIELD(CName, "isContract");
        HIDE_FIELD(CName, "isErc20");
        HIDE_FIELD(CName, "isErc721");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_PREFUND);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    match_case = false;
    prefund = false;
    tags = false;
    to_custom = false;
    // END_CODE_INIT

    terms.clear();
    items.clear();
    searches.clear();
    searchFields = getSearchFields(STR_DISPLAY_NAME);
    types = NAMED;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    establishFolder(cacheFolder_names);

    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The tool will accept up to three terms, each of which must match against any field in the database.");  // NOLINT
    notes.push_back("The `--match_case` option enables case sensitive matching.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool COptions::addIfUnique(const CName& item) {
    if (isZeroAddr(item.address))
        return false;

    if (isTestMode() && !isCrudCommand()) {
        if (items.size() > 200)
            return true;
        if ((contains(item.tags, "Kickback") || contains(item.tags, "Humanity")))  // don't expose people during testing
            return true;
    }

    if (tags) {
        string_q key = item.tags;
        if (items[key].tags == key)  // already exists
            return false;
        items[key] = item;
        return true;
    }

    address_t key = toLower(item.address);
    if (items[key].address == key) {  // it's already in the map, but we want the last in name to win
        bool empty = item.name.empty();
        bool isDifferent = items[key].name != item.name;
        bool isPrefund = startsWith(items[key].name, "Prefund_");
        if (!empty && (isDifferent || isPrefund)) {
            items[key].name = item.name;
        }
        return false;
    }

    if (searches.size() == 0) {
        items[key] = item;
        return true;
    }

    if (!match_case) {
        for (size_t i = 0; i < searches.size(); i++) {
            searches[i] = toLower(searches[i]);
        }
    }

    string_q search1 = searches.size() > 0 ? searches[0] : "";
    string_q search2 = searches.size() > 1 ? searches[1] : "";
    string_q search3 = searches.size() > 2 ? searches[2] : "";

    string_q str = item.Format(searchFields);
    if (!match_case)
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
bool addCustom(NameOnDisc* item, void* data) {
    COptions* opts = (COptions*)data;
    if (item->flags & IS_CUSTOM) {
        CName name;
        item->disc_2_Name(name);
        opts->addIfUnique(name);
    }
    return true;
}

//-----------------------------------------------------------------------
bool addRegular(CName& item, void* data) {
    COptions* opts = (COptions*)data;
    if (!item.isCustom && !item.isPrefund)
        opts->addIfUnique(item);
    return true;
}

//-----------------------------------------------------------------------
bool addPrefund(NameOnDisc* item, void* data) {
    COptions* opts = (COptions*)data;
    if (item->flags & IS_PREFUND) {
        CName name;
        item->disc_2_Name(name);
        opts->addIfUnique(name);
    }
    return true;
}

//-----------------------------------------------------------------------
// order matters...first in wins...
void COptions::filterNames() {
    if (types & CUSTOM) {
        if (isTestMode() && !isCrudCommand()) {
            for (uint32_t i = 1; i < 5; i++) {
                CName item;
                item.tags = "81-Custom";
                item.address = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
                item.petname = addr_2_Petname(item.address, '-');
                item.name = "Account_" + uint_2_Str(i);
                if (!(i % 2)) {
                    item.symbol = "AC_" + uint_2_Str(i);
                    item.decimals = i;
                }
                item.source = "Testing";
                addIfUnique(item);
            }
        } else {
            forEveryName(addCustom, this);
        }
    }

    if (types & NAMED) {
        forEveryNameOld(addRegular, this);
    }

    if (types & PREFUND) {
        forEveryName(addPrefund, this);
    }
}

//-----------------------------------------------------------------------
string_q shortenFormat(const string_q& fmtIn) {
    string_q ret = toUpper(fmtIn);
    replace(ret, "[{SOURCE}]", "");
    replace(ret, "[{DECIMAL}]", "");
    replace(ret, "[{DELETED}]", "");
    replace(ret, "[{ISCUSTOM}]", "");
    replace(ret, "[{ISPREFUND}]", "");
    replace(ret, "[{ISCONTRACT}]", "");
    replace(ret, "[{ISERC20}]", "");
    replace(ret, "[{ISERC721}]", "");
    return trim(ret, '\t');
}

//-----------------------------------------------------------------------
string_q getSearchFields(const string_q& fmtIn) {
    string_q ret = toUpper(fmtIn);
    replace(ret, "[{SOURCE}]", "");
    replace(ret, "[{DECIMAL}]", "");
    replace(ret, "[{DELETED}]", "");
    replace(ret, "[{ISCUSTOM}]", "");
    replace(ret, "[{ISPREFUND}]", "");
    replace(ret, "[{ISCONTRACT}]", "");
    replace(ret, "[{ISERC20}]", "");
    replace(ret, "[{ISERC721}]", "");
    return trim(ret, '\t');
}
