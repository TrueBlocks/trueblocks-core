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
extern string_q get_aliases(const CCommandOption& cmd);
extern string_q get_optfields(const CCommandOption& cmd);
extern string_q get_requestopts(const CCommandOption& cmd);
extern string_q get_defaults_apis(const CCommandOption& cmd);
extern string_q get_ens_convert1(const CCommandOption& cmd);
extern string_q get_ens_convert2(const CCommandOption& cmd);
extern string_q get_capabilities(const CCommandOption& cmd);
extern string_q get_config_package(const CCommandOption& cmd);
extern string_q get_walk_package(const CCommandOption& cmd);
extern string_q get_os_package(const string_q& fn);
extern string_q get_base_package(const string_q& fn);
extern string_q get_index_package(const string_q& fn);
extern string_q get_setopts(const CCommandOption& cmd);
extern string_q get_testlogs(const CCommandOption& cmd);
extern string_q get_godefaults(const CCommandOption& cmd);
extern string_q get_use(const CCommandOption& cmd);
extern string_q clean_go_positionals(const string_q& in, bool hasRpc);
extern string_q clean_positionals(const string& progName, const string_q& strIn);

extern const char* STR_REQUEST_CASE1;
extern const char* STR_REQUEST_CASE2;
extern const char* STR_CHIFRA_HELP_END;

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_cmd(const CCommandOption& ep) {
    string_q source = asciiFileToString(getPathToTemplates("blank.go.tmpl"));

    replaceAll(source, "[{CAPABILITIES}]", get_capabilities(ep));
    replaceAll(source, "[{LONG}]", "Purpose:\n  " + ep.description);
    replaceAll(source, "[{OPT_DEF}]", "");
    replaceAll(source, "validate[{PROPER}]Args", "[{ROUTE}]Pkg.Validate");
    replaceAll(source, "[{SET_OPTS}]", get_setopts(ep));
    replaceAll(source, "[{HIDDEN}]", get_hidden(ep));
    replaceAll(source, "[{USE}]", get_use(ep));
    replaceAll(source, "[{ROUTE}]", toLower(ep.api_route));
    replaceAll(source, "[{LOWER}]", toLower(ep.api_route));
    replaceAll(source, "[{PROPER}]", toProper(ep.api_route));
    replaceAll(source, "[{POSTNOTES}]", get_notes2(ep));
    replaceAll(source, "[{ALIASES}]", get_aliases(ep));
    string_q descr = firstLower(ep.description);
    if (endsWith(descr, "."))
        replaceReverse(descr, ".", "");
    replaceAll(source, "[{SHORT}]", descr);

    string_q fn = getPathToSource("apps/chifra/cmd/" + ep.api_route + ".go");
    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(this, cw);
    counter.nVisited++;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool hasValidator(const string_q& route, const string_q& str) {
    string_q contents = asciiFileToString(getPathToSource("apps/chifra/internal/" + route + "/validate.go"));
    CStringArray parts;
    explode(parts, str, '|');
    for (auto part : parts) {
        if (!contains(contents, part))
            return false;
    }
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
            if (!hasValidator(p.api_route, e)) {
                LOG_WARN("\t", bRed, p.api_route, " has no enum validator for ", e, cOff);
                exit(0);
            }
        }
        if (p.generate == "deprecated") {
            if (!hasValidator(p.api_route, "deprecated|--" + p.longName)) {
                ostringstream os;
                os << "The '" << p.api_route << " --" << p.longName
                   << "' option is deprecated, but no usage message in validate.go";
                LOG_WARN("\t", bRed, os.str(), cOff);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_docfile(const CCommandOption& ep) {
    string_q source = asciiFileToString(getPathToTemplates("blank_doc.go.tmpl"));
    string_q fn = getPathToSource("apps/chifra/internal/" + ep.api_route + "/doc.go");
    establishFolder(fn);

    string_q docFn = substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".md";
    string_q docSource = getDocsPathTemplates("readme-intros/" + docFn);
    const char* STR_WHAT = "// Package [{API_ROUTE}]Pkg handles the chifra [{API_ROUTE}] command. It ";
    string_q contents = STR_WHAT + asciiFileToString(docSource);
    contents = substitute(substitute(substitute(contents, "\n", " "), "\t", " "), "\r", " ");
    replaceAll(contents, "  ", " ");
    replaceAll(contents, "`", "");
    replaceAll(contents, "[{NAME}]", "[chifra {ROUTE}]");
    replaceAll(source, "TEXT_TEMPLATE", contents);
    replaceAll(source, "[{ROUTE}]", ep.api_route);
    replaceAll(source, "[{LOWER}]", toLower(ep.api_route));
    source = ep.Format(source);

    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(this, cw);
    counter.nVisited++;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_options(const CCommandOption& ep) {
    string_q fn = getPathToSource("apps/chifra/internal/" + ep.api_route + "/options.go");
    establishFolder(fn);
    bool hasRpc = contains(asciiFileToString(fn), "rpc.");

    string_q source = asciiFileToString(getPathToTemplates("blank_options.go.tmpl"));

    replaceAll(source, "[{CAPABILITIES}]", get_capabilities(ep));

    replaceAll(source, "[{ROUTE}]", ep.api_route);
    replaceAll(source, "[{LOWER}]", toLower(ep.api_route));
    replaceAll(source, "[{PROPER}]", toProper(ep.api_route));
    replaceAll(source, "[{OPT_FIELDS}]", get_optfields(ep));
    replaceAll(source, "[{DEFAULTS_API}]", get_defaults_apis(ep));
    replaceAll(source, "[{ENS_CONVERT1}]", get_ens_convert1(ep));
    replaceAll(source, "[{ENS_CONVERT2}]", get_ens_convert2(ep));
    replaceAll(source, "[{CONFIGPKG}]", get_config_package(ep));
    replaceAll(source, "[{WALKPKG}]", get_walk_package(ep));
    replaceAll(source, "[{BASEPKG}]", get_base_package(fn));
    replaceAll(source, "[{OS}]", get_os_package(fn));
    replaceAll(source, "[{INDEXPKG}]", get_index_package(fn));

    string_q req = get_requestopts(ep);
    replaceAll(source, "[{REQUEST_OPTS}]", req);
    if (!contains(substitute(req, "for key, value := range r.URL.Query() {", ""), "value")) {
        replaceAll(source, "for key, value := range r.URL.Query() {", "for key, _ := range r.URL.Query() {");
    }
    replaceAll(source, "[{TEST_LOGS}]", get_testlogs(ep));
    replaceAll(source, "[{GODEFS}]", get_godefaults(ep));
    replaceAll(source, "opts.LastBlock != utils.NOPOS", "opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS");
    source = clean_go_positionals(source, hasRpc);
    if (!contains(source, "fmt.")) {
        replaceAll(source, "\t\"fmt\"\n", "");
    }
    if (!contains(source, "strings.")) {
        replaceAll(source, "\t\"strings\"\n", "");
    }

    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(this, cw);
    counter.nVisited++;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds_output(const CCommandOption& ep) {
    string_q source = asciiFileToString(getPathToTemplates("blank_output.go.tmpl"));
    replaceAll(source, "[{ROUTE}]", ep.api_route);
    replaceAll(source, "[{LOWER}]", toLower(ep.api_route));
    source = substitute(source, "[]string", "++SAVED++");
    source = ep.Format(source);
    replaceAll(source, "++SAVED++", "[]string");

    string_q fn = getPathToSource("apps/chifra/internal/" + ep.api_route + "/output.go");

    establishFolder(fn);
    codewrite_t cw(fn, source);
    cw.nSpaces = 0;
    cw.stripEOFNL = false;
    counter.nProcessed += writeCodeIn(this, cw);
    counter.nVisited++;

    return true;
}

//---------------------------------------------------------------------------------------------------
string_q toChifraHelp(const CCommandOption& cmd) {
    if ((cmd.description.empty() && !cmd.api_route.empty()) || cmd.api_route == "blaze")
        return "";

    CCommandOption ret = cmd;
    replaceAll(ret.description, ".", "");
    ret.description = firstLower(ret.description);
    if (cmd.api_route.empty())
        return toProper(ret.Format("  [{GROUP}]:")) + "\n";
    return ret.Format("    [{w:14:API_ROUTE}][{DESCRIPTION}]") + "\n";
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_gocmds(void) {
    LOG_INFO(cYellow, "handling go commands...", string_q(50, ' '), cOff);
    counter = CCounter();  // reset

    for (auto ep : endpointArray) {
        if (!ep.is_visible) {
            if (!ep.group.empty())
                chifraHelpStream << toChifraHelp(ep);
            continue;
        }
        CCommandOptionArray members;
        CCommandOptionArray notes;
        for (auto option : routeOptionArray) {
            bool isOne = option.api_route == ep.api_route && option.isChifraRoute(true);
            if (isOne) {
                members.push_back(option);
            }
            if (option.api_route == ep.api_route) {
                if (option.isNote) {
                    notes.push_back(option);
                } else if (option.isAlias) {
                    ep.aliases.push_back(option.description);
                }
            }
        }
        ep.members = &members;
        ep.notes = &notes;

        handle_gocmds_cmd(ep);
        handle_gocmds_options(ep);
        handle_gocmds_output(ep);
        handle_gocmds_docfile(ep);
        chifraHelpStream << toChifraHelp(ep);
    }
    chifraHelpStream << STR_CHIFRA_HELP_END;

    string_q contents = asciiFileToString(getPathToTemplates("help_text.go.tmpl"));
    replace(contents, "[{HELP_TEXT}]", chifraHelpStream.str());
    stringToAsciiFile(getPathToSource("apps/chifra/cmd/help_text.go"), contents);

    contents = asciiFileToString(getPathToTemplates("version.go.tmpl"));
    replace(contents, "[{VERSION}]", getVersionStr(true, false));
    stringToAsciiFile(getPathToSource("apps/chifra/pkg/version/version_strings.go"), contents);

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
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.option_type == "positional") {
            if (arguments.str().empty())
                arguments << endl << "Arguments:" << endl;
            else
                arguments << endl;
            arguments << member.Format("  [{LONGNAME}] - [{DESCRIPTION}]");
            if (member.is_required)
                arguments << " (required)";
            if (contains(member.data_type, "enum")) {
                ostringstream os;
                forEveryEnum(visitEnumItem2, member.data_type, &os);
                os << " ]";
                string_q str = substitute(os.str(), "One of",
                                          contains(member.data_type, "list") ? "\tOne or more of" : "\tOne of");
                // replace(str, "", "");
                arguments << str;
            }
        }
    }

    ostringstream positionals;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.option_type == "positional") {
            if (!positionals.str().empty())
                positionals << " ";
            positionals << member.data_type;
        }
    }

    string_q ret = "[{ROUTE}] [flags][{TYPES}][{POSITIONALS}]";
    if (contains(toLower(cmd.tool), "scrape")) {
        ret = "[{ROUTE}] [flags]";
    }
    replace(ret, "[{TYPES}]", clean_positionals(cmd.api_route, positionals.str()));
    replace(ret, "[{POSITIONALS}]", arguments.str());
    replace(ret, "enum[show*|edit]", " <mode>\n");
    replace(ret, "[flags] <mode> [blocks...]", "<mode> [flags] [blocks...]");
    replace(ret, "[flags] <mode> [mode...]", "<mode> [mode...] [flags]");
    replace(ret, "[flags] <mode>", "<mode> [flags]");
    return ret;
}

string_q get_notes2(const CCommandOption& cmd) {
    ostringstream os;
    for (auto note : *((CCommandOptionArray*)cmd.notes)) {
        if (os.str().empty()) {
            os << endl;
            os << "Notes:" << endl;
        } else {
            os << endl;
        }
        os << "  - " << substitute(note.description, "`", "");
        // TODO: Coloring in notes (search in makeClass for this note)
        // os << "  - " << substitute(note.description, "`", "++");
    }

    return trim(substitute(os.str(), "|", "\n    "));
}

string_q get_aliases(const CCommandOption& cmd) {
    if (cmd.aliases.size() == 0) {
        return "";
    }

    ostringstream os;
    os << "\tAliases: []string{" << endl;
    for (auto a : cmd.aliases) {
        os << "\t\t\"" << a << "\"," << endl;
    }
    os << "\t}," << endl;
    return os.str();
}

string_q noUnderbars(const string_q& in) {
    return substitute(toProper(in), "_", "");
}

bool isDef(const CCommandOption& p) {
    if (p.generate == "config")
        return true;
    if (p.def_val.empty())
        return true;
    if (p.def_val == "\"\"")
        return true;
    if (p.def_val == "0")
        return true;
    if (p.def_val == "0.0")
        return true;
    if (p.def_val == "false")
        return true;
    if (contains(p.go_intype, "[]"))
        return true;
    if (p.longName == "sleep")
        return true;
    return false;
}

string_q get_godefaults(const CCommandOption& cmd) {
    size_t wid = 0;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (!isDef(member)) {
            wid = max(wid, member.Format("[{VARIABLE}]").length());
        }
    }

    ostringstream os;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (!isDef(member)) {
            string_q val = substitute(member.def_val, "NOPOS", "utils.NOPOS");
            os << "\t" << padRight(member.Format("[{VARIABLE}]") + ": ", wid + 2, ' ') << val << "," << endl;
        }
    }

    return os.str().length() == 0 ? "" : "\n" + os.str();
}

