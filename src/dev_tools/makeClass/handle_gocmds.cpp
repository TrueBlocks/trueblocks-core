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
extern string_q get_testlogs(const CCommandOption& cmd);
extern string_q get_copyopts(const CCommandOption& cmd);
extern string_q get_use(const CCommandOption& cmd);
extern const char* STR_REPLACE_OPTS;

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_cmd(const CCommandOption& p) {
    string_q source = asciiFileToString(getTemplatePath("blank.go"));
    replaceAll(source, "[{LONG}]", "Purpose:\n  " + p.description);
    replaceAll(source, "[{OPT_DEF}]", "");
    replaceAll(
        source, "[{IMPORTS}]",
        "\t[{ROUTE}]Pkg \"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/[{ROUTE}]\"\n[{IMPORTS}]");
    if (p.api_route == "serve") {
        replaceAll(source, "/internal/[{ROUTE}]", "/server");
    }
    replaceAll(source, STR_REPLACE_OPTS, "");
    replaceAll(source, "run[{PROPER}]", "[{ROUTE}]Pkg.Run");
    replaceAll(source, "validate[{PROPER}]Args", "[{ROUTE}]Pkg.Validate");
    replaceAll(source, "[{COPY_OPTS}]", get_copyopts(p));
    replaceAll(source, "[{SET_OPTS}]", get_setopts(p));
    replaceAll(source, "[{LOG_OPTS}]", get_logopts(p));
    replaceAll(source, "[{HIDDEN}]", get_hidden(p));
    replaceAll(source, "[{PERPRERUN}]", get_hidden2(p));
    replaceAll(source, "[{USE}]", get_use(p));
    replaceAll(source, "[{ROUTE}]", toLower(p.api_route));
    replaceAll(source, "[{PROPER}]", toProper(p.api_route));
    replaceAll(source, "[{POSTNOTES}]", get_notes2(p));
    string_q descr = firstLower(p.description);
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

    string_q fn = getSourcePath("apps/chifra/cmd/" + p.api_route + ".go");
    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(cw);
    counter.nVisited++;
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_options(const CCommandOption& p) {
    string_q source = asciiFileToString(getTemplatePath("blank_options.go"));
    replaceAll(source, "[{ROUTE}]", p.api_route);
    replaceAll(source, "[{PROPER}]", toProper(p.api_route));
    replaceAll(source, "[{OPT_FIELDS}]", get_optfields(p));
    replaceAll(source, "[{TEST_LOGS}]", get_testlogs(p));

    string_q fn = getSourcePath("apps/chifra/internal/" + p.api_route + "/options.go");
    replaceAll(fn, "/internal/serve", "/server");
    establishFolder(fn);
    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(cw);
    counter.nVisited++;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds(void) {
    LOG_INFO(cYellow, "handling go commands...", string_q(50, ' '), cOff);
    counter = CCounter();  // reset

    for (auto p : endpointArray) {
        if (!p.is_visible)
            continue;
        CCommandOptionArray params;
        CCommandOptionArray notes;
        for (auto option : routeOptionArray) {
            bool isOne = option.api_route == p.api_route && option.isChifraRoute(true);
            if (isOne) {
                params.push_back(option);
            }
            if (option.api_route == p.api_route && option.option_type == "note")
                notes.push_back(option);
        }
        p.params = &params;
        p.notes = &notes;

        handle_gocmds_cmd(p);
        handle_gocmds_options(p);
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

string_q noUnderbars(const string_q& in) {
    return substitute(toProper(in), "_", "");
}

string_q get_testlogs(const CCommandOption& cmd) {
    const char* STR_TESTLOG_LINE = "\tlogger.Log(logger.Test, \"[{LONGNAME}]: \", opts.[{LONGNAME}])";
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        p.longName = noUnderbars(p.longName);
        if (p.option_type != "positional" && !p.isDeprecated) {
            if (p.data_type == "<boolean>") {
                os << "\tif opts." << p.Format("[{LONGNAME}]") << " {" << endl;
                os << "\t" << p.Format(STR_TESTLOG_LINE) << endl;
                os << "\t}" << endl;
            } else if (startsWith(p.data_type, "list<")) {
                os << "\tif len(opts." << p.Format("[{LONGNAME}]") << ") > 0 {" << endl;
                os << "\t" << p.Format(STR_TESTLOG_LINE) << endl;
                os << "\t}" << endl;
            } else {
                cerr << padRight(p.data_type, 30) << p.def_val << endl;
                os << p.Format(STR_TESTLOG_LINE) << endl;
            }
        }
    }
    return os.str();
}

string_q get_optfields(const CCommandOption& cmd) {
    size_t wid = 0;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        p.longName = noUnderbars(p.longName);
        if (p.option_type != "positional") {
            wid = max(p.longName.length(), wid);
        }
    }
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        p.longName = noUnderbars(p.longName);
        if (p.option_type != "positional") {
            os << "\t" << padRight(p.longName, wid) << " " << p.go_type << endl;
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
    // TODO: search for go-port
    if (cmd.api_route != "explore")
        return "";
    const char* STR_RET =
        "	utils.TestLogArgs(\"terms\", args)\n"
        "	utils.TestLogBool(\"local\", ExploreOpts.Local)\n"
        "	utils.TestLogBool(\"google\", ExploreOpts.Google)\n";
    return STR_RET;
}

string_q get_setopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional") {
            replace(p.longName, "deleteMe", "delete");

            os << "\t[{ROUTE}]Cmd.Flags().";
            os << p.go_flagtype;
            os << "(&[{ROUTE}]Pkg.Options.";
            os << noUnderbars(p.longName) << ", ";
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

const char* STR_REPLACE_OPTS =
    "type [{ROUTE}]OptionsType struct {\n"
    "[{OPT_FIELDS}]}\n"
    "\n";
