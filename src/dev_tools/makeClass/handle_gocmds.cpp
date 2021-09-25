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
#include "options.h"

extern string_q get_optfields(const CCommandOption& cmd);
extern string_q get_setopts(const CCommandOption& cmd);
extern string_q get_copyopts(const CCommandOption& cmd);
extern string_q get_use(const CCommandOption& cmd);
extern map<string, string> chifraCmdMap;

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds(void) {
    LOG_INFO(cYellow, "handling go commands...", string_q(50, ' '), cOff);
    counter = CCounter();  // reset

    CCommandOptionArray endpointArray;
    forEveryLineInAsciiFile("../src/cmd-line-endpoints.csv", parseCommandData, &endpointArray);

    for (auto ep : endpointArray) {
        CCommandOptionArray params;
        for (auto option : optionArray)
            if (option.api_route == ep.api_route && option.isChifraRoute())
                params.push_back(option);
        ep.params = &params;
        if (ep.api_route == "abis" || ep.api_route == "when" || ep.api_route == "receipts" ||
            ep.api_route == "blocks" || ep.api_route == "names") {
            string_q source = asciiFileToString(configPath("makeClass/blank.go"));
            string_q path = substitute(getCommandPath(chifraCmdMap[ep.api_route]), "~/", "/Users/jrush/");
            replaceAll(source, "[{COPY_OPTS}]", get_copyopts(ep));
            replaceAll(source, "[{SET_OPTS}]", get_setopts(ep));
            replaceAll(source, "[{USE}]", get_use(ep));
            replaceAll(source, "[{ROUTE}]", toLower(ep.api_route));
            replaceAll(source, "[{PROPER}]", toProper(ep.api_route));
            replaceAll(source, "[{PATH}]", path);
            replaceAll(source, "[{OPT_FIELDS}]", get_optfields(ep));

            replaceAll(source, "[{LONG}]", "Purpose:\n  " + ep.description);
            string_q descr = firstLower(ep.description);
            if (endsWith(descr, "."))
                replaceReverse(descr, ".", "");
            replaceAll(source, "[{SHORT}]", descr);

            stringToAsciiFile("../src/apps/chifra-new/cmd/" + ep.api_route + ".go", source);
            counter.nProcessed++;
            counter.nVisited++;
        }
    }

    LOG_INFO(cYellow, "makeClass --gocmds", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    return true;
}

string_q get_use(const CCommandOption& cmd) {
    const char* STR_USE =
        "[{ROUTE}] [flags] <address> [address...]\n"
        "\n"
        "Arguments:\n"
        "[{POSITIONALS}]";
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type == "positional") {
            os << "  " << p.longName << " - " << p.description;
            if (p.is_required)
                os << " (required)";
            os << endl;
        }
    }
    string_q ret = STR_USE;
    replace(ret, "[{POSITIONALS}]", trim(os.str(), '\n'));
    return ret;
}

string_q goType(const string_q& typeIn) {
    string_q ret = typeIn;
    replace(ret, "string_q", "string");
    replace(ret, "uint64_t", "uint64");
    replace(ret, "blknum_t", "uint64");
    return ret;
}

string_q get_optfields(const CCommandOption& cmd) {
    size_t wid = 0;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional")
            wid = max(p.longName.length(), wid);
    }
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        string_q type = goType(p.real_type);
        if (p.option_type != "positional")
            os << "\t" << padRight(p.longName, wid) << " " << type << endl;
    }
    return os.str();
}

string_q goDefault(const CCommandOption& p) {
    string_q type = goType(p.real_type);
    if (type == "string")
        return "\"\"";
    else if (type == "uint64")
        return "0";
    return "false";
}

string_q get_setopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        string_q type = goType(p.real_type);
        if (p.option_type != "positional") {
            os << "\t[{ROUTE}]Cmd.Flags().";
            os << toProper(type);
            os << "VarP(&[{PROPER}]Opts.";
            os << p.Format("[{LONGNAME}], ");
            os << p.Format("\"[{LONGNAME}]\", ");
            os << p.Format("\"[{HOTKEY}]\", ");
            os << goDefault(p) << ", ";
            os << p.Format("\"[{DESCRIPTION}]\"");
            os << ")" << endl;
        }
    }
    return os.str();
}

string_q get_copyopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional") {
            string_q type = goType(p.real_type);
            if (type == "string") {
                os << "\tif len([{PROPER}]Opts." << p.longName << ") > 0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + [{PROPER}]Opts." << p.longName << endl;
            } else if (type == "uint64") {
                os << "\tif [{PROPER}]Opts." << p.longName << " > 0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + ";
                os << "strconv.FormatUint([{PROPER}]Opts." << p.longName << ", 10)" << endl;
            } else {
                os << "\tif [{PROPER}]Opts.";
                os << p.longName;
                os << " {" << endl;
                os << "\t\toptions += \" --" << p.longName << "\"" << endl;
            }
            os << "\t}" << endl;
        }
    }
    return os.str();
}

//------------------------------------------------------------------------------------------------
map<string, string> chifraCmdMap = {
    // BEG_CODE_CHIFRA_CMDMAP
    // -- Accounts
    {"list", "acctExport --appearances"},
    {"export", "acctExport"},
    {"monitors", "acctExport --appearances"},
    {"names", "ethNames"},
    {"abis", "grabABI"},
    // -- Chain Data
    {"blocks", "getBlocks"},
    {"transactions", "getTrans"},
    {"receipts", "getReceipts"},
    {"logs", "getLogs"},
    {"traces", "getTraces"},
    {"when", "whenBlock"},
    // -- Chain State
    {"state", "getState"},
    {"tokens", "getTokens"},
    // -- Admin
    {"status", "cacheStatus"},
    {"serve", "flame"},
    {"scrape", "blockScrape"},
    {"init", "pinMan local --init"},
    {"pins", "pinMan"},
    // -- Other
    {"quotes", "getQuotes"},
    {"explore", "fireStorm"},
    {"slurp", "ethslurp"},
    // END_CODE_CHIFRA_CMDMAP
};