string_q get_testlogs(const CCommandOption& cmd) {
    bool hasConfig = false;
    ostringstream os;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        member.def_val = substitute(member.def_val, "NOPOS", "utils.NOPOS");
        if (member.generate == "config") {
            hasConfig = true;
            continue;
        }
        if (!member.isDeprecated) {
            if (member.data_type == "<boolean>") {
                const char* STR_TESTLOG_BOOL =
                    "\tlogger.TestLog(opts.[{VARIABLE}], \"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                os << member.Format(STR_TESTLOG_BOOL) << endl;

            } else if (startsWith(member.data_type, "list<") || member.data_type == "<string>" ||
                       member.data_type == "<address>" || contains(member.data_type, "enum")) {
                if (!member.def_val.empty() && member.def_val != "\"\"" && member.def_val != "utils.NOPOS") {
                    string_q STR_TESTLOG_STRING =
                        "\tlogger.TestLog(len(opts.[{VARIABLE}]) > 0 && opts.[{VARIABLE}] != \"[{DEF_VAL}]\", "
                        "\"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                    if (contains(member.def_val, ".eth")) {
                        STR_TESTLOG_STRING =
                            "\tlogger.TestLog(!rpc.IsSame(opts.[{VARIABLE}], \"[{DEF_VAL}]\"), "
                            "\"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                    }
                    member.def_val = substitute(member.def_val, "\"", "");
                    os << member.Format(STR_TESTLOG_STRING) << endl;
                } else {
                    const char* STR_TESTLOG_STRING =
                        "\tlogger.TestLog(len(opts.[{VARIABLE}]) > 0, \"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                    os << member.Format(STR_TESTLOG_STRING) << endl;
                }

            } else if (member.data_type == "<blknum>" || member.data_type == "<uint64>") {
                const char* STR_TESTLOG_UINT =
                    "\tlogger.TestLog(opts.[{VARIABLE}] != [{DEF_VAL}], \"[{VARIABLE}]: \", opts.[{VARIABLE}])";
                os << member.Format(STR_TESTLOG_UINT) << endl;

            } else if (member.data_type == "<double>") {
                const char* STR_TESTLOG_DOUBLE =
                    "\tlogger.TestLog(opts.[{VARIABLE}] != float64([{DEF_VAL}]), \"[{VARIABLE}]: \", "
                    "opts.[{VARIABLE}])";
                os << member.Format(STR_TESTLOG_DOUBLE) << endl;

            } else {
                cerr << "Unknown type: " << padRight(member.data_type, 30) << member.def_val << endl;
                exit(0);
            }
        }
    }
    if (hasConfig) {
        os << cmd.Format("\topts.Settings.TestLog(opts.Globals.Chain, opts.Globals.TestMode)\n");
    }
    return os.str();
}

