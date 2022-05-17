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
extern string_q get_notes2(const CCommandOption& cmd);
extern string_q get_optfields(const CCommandOption& cmd);
extern string_q get_requestopts(const CCommandOption& cmd);
extern string_q get_defaults_apis(const CCommandOption& cmd);
extern string_q get_setopts(const CCommandOption& cmd);
extern string_q get_testlogs(const CCommandOption& cmd);
extern string_q get_copyopts(const CCommandOption& cmd);
extern string_q get_positionals2(const CCommandOption& cmd);
extern string_q get_use(const CCommandOption& cmd);
extern string_q clean_go_positionals(const string_q& in, bool hasEns);

extern const char* STR_REQUEST_CASE1;
extern const char* STR_REQUEST_CASE2;
extern const char* STR_CHIFRA_HELP_END;
extern const char* STR_TO_CMD_LINE;
//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_cmd(const CCommandOption& p) {
    string_q source = asciiFileToString(getPathToTemplates("blank.go.tmpl"));
    replaceAll(source, "[{LONG}]", "Purpose:\n  " + p.description);
    replaceAll(source, "[{OPT_DEF}]", "");
    replaceAll(source, "validate[{PROPER}]Args", "[{ROUTE}]Pkg.Validate");
    replaceAll(source, "/internal/[{ROUTE}]", (p.api_route == "serve" ? "/server" : "/internal/[{ROUTE}]"));
    replaceAll(source, "[{SET_OPTS}]", get_setopts(p));
    replaceAll(source, "[{HIDDEN}]", get_hidden(p));
    replaceAll(source, "[{USE}]", get_use(p));
    replaceAll(source, "[{ROUTE}]", toLower(p.api_route));
    replaceAll(source, "[{PROPER}]", toProper(p.api_route));
    replaceAll(source, "[{POSTNOTES}]", get_notes2(p));
    string_q descr = firstLower(p.description);
    if (endsWith(descr, "."))
        replaceReverse(descr, ".", "");
    replaceAll(source, "[{SHORT}]", descr);

    string_q fn = getPathToSource("apps/chifra/cmd/" + p.api_route + ".go");
    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(cw);
    counter.nVisited++;
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::verifyGoEnumValidators(void) {
    for (auto p : cmdOptionArray) {
        if (contains(p.data_type, "enum") && !p.api_route.empty()) {
            string_q e = p.data_type;
            replace(e, "enum", "");
            replace(e, "list", "");
            replaceAny(e, "<[*]>", "");
            e = "[" + e + "]";
            string_q fn = getPathToSource("apps/chifra/internal/" + p.api_route + "/validate.go");
            string_q contents = asciiFileToString(fn);
            if (contains(contents, e)) {
                // cout << cGreen << "HAS: " << fn << ": " << e << cOff << endl;
            } else {
                LOG_WARN("\t", p.api_route, " has no an enum validator for ", e);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_options(const CCommandOption& p) {
    string_q fn = getPathToSource("apps/chifra/internal/" + p.api_route + "/options.go");
    replaceAll(fn, "/internal/serve", "/server");
    establishFolder(fn);
    bool hasEns = contains(asciiFileToString(fn), "ens.Convert");

    string_q source = asciiFileToString(getPathToTemplates("blank_options.go.tmpl"));
    if (isFullyPorted(p.api_route)) {
        replaceAll(source, "[{TOCMDLINE}]", "");
    } else {
        replaceAll(source, "[{TOCMDLINE}]", STR_TO_CMD_LINE);
    }
    replaceAll(source, "[{ROUTE}]", p.api_route);
    replaceAll(source, "[{PROPER}]", toProper(p.api_route));
    replaceAll(source, "[{OPT_FIELDS}]", get_optfields(p));
    replaceAll(source, "[{DEFAULTS_API}]", get_defaults_apis(p));
    string_q req = get_requestopts(p);
    replaceAll(source, "[{REQUEST_OPTS}]", req);
    if (!contains(substitute(req, "for key, value := range r.URL.Query() {", ""), "value")) {
        replaceAll(source, "for key, value := range r.URL.Query() {", "for key, _ := range r.URL.Query() {");
    }
    replaceAll(source, "[{TEST_LOGS}]", get_testlogs(p));
    replaceAll(source, "[{DASH_STR}]", get_copyopts(p));
    replaceAll(source, "[{POSITIONALS}]", get_positionals2(p));
    replaceAll(source, "opts.LastBlock != utils.NOPOS", "opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS");
    source = clean_go_positionals(source, hasEns);
    if (isFullyPorted(p.api_route)) {
        if (!contains(source, "fmt.")) {
            replaceAll(source, "\t\"fmt\"\n", "");
        }
        if (!contains(source, "strings.")) {
            replaceAll(source, "\t\"strings\"\n", "");
        }
    }

    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(cw);
    counter.nVisited++;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_output(const CCommandOption& p) {
    string_q source = asciiFileToString(getPathToTemplates("blank_output.go.tmpl"));
    replaceAll(source, "[{ROUTE}]", p.api_route);
    source = substitute(source, "[]string", "++SAVED++");
    source = p.Format(source);
    replaceAll(source, "++SAVED++", "[]string");
    if (contains(source, "\t// return nil\n\t// EXISTING_CODE\n")) {
        replaceAll(source,
                   "\n"
                   "\treturn opts.Globals.PassItOn(\"[{TOOL}]\", opts.ToCmdLine())\n",
                   "");
    }

    string_q fn = getPathToSource("apps/chifra/internal/" + p.api_route + "/output.go");
    if (contains(fn, "/serve"))
        return true;

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
        if (!p.is_visible) {
            if (!p.group.empty())
                chifraHelpStream << p.toChifraHelp();
            continue;
        }
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
        handle_gocmds_output(p);
        chifraHelpStream << p.toChifraHelp();
    }
    chifraHelpStream << STR_CHIFRA_HELP_END;

    string_q contents = asciiFileToString(getPathToTemplates("help_text.go.tmpl"));
    replace(contents, "[{HELP_TEXT}]", chifraHelpStream.str());
    replace(contents, "[{VERSION}]", getVersionStr(true, false));
    stringToAsciiFile(getPathToSource("apps/chifra/cmd/help_text.go"), contents);

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
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        p.def_val = substitute(p.def_val, "NOPOS", "utils.NOPOS");

        if (!p.isDeprecated) {
            if (p.data_type == "<boolean>") {
                const char* STR_TESTLOG_BOOL =
                    "\tlogger.TestLog(opts.[{VARIABLE}], \"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                os << p.Format(STR_TESTLOG_BOOL) << endl;

            } else if (startsWith(p.data_type, "list<") || p.data_type == "<string>" || p.data_type == "<address>" ||
                       contains(p.data_type, "enum")) {
                const char* STR_TESTLOG_STRING =
                    "\tlogger.TestLog(len(opts.[{VARIABLE}]) > 0, \"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                os << p.Format(STR_TESTLOG_STRING) << endl;

            } else if (p.data_type == "<blknum>" || p.data_type == "<uint64>" || p.data_type == "<double>") {
                const char* STR_TESTLOG_UINT =
                    "\tlogger.TestLog(opts.[{VARIABLE}] != [{DEF_VAL}], \"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                os << p.Format(STR_TESTLOG_UINT) << endl;

            } else {
                cerr << "Unknown type: " << padRight(p.data_type, 30) << p.def_val << endl;
                exit(0);
            }
        }
    }
    return os.str();
}

string_q get_optfields(const CCommandOption& cmd) {
    size_t wid = 0;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        string_q v = p.Format("[{VARIABLE}]");
        wid = max(v.length(), wid);
        if (contains(v, "Blocks") && contains(p.go_type, "[]string")) {
            wid = max(string_q("BlockNums").length(), wid);
        }
    }
    wid = max(string_q("Globals").length(), wid);
    wid = max(string_q("BadFlag").length(), wid);

    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        string_q v = p.Format("[{VARIABLE}]");
        os << "\t" << padRight(v, wid) << " " << p.go_type << endl;
        if (contains(v, "Blocks") && contains(p.go_type, "[]string")) {
            os << "\t" << padRight("BlockNums", wid) << " []blockRange.BlockRange" << endl;
        }
    }
    os << "\t" << padRight("Globals", wid) << " globals.GlobalOptions" << endl;
    os << "\t" << padRight("BadFlag", wid) << " error" << endl;

    return os.str();
}

string_q get_defaults_apis(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        p.def_val = substitute(p.def_val, "NOPOS", "utils.NOPOS");
        if (!p.isDeprecated && (p.data_type == "<blknum>" || p.data_type == "<uint64>" || p.data_type == "<double>")) {
            os << p.Format("\topts.[{VARIABLE}] = [{DEF_VAL}]") << endl;
        }
    }
    return os.str();
}

