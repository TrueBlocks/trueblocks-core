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

extern string_q get_hidden(const CCommandOption& cmd);
extern string_q get_hidden2(const CCommandOption& cmd);
extern string_q get_notes2(const CCommandOption& cmd);
extern string_q get_optfields(const CCommandOption& cmd);
extern string_q get_logopts(const CCommandOption& cmd);
extern string_q get_setopts(const CCommandOption& cmd);
extern string_q get_copyopts(const CCommandOption& cmd);
extern string_q get_use(const CCommandOption& cmd);

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds(void) {
    LOG_INFO(cYellow, "handling go commands...", string_q(50, ' '), cOff);
    counter = CCounter();  // reset

    for (auto ep : endpointArray) {
        if (!ep.is_visible)
            continue;
        CCommandOptionArray params;
        CCommandOptionArray notes;
        for (auto option : routeOptionArray) {
            bool isOne = option.api_route == ep.api_route && option.isChifraRoute(true);
            if (isOne) {
                params.push_back(option);
            }
            if (option.api_route == ep.api_route && option.option_type == "note")
                notes.push_back(option);
        }
        ep.params = &params;
        ep.notes = &notes;
        string_q source = asciiFileToString(getTemplatePath("blank.go"));
        replaceAll(source, "[{COPY_OPTS}]", get_copyopts(ep));
        replaceAll(source, "[{SET_OPTS}]", get_setopts(ep));
        replaceAll(source, "[{LOG_OPTS}]", get_logopts(ep));
        replaceAll(source, "[{HIDDEN}]", get_hidden(ep));
        replaceAll(source, "[{PERPRERUN}]", get_hidden2(ep));
        replaceAll(source, "[{USE}]", get_use(ep));
        replaceAll(source, "[{ROUTE}]", toLower(ep.api_route));
        replaceAll(source, "[{PROPER}]", toProper(ep.api_route));
        replaceAll(source, "[{OPT_FIELDS}]", get_optfields(ep));
        replaceAll(source, "[{LONG}]", "Purpose:\n  " + ep.description);
        replaceAll(source, "[{POSTNOTES}]", get_notes2(ep));
        string_q descr = firstLower(ep.description);
        if (endsWith(descr, "."))
            replaceReverse(descr, ".", "");
        replaceAll(source, "[{SHORT}]", descr);
        string_q imports;
        if (contains(source, "errors."))
            imports += "\t\"errors\"\n";
        if (contains(source, "fmt."))
            imports += "\t\"fmt\"\n";
        if (contains(source, "utils."))
            imports += "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils\"\n";
        replaceAll(source, "[{IMPORTS}]", imports);

        string_q fn = getSourcePath("apps/chifra/cmd/" + ep.api_route + ".go");
        codewrite_t cw(fn, source);
        cw.nSpaces = 0;
        cw.stripEOFNL = false;
        counter.nProcessed += writeCodeIn(cw);
        counter.nVisited++;
    }

    LOG_INFO(cYellow, "makeClass --gocmds", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    return true;
}

bool visitEnumItem2(string_q& item, void* data) {
    ostringstream* osp = (ostringstream*)data;
    if (osp->str().empty())
        *osp << endl << "One of [ ";
    else
        *osp << " | ";
    *osp << item;
    return true;
}

string_q get_use(const CCommandOption& cmd) {
    ostringstream arguments;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        if (p.option_type == "positional") {
            if (arguments.str().empty())
                arguments << endl << "Arguments:" << endl;
            else
                arguments << endl;
            arguments << substitute(p.Format("  [{LONGNAME}] - [{DESCRIPTION}]"), "addrs2", "addrs");
            if (p.is_required)
                arguments << " (required)";
            if (contains(p.data_type, "enum")) {
                ostringstream os;
                forEveryEnum(visitEnumItem2, p.data_type, &os);
                os << " ]";
                arguments << substitute(os.str(), "One of",
                                        contains(p.data_type, "list") ? "\tOne or more of" : "\tOne of");
            }
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

string_q get_notes2(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.notes)) {
        if (os.str().empty()) {
            os << endl;
            os << "Notes:" << endl;
        } else {
            os << endl;
        }
        os << "  - " << substitute(p.description, "`", "");
    }

    return trim(substitute(os.str(), "|", "\n    "));
}

// TODO: search for go-port
#define upperCaseGoOpts (p.api_route == "scrape")
string_q get_optfields(const CCommandOption& cmd) {
    size_t wid = 0;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        if (p.option_type != "positional") {
            wid = max(p.longName.length(), wid);
        }
    }
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        if (p.option_type != "positional") {
            if (upperCaseGoOpts) {
                os << "\t" << padRight(toProper(p.longName), wid) << " " << p.go_type << endl;
            } else {
                os << "\t" << padRight(p.longName, wid) << " " << p.go_type << endl;
            }
        }
    }
    return os.str();
}