string_q get_optfields(const CCommandOption& cmd) {
    string_q n = "readme-intros/" + substitute(toLower(cmd.group), " ", "") + "-" + cmd.api_route + ".config.md";
    string_q configDocs = getDocsPathTemplates(n);
    ::remove(configDocs.c_str());  // remove it if it exists, we will replace it

    size_t varWidth = 0, typeWidth = 0;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        string_q var = member.Format("[{VARIABLE}]");
        if (member.generate == "config") {
            var = toCamelCase(var);
            varWidth = max(var.length(), varWidth);
            string_q type = cmd.Format("config.[{PROPER}]Settings");
            typeWidth = max(type.length(), typeWidth);
            continue;
        }
        varWidth = max(var.length(), varWidth);
        string_q type = member.Format("[{GO_INTYPE}]");
        typeWidth = max(type.length(), typeWidth);
        if (contains(var, "Blocks") && contains(member.go_intype, "[]string")) {
            varWidth = max(string_q("BlockIds").length(), varWidth);
            typeWidth = max(string_q("[]identifiers.Identifier").length(), typeWidth);
        }
        if (contains(var, "Transactions") && contains(member.go_intype, "[]string")) {
            varWidth = max(string_q("TransactionIds").length(), varWidth);
            typeWidth = max(string_q("[]identifiers.Identifier").length(), typeWidth);
        }
    }
    varWidth = max(string_q("Globals").length(), varWidth);
    varWidth = max(string_q("Conn").length(), varWidth);
    varWidth = max(string_q("BadFlag").length(), varWidth);
    typeWidth = max(string_q("globals.GlobalOptions").length(), typeWidth);
    typeWidth = max(string_q("error").length(), typeWidth);
    varWidth++;
    typeWidth++;

