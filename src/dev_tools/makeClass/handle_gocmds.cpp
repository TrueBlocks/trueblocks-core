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
        if (!ep.is_visible)
            continue;
        CCommandOptionArray params;
        for (auto option : optionArray)
            if (option.api_route == ep.api_route && option.isChifraRoute())
                params.push_back(option);
        ep.params = &params;
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
        string_q imports;
        if (contains(source, "errors."))
            imports += "\t\"errors\"\n";
        if (contains(source, "fmt."))
            imports += "\t\"fmt\"\n";
        replaceAll(source, "[{IMPORTS}]", imports);

        counter.nProcessed += writeIfDifferent("../src/apps/chifra/cmd/" + ep.api_route + ".go", source);
        counter.nVisited++;
    }

    LOG_INFO(cYellow, "makeClass --gocmds", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    return true;
}

string_q get_use(const CCommandOption& cmd) {
    ostringstream arguments;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type == "positional") {
            if (arguments.str().empty())
                arguments << endl << "Arguments:" << endl;
            else
                arguments << endl;
            arguments << substitute(p.Format("  [{LONGNAME}] - [{DESCRIPTION}]"), "addrs2", "addrs");
            if (p.is_required)
                arguments << " (required)";
        }
    }

    ostringstream positionals;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type == "positional") {
            if (!positionals.str().empty())
                positionals << " ";
            positionals << p.data_type;
        }
    }
    string_q ret = "[{ROUTE}] [flags][{TYPES}][{POSITIONALS}]";
    replace(ret, "[{TYPES}]", clean_positionals(cmd.api_route, positionals.str()));
    replace(ret, "[{POSITIONALS}]", arguments.str());
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
        if (p.option_type != "positional")
            os << "\t" << padRight(p.longName, wid) << " " << p.go_type << endl;
    }
    return os.str();
}

string_q goDefault(const CCommandOption& p) {
    if (p.go_type == "[]string")
        return "nil";
    else if (p.go_type == "float64")
        return "0.0";
    else if (p.go_type == "string")
        return "\"\"";
    else if (p.go_type == "uint64")
        return "0";
    return "false";
}

string_q get_setopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional") {
            os << "\t[{ROUTE}]Cmd.Flags().";
            os << p.go_flagtype;
            os << "(&[{PROPER}]Opts.";
            os << p.Format("[{LONGNAME}], ");
            os << p.Format("\"[{LONGNAME}]\", ");
            os << p.Format("\"[{HOTKEY}]\", ");
            os << goDefault(p) << ", ";
            if (p.is_visible)
                os << p.Format("\"[{DESCRIPTION}]\"");
            else
                os << p.Format("\"[{DESCRIPTION}] (hidden)\"");
            os << ")" << endl;
        }
    }
    return os.str();
}

string_q get_copyopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional") {
            if (p.go_type == "[]string") {
                os << "\tif len([{PROPER}]Opts." << p.longName << ") > 0 {" << endl;
                os << "\t\t// TODO(tjayrush): this loses the remaining items after the first" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + [{PROPER}]Opts." << p.longName << "[0]" << endl;
            } else if (p.go_type == "string") {
                os << "\tif len([{PROPER}]Opts." << p.longName << ") > 0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + [{PROPER}]Opts." << p.longName << endl;
            } else if (p.go_type == "uint64" || p.go_type == "uint32") {
                os << "\tif [{PROPER}]Opts." << p.longName << " > 0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + ";
                os << "fmt.Sprintf(\"%d\", [{PROPER}]Opts." << p.longName << ")" << endl;
            } else if (p.go_type == "float64") {
                os << "\tif [{PROPER}]Opts." << p.longName << " > 0.0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + ";
                os << "fmt.Sprintf(\"%.1f\", [{PROPER}]Opts." << p.longName << ")" << endl;
            } else {
                os << "\tif [{PROPER}]Opts." << p.longName << " {" << endl;
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