string_q get_goDefault(const CCommandOption& p) {
    if (p.go_type == "[]string") {
        return "nil";
    } else if (p.go_type == "float64") {
        if (!p.def_val.empty())
            return p.def_val;
        return "0.0";
    } else if (p.go_type == "string") {
        return p.def_val;
    } else if (p.go_type == "uint64") {
        if (p.def_val == "NOPOS")
            return "0";
        else if (!p.def_val.empty() && !startsWith(p.def_val, "("))
            return p.def_val;
        return "0";
    }
    return "false";
}

string_q get_goDescription(const CCommandOption& cmd) {
    string_q addendum;
    if (contains(cmd.data_type, "enum")) {
        ostringstream os;
        forEveryEnum(visitEnumItem2, cmd.data_type, &os);
        os << " ]";
        addendum += substitute(os.str(), "One of", contains(cmd.data_type, "list") ? "One or more of" : "One of");
    }

    string_q fmt = "[{DESCRIPTION}]";
    string_q ret = cmd.Format(fmt);
    if (contains(ret, " (default")) {
        replace(ret, " (default", "#");
        ret = nextTokenClear(ret, '#');
    }
    ret += cmd.is_visible ? "" : " (hidden)";
    ret += addendum;
    ret = "\"" + ret + "\"";
    if (!addendum.empty())
        replaceAll(ret, "\"", "`");
    return ret;
}

string_q get_hidden2(const CCommandOption& cmd) {
    // if (cmd.api_route != "scrape")
    return "";
    // ostringstream os;
    // os << "\tPersistentPreRun: func(cmd *cobra.Command, args []string) {" << endl;
    // os << "\t\trootCmd.PersistentFlags().MarkHidden(\"fmt\")" << endl;
    // os << "\t}," << endl;
    // return os.str();
}

string_q get_hidden(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        if (!p.is_visible) {
            os << "\t\t[{ROUTE}]Cmd.Flags().MarkHidden(\"" + p.Format("[{LONGNAME}]") + "\")" << endl;
        }
    }
    ostringstream hide;
    if (os.str().empty()) {
        return "";
    }

    ostringstream ret;
    ret << "\tif !utils.IsTestMode() {" << endl;
    ret << os.str();
    ret << "\t}" << endl;
    return ret.str();
}

string_q get_logopts(const CCommandOption& cmd) {
    if (cmd.api_route != "explore")
        return "";
    const char* STR_RET =
        "	utils.TestLogArgs(\"terms\", args)\n"
        "	utils.TestLogBool(\"local\", ExploreOpts.local)\n"
        "	utils.TestLogBool(\"google\", ExploreOpts.google)\n";
    return STR_RET;
}

string_q get_setopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional") {
            os << "\t[{ROUTE}]Cmd.Flags().";
            os << p.go_flagtype;
            os << "(&[{PROPER}]Opts.";
            replace(p.longName, "deleteMe", "delete");
            if (upperCaseGoOpts) {
                os << toProper(p.longName) << ", ";
            } else {
                os << p.Format("[{LONGNAME}], ");
            }
            os << p.Format("\"[{LONGNAME}]\", ");
            os << p.Format("\"[{HOTKEY}]\", ");
            os << get_goDefault(p) << ", ";
            os << get_goDescription(p);
            os << ")" << endl;
        }
    }
    return os.str();
}

string_q get_copyopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        if (p.option_type != "positional" && !p.isDeprecated) {
            if (p.go_type == "[]string") {
                os << "\tfor _, t := range [{PROPER}]Opts." << p.longName << " {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + t" << endl;
                os << "\t}" << endl;
            } else if (p.go_type == "string") {
                os << "\tif len([{PROPER}]Opts." << p.longName << ") > 0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + [{PROPER}]Opts." << p.longName << endl;
                os << "\t}" << endl;
            } else if (p.go_type == "uint64" || p.go_type == "uint32") {
                os << "\tif [{PROPER}]Opts." << p.longName << " > 0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + ";
                os << "fmt.Sprintf(\"%d\", [{PROPER}]Opts." << p.longName << ")" << endl;
                os << "\t}" << endl;
            } else if (p.go_type == "float64") {
                os << "\tif [{PROPER}]Opts." << p.longName << " > 0.0 {" << endl;
                os << "\t\toptions += \" --" << p.longName << " \" + ";
                os << "fmt.Sprintf(\"%.1f\", [{PROPER}]Opts." << p.longName << ")" << endl;
                os << "\t}" << endl;
            } else {
                os << "\tif [{PROPER}]Opts." << p.longName << " {" << endl;
                os << "\t\toptions += \" --" << p.longName << "\"" << endl;
                os << "\t}" << endl;
            }
        }
    }
    return os.str();
}