#define ONE(os, v1, w1, v2, w2, c)                                                                                     \
    os << "\t";                                                                                                        \
    os << padRight(v1, (w1));                                                                                          \
    os << padRight(v2, (w2));                                                                                          \
    os << padRight("`json:\"" + firstLower(v1) + ",omitempty\"`", (w1) + 19);                                          \
    os << "// " << firstUpper(c) << endl;

    ostringstream os;
    bool hasConfig = 0;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.generate == "config") {
            ostringstream dd;
            if (!hasConfig) {
                dd << "| " << padRight("Item", 18) << " | " << padRight("Type", 12) << " | " << padRight("Default", 12)
                   << " | Description / Default |" << endl;
                dd << "| " << string_q(18, '-') << " | " << string_q(12, '-') << " | " << string_q(12, '-')
                   << " | --------- |" << endl;
            }
            string_q x = toCamelCase(member.Format("[{LONGNAME}]"));
            dd << "| " << padRight(x, 18) << " | " << padRight(member.Format("[{GO_INTYPE}]"), 12) << " | "
               << padRight(member.Format("[{DEF_VAL}]"), 12) << " | " << member.Format("[{DESCRIPTION}]") << " |"
               << endl;
            appendToAsciiFile(configDocs, dd.str());
            hasConfig = true;
            continue;
        }
        string_q var = member.Format("[{VARIABLE}]");
        string_q type = member.Format("[{GO_INTYPE}]");
        ONE(os, var, varWidth, type, typeWidth, member.description);
        if (contains(var, "Blocks") && contains(member.go_intype, "[]string")) {
            ONE(os, "BlockIds", varWidth, "[]identifiers.Identifier", typeWidth, "block identifiers");
        }
        if (contains(var, "Transactions") && contains(member.go_intype, "[]string")) {
            ONE(os, "TransactionIds", varWidth, "[]identifiers.Identifier", typeWidth, "transaction identifiers");
        }
    }

    if (hasConfig) {
        string type = cmd.Format("config.[{PROPER}]Settings");
        ONE(os, "Settings", varWidth, type, typeWidth, "Configuration items for the " + cmd.api_route);
    }

    ONE(os, "Globals", varWidth, "globals.GlobalOptions", typeWidth, "the global options");
    ONE(os, "Conn", varWidth, "*rpc.Connection", typeWidth, "the connection to the RPC server");
    ONE(os, "BadFlag", varWidth, "error", typeWidth, "an error flag if needed");

    return os.str();
}