string_q get_requestopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        replace(p.longName, "deleteMe", "delete");
        string_q low = toCamelCase(p.Format("[{LOWER}]"));
        if (startsWith(p.data_type, "list<")) {
            os << p.Format(substitute(STR_REQUEST_CASE2, "++LOWER++", low)) << endl;
        } else {
            os << p.Format(substitute(STR_REQUEST_CASE1, "++LOWER++", low)) << endl;
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

//---------------------------------------------------------------------------------------------------
string_q clean_go_positionals(const string_q& in, bool hasEns) {
    string_q ret = in;
    replaceAll(ret, "\t[]string{} = args\n", "");
    replaceAll(ret, "opts.[]string{}", "[]string{}");
    if (!contains(ret, "utils."))
        replaceAll(ret, "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils\"\n", "");
    if (!contains(ret, "blockRange."))
        replaceAll(ret, "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange\"\n", "");
    if (!hasEns)
        replaceAll(ret, "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens\"\n", "");
    return ret;
}

const char* STR_POSITIONALS1 = "\toptions += \" \" + strings.Join(opts.[{VARIABLE}], \" \")";
//---------------------------------------------------------------------------------------------------
string_q get_positionals2(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params))
        if (p.option_type == "positional")
            os << p.Format(STR_POSITIONALS1) << endl;
    if (os.str().empty())
        os << substitute(STR_POSITIONALS1, "[{VARIABLE}]", "[]string{}") << endl;
    return os.str();
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
    ret << "\tif os.Getenv(\"TEST_MODE\") != \"true\" {" << endl;
    ret << os.str();
    ret << "\t}" << endl;
    return ret.str();
}

