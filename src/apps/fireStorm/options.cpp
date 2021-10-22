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
#define LOGGING_LEVEL_TEST
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("terms", "", "list<string>", OPT_POSITIONAL, "one or more address, name, block, or transaction identifier"),
    COption("local", "l", "", OPT_SWITCH, "open the local TrueBlocks explorer"),
    COption("google", "g", "", OPT_SWITCH, "search google excluding popular blockchain explorers"),
    COption("", "", "", OPT_DESCRIPTION, "Open a local or remote explorer for one or more addresses, blocks, or transactions."),  // NOLINT
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--local") {
            local = true;

        } else if (arg == "-g" || arg == "--google") {
            google = true;

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

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("terms", terms, terms.empty());
    LOG_TEST_BOOL("local", local);
    LOG_TEST_BOOL("google", google);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    if (isApiMode())
        return usage("This command is not available under API mode.");

    if (google && local)
        return usage("Choose either --google or --local, not both.");

    for (auto term : terms) {
        term = toLower(term);
        if (google && !isAddress(term))
            return usage("Option --google allows only an address term.");
    }

    for (auto term : terms) {
        term = toLower(term);
        string_q url = getBaseUrl(local ? "local" : google ? "google" : "remote");
        if (isAddress(term)) {
            if (google) {
                term = term + getExlusions();
            } else {
                term = "address/" + term;
            }

        } else if (isHash(term)) {
            CTransaction trans;
            getTransaction(trans, term);
            if (trans.hash == term) {
                term = "tx/" + term;
            } else {
                term = "block/" + term;
            }

        } else if (isFourByte(term)) {
            url = getBaseUrl("fourByte");
            term = "signatures/?bytes4_signature=" + term;

        } else if (endsWith(term, ".eth")) {
            url = getBaseUrl("remote");
            term = "enslookup-search?search=" + term;

        } else if (contains(term, ".")) {
            CUintArray parts;
            explode(parts, term, '.');
            CTransaction trans;
            getTransaction(trans, parts[0], parts[1]);
            term = "tx/" + trans.hash;

        } else {
            term = "block/" + term;
        }
        if (local) {
            replace(term, "tx/", "explorer/transactions/");
            replace(term, "block/", "explorer/blocks/");
            replace(term, "address/", "dashboard/accounts?address=");
        }
        urls.push_back(url + term);
    }
    if (urls.empty())
        urls.push_back(getBaseUrl(local ? "local" : "remote"));

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params, NOOPT, OPT_FMT | OPT_VERBOSE);

    // BEG_CODE_INIT
    local = false;
    google = false;
    // END_CODE_INIT

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::getExlusions(void) {
    if (!google)
        return "";

    CStringArray exclusions = {
        "etherscan", "etherchain", "bloxy", "bitquery", "ethplorer", "tokenview", "anyblocks", "explorer",
    };
    ostringstream ret;
    for (auto ex : exclusions)
        ret << "+-" << ex;

    return ret.str();
}

//----------------------------------------------------------------
string_q getBaseUrl(const string_q& type) {
    string_q ret;
    if (type == "remote") {
        ret = "https://etherscan.io/";
    } else if (type == "fourByte") {
        ret = "https://www.4byte.directory/";
    } else if (type == "google") {
        ret = "https://www.google.com/search?q=";
    } else {
        ASSERT(type == "local");
        ret = getGlobalConfig("fireStorm")->getConfigStr("settings", "baseURL", "http://localhost:1234/");
    }
    if (!endsWith(ret, "/"))
        ret += "/";
    return ret;
}