string_q get_ens_convert1(const CCommandOption& cmd) {
    ostringstream os;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.isAddress) {
            string_q str = "\topts.[{VARIABLE}], _ = opts.Conn.GetEnsAddress(opts.[{VARIABLE}])";
            if (containsI(member.real_type, "address_t")) {
                str += "\n\topts.[{VARIABLE}]Addr = base.HexToAddress(opts.[{VARIABLE}])";
            }
            if (member.longName % "publisher") {
                replace(str, "opts.Conn.GetEnsAddress(opts.[{VARIABLE}])",
                        "opts.Conn.GetEnsAddress(config.GetPublisher(opts.[{VARIABLE}]))");
            }
            os << member.Format(str) << endl;
        }
    }
    return os.str();
}

string_q get_capabilities(const CCommandOption& ep) {
    ostringstream os;
    os << "var capabilities caps.Capability // capabilities for chifra [{ROUTE}]" << endl;

    CStringArray caps;
    explode(caps, ep.capabilities, '|');
    for (auto cap : caps) {
        os << "\tcapabilities = capabilities.Add(caps." << firstUpper(cap) << ")" << endl;
    }

    return os.str();
}

string_q get_ens_convert2(const CCommandOption& cmd) {
    ostringstream os;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.isAddressList) {
            string_q str = "\topts.[{VARIABLE}], _ = opts.Conn.GetEnsAddresses(opts.[{VARIABLE}])";
            if (containsI(member.real_type, "address_t")) {
                str += "\n\topts.[{VARIABLE}]Addr = base.HexToAddress(opts.[{VARIABLE}])";
            }
            if (member.longName % "publisher") {
                replace(str, "opts.Conn.GetEnsAddress(opts.[{VARIABLE}])",
                        "opts.Conn.GetEnsAddress(config.GetPublisher(opts.[{VARIABLE}]))");
            }
            os << member.Format(str) << endl;
        }
    }
    return os.str();
}