string_q get_setopts(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.params)) {
        if (p.option_type != "positional") {
            replace(p.longName, "deleteMe", "delete");

            os << "\t[{ROUTE}]Cmd.Flags().";
            os << p.go_flagtype;
            os << "(&[{ROUTE}]Pkg.GetOptions().";
            os << p.Format("[{VARIABLE}]") << ", ";
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
        if (p.isDeprecated || p.codeLoc() == GOCMD)
            continue;

        replace(p.longName, "deleteMe", "delete");
        if (p.option_type != "positional") {
            string_q format;
            if (p.go_type == "[]string") {
                format =
                    "\tfor _, [{SINGULAR}] := range opts.[{VARIABLE}] {\n"
                    "\t\toptions += \" --[{LONGNAME}] \" + [{SINGULAR}]\n"
                    "\t}";
            } else if (p.go_type == "string") {
                format =
                    "\tif len(opts.[{VARIABLE}]) > 0 {\n"
                    "\t\toptions += \" --[{LONGNAME}] \" + opts.[{VARIABLE}]\n"
                    "\t}";
            } else if (p.go_type == "uint64" || p.go_type == "uint32") {
                format =
                    "\tif opts.[{VARIABLE}] != [{DEF_VAL}] {\n"
                    "\t\toptions += (\" --[{LONGNAME}] \" + fmt.Sprintf(\"%d\", opts.[{VARIABLE}]))\n"
                    "\t}";
            } else if (p.go_type == "float64") {
                format =
                    "\tif opts.[{VARIABLE}] != [{DEF_VAL}] {\n"
                    "\t\toptions += (\" --[{LONGNAME}] \" + fmt.Sprintf(\"%.1f\", opts.[{VARIABLE}]))\n"
                    "\t}";
            } else {
                format =
                    "\tif opts.[{VARIABLE}] {\n"
                    "\t\toptions += \" --[{LONGNAME}]\"\n"
                    "\t}";
            }
            os << substitute(p.Format(format), "NOPOS", "utils.NOPOS") << endl;
        }
    }
    return os.str();
}

const char* STR_REQUEST_CASE1 =
    "\t\tcase \"++LOWER++\":\n"
    "\t\t\topts.[{VARIABLE}] = [{ASSIGN}]";

const char* STR_REQUEST_CASE2 =
    "\t\tcase \"++LOWER++\":\n"
    "\t\t\tfor _, val := range value {\n"
    "\t\t\t\ts := strings.Split(val, \" \") // may contain space separated items\n"
    "\t\t\t\topts.[{VARIABLE}] = append(opts.[{VARIABLE}], s...)\n"
    "\t\t\t}";

const char* STR_CHIFRA_HELP_END =
    "  Flags:\n"
    "    -h, --help    display this help screen\n"
    "\n"
    "  Use \"chifra [command] --help\" for more information about a command.\n";

const char* STR_TO_CMD_LINE =
    "func (opts *[{PROPER}]Options) ToCmdLine() string {\n"
    "\toptions := \"\"\n"
    "[{DASH_STR}][{POSITIONALS}]\toptions += fmt.Sprintf(\"%s\", \"\") // silence go compiler for auto gen\n"
    "\treturn options\n"
    "}\n\n";