string_q get_config_package(const CCommandOption& cmd) {
    for (auto member : *((CCommandOptionArray*)cmd.members))
        if (member.longName % "publisher" || member.generate == "config")
            return "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config\"\n";
    return "";
}

string_q get_walk_package(const CCommandOption& cmd) {
    if (cmd.api_route == "status") {
        return "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk\"\n";
    }
    return "";
}

string_q get_base_package(const string_q& fn) {
    string_q existing = asciiFileToString(fn);
    if (contains(existing, "base.")) {
        return "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base\"\n";
    }
    return "";
}

string_q get_os_package(const string_q& fn) {
    string_q existing = asciiFileToString(fn);
    if (contains(existing, "os.")) {
        return "\t\"os\"\n";
    }
    return "";
}

string_q get_index_package(const string_q& fn) {
    // string_q existing = asciiFileToString(fn);
    // if (contains(existing, "index.")) {
    //     return "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index\"\n";
    // }
    return "";
}

string_q get_defaults_apis(const CCommandOption& cmd) {
    string_q last = "";
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.generate == "config") {
            last = member.longName;
        }
    }

    ostringstream os;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.isDeprecated) {
            continue;
        }
        if (member.def_val == "") {
            continue;
        }

        member.def_val = substitute(member.def_val, "NOPOS", "utils.NOPOS");
        if (member.data_type == "<blknum>" || member.data_type == "<uint64>" || member.data_type == "<double>") {
            string_q fmt = "\topts.[{VARIABLE}] = [{DEF_VAL}]";
            if (member.generate == "config") {
                if (member.go_intype == "float64") {
                    fmt = "\topts.Settings.[{VARIABLE}] = float64([{DEF_VAL}])";
                } else {
                    fmt = "\topts.Settings.[{VARIABLE}] = [{DEF_VAL}]";
                }
            }
            os << member.Format(fmt) << endl;
        }
        if (member.longName == last) {
            os << "\tconfigs := make(map[string]string, 10)" << endl;
        }
    }
    return os.str();
}

string_q get_requestopts(const CCommandOption& cmd) {
    string_q last = "";
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        if (member.generate == "config") {
            last = member.longName;
        }
    }

    ostringstream os;
    for (auto member : *((CCommandOptionArray*)cmd.members)) {
        string_q low = toCamelCase(member.Format("[{LOWER}]"));
        string_q fmt;
        if (startsWith(member.data_type, "list<")) {
            fmt = substitute(STR_REQUEST_CASE2, "++LOWER++", low);
        } else {
            fmt = substitute(STR_REQUEST_CASE1, "++LOWER++", low);
        }
        if (member.generate == "config") {
            if (member.longName == last) {
                os << "\t\tcase \"" + low + "\":" << endl;
                os << "\t\t\tconfigs[key] = value[0]" << endl;
            } else {
                fmt = substitute(fmt, "opts.[{VARIABLE}] = [{ASSIGN}]", "fallthrough");
                os << member.Format(fmt) << endl;
            }
        } else {
            os << member.Format(fmt) << endl;
        }
    }
    return os.str();
}

string_q get_goDefault(const CCommandOption& p) {
    if (p.go_intype == "[]string") {
        return "nil";
    } else if (p.go_intype == "float64") {
        if (contains(p.def_val, "NOPOS")) {
            return "0.0";
        } else if (!p.def_val.empty())
            return p.def_val;
        return "0.0";
    } else if (p.go_intype == "string") {
        if (contains(p.def_val, ".eth")) {  // an address
            return "\"" + p.def_val + "\"";
        } else {
            return p.def_val;
        }
    } else if (p.go_intype == "uint64") {
        if (contains(p.def_val, "NOPOS")) {
            return "0";
        } else if (!p.def_val.empty() && !startsWith(p.def_val, "(")) {
            return p.def_val;
        }
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
string_q clean_go_positionals(const string_q& in, bool hasRpc) {
    string_q ret = in;
    replaceAll(ret, "\t[]string{} = args\n", "");
    replaceAll(ret, "opts.[]string{}", "[]string{}");
    if (!contains(ret, "utils."))
        replaceAll(ret, "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils\"\n", "");
    if (!contains(ret, "identifiers."))
        replaceAll(ret, "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers\"\n", "");
    if (!hasRpc)
        replaceAll(ret, "\t\"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc\"\n", "");
    return ret;
}

string_q get_hidden(const CCommandOption& cmd) {
    ostringstream os;
    for (auto p : *((CCommandOptionArray*)cmd.members)) {
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
    for (auto p : *((CCommandOptionArray*)cmd.members)) {
        if (p.option_type != "positional") {
            os << "\t[{ROUTE}]Cmd.Flags().";
            os << p.go_flagtype;
            os << "(&[{ROUTE}]Pkg.GetOptions().";
            if (p.isConfig) {
                os << "Settings." + p.Format("[{VARIABLE}]") << ", ";
            } else {
                os << p.Format("[{VARIABLE}]") << ", ";
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

//--------------------------------------------------------------------------------
string_q clean_positionals(const string_q& progName, const string_q& strIn) {
    if (contains(strIn, "<")) {
        ostringstream os;
        os << (strIn == "list<addr>" ? "<address> [address...]" : "");
        os << (strIn == "list<blknum>" ? "<block> [block...]" : "");
        os << (strIn == "list<tx_id>" ? "<tx_id> [tx_id...]" : "");
        os << (startsWith(strIn, "list<addr> list<topic> list<fourbyte>")
                   ? "<address> [address...] [topics...] [fourbytes...]"
                   : "");
        os << (startsWith(strIn, "list<enum") ? "<mode> [mode...]" : "");

        if (contains(toLower(progName), "tokens")) {
            os << (strIn == "list<addr> list<blknum>" ? "<address> <address> [address...] [block...]" : "");

        } else if (contains(toLower(progName), "chunks")) {
            os << (strIn == "enum[manifest|index|blooms|pins|addresses|appearances|stats] list<blknum>"
                       ? "<mode> [blocks...] [address...]"
                       : "");

        } else {
            os << (strIn == "list<addr> list<blknum>" ? "<address> [address...] [block...]" : "");
        }

        if (contains(toLower(progName), "when"))
            os << (strIn == "list<string>" ? "< block | date > [ block... | date... ]" : "");
        else
            os << (strIn == "list<string>" ? "<term> [term...]" : "");

        if (os.str().empty()) {
            cerr << "Could not convert " << strIn << " for tool " << progName << endl;
            os << strIn;
        }

        os << endl;
        return " " + os.str();
    }
    string_q strOut = strIn;
    replaceAll(strOut, "addrs blocks", "<address> [address...] [block...]");
    replaceAll(strOut, "transactions", "<tx_id> [tx_id...]");
    if (contains(progName, "when"))
        replaceAll(strOut, "blocks", "< block | date > [ block... | date... ]");
    else
        replaceAll(strOut, "blocks", "<block> [block...]");
    replaceAll(strOut, "addrs topics fourbytes", "<address> [address...] [topics] [fourbytes]");
    replaceAll(strOut, "addrs", "<address> [address...]");
    replaceAll(strOut, "files", "<file> [file...]");
    replaceAll(strOut, "terms", "<term> [term...]");
    replaceAll(strOut, "modes", "<mode> [mode...]");
    return trim(strOut);
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
