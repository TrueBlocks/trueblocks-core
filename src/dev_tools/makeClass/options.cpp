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
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("files", "", "list<path>", OPT_REQUIRED | OPT_POSITIONAL, "one or more class definition files"),
    COption("all", "a", "", OPT_SWITCH, "list, or run all class definitions found in the local folder"),
    COption("options", "o", "", OPT_SWITCH, "export options code (check validity in the process)"),
    COption("gocmds", "g", "", OPT_SWITCH, "export go command code"),
    COption("readmes", "m", "", OPT_SWITCH, "create readme files for each tool and app"),
    COption("format", "f", "", OPT_SWITCH, "format source code files (.cpp and .h) found in local folder and below"),
    COption("lint", "l", "", OPT_SWITCH, "lint source code files (.cpp and .h) found in local folder and below"),
    COption("tsx", "t", "", OPT_SWITCH, "create typescript routes, help text and types for the front end"),
    COption("sdk", "s", "", OPT_SWITCH, "create typescript sdk"),
    COption("dump", "d", "", OPT_HIDDEN | OPT_SWITCH, "dump any classDefinition config tomls to screen and quit"),
    COption("openapi", "A", "", OPT_HIDDEN | OPT_SWITCH, "export openapi.yaml file for API documentation"),
    COption("", "", "", OPT_DESCRIPTION, "Automatically writes C++ for various purposes."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray files;
    bool options = false;
    bool gocmds = false;
    bool readmes = false;
    bool format = false;
    bool lint = false;
    bool dump = false;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg == "-o" || arg == "--options") {
            options = true;

        } else if (arg == "-g" || arg == "--gocmds") {
            gocmds = true;

        } else if (arg == "-m" || arg == "--readmes") {
            readmes = true;

        } else if (arg == "-f" || arg == "--format") {
            format = true;

        } else if (arg == "-l" || arg == "--lint") {
            lint = true;

        } else if (arg == "-t" || arg == "--tsx") {
            tsx = true;

        } else if (arg == "-s" || arg == "--sdk") {
            sdk = true;

        } else if (arg == "-d" || arg == "--dump") {
            dump = true;

        } else if (arg == "-A" || arg == "--openapi") {
            openapi = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseStringList2(this, files, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    string_q aliasesFn = getDocsPathTemplates("aliases.csv");
    if (fileExists(aliasesFn)) {
        CStringArray lines;
        asciiFileToLines(aliasesFn, lines);
        for (auto line : lines) {
            CStringArray parts;
            explode(parts, line, ',');
            // cerr << line << " --> " << parts[0] << " --> " << parts[1] << endl;
            if (parts.size() == 2) {
                if (!hugoAliasMap[parts[0]].empty()) {
                    hugoAliasMap[parts[0]] += ",";
                }
                hugoAliasMap[parts[0]] += parts[1];
            }
        }
    }

    if (readmes || openapi) {
        establishFolder(getDocsPathContent(""));
        establishFolder(getDocsPathContent("api/"));
        establishFolder(getDocsPathContent("data-model/"));
        establishFolder(getDocsPathContent("docs/"));
        establishFolder(getDocsPathContent("chifra/"));
    }

    string_q endpointsFile = getPathToSource("cmd-line-endpoints.csv");
    if (!fileExists(endpointsFile)) {
        endpointsFile = "./cmd-line-endpoints.csv";
        if (!fileExists(endpointsFile)) {
            return usage("Could not find " + endpointsFile);
        }
    }
    forEveryLineInAsciiFile(endpointsFile, parseEndpointsFile, this);

    string_q optionsFile = getPathToSource("cmd-line-options.csv");
    if (!fileExists(optionsFile)) {
        optionsFile = "./cmd-line-options.csv";
        if (!fileExists(optionsFile)) {
            return usage("Could not find " + optionsFile);
        }
    }
    forEveryLineInAsciiFile(optionsFile, parseOptionsFile, this);

    CToml config(rootConfigToml_makeClass);
    bool enabled = config.getConfigBool("enabled", "generate", false);
    if (!enabled) {
        LOG_WARN("Skipping code generation...");
        return false;
    }

    verifyDescriptions();
    if (gocmds) {
        verifyGoEnumValidators();
    }

    if (tsx) {
        handle_tsx();
    }

    // If the user has explicitly specified a classDef, use that
    LOG8("pwd: ", getCWD());
    for (auto file : files) {
        CClassDefinition cl;
        if (fileExists(file)) {
            cl.short_fn = substitute(substitute(file, "./classDefinitions/", ""), ".txt", "");
            cl.input_path = file;
        } else {
            cl.short_fn = file;
            cl.input_path = "./classDefinitions/" + file + ".txt";
        }
        classDefs.push_back(cl);
    }

    // If the user has not specified a classDef, try to find some
    if (classDefs.empty()) {
        // If not in a folder with a specific classDef, try to produce all classDefs
        if (!folderExists("./classDefinitions")) {
            forEveryFileInFolder("./", listClasses, this);
        } else {
            forEveryFileInFolder("./classDefinitions/", listClasses, this);
        }
    }
    LOG_INFO("Processing ", classDefs.size(), " class definition files.");

    for (auto classDefIn : classDefs) {
        CToml toml(classDefIn.input_path);
        CClassDefinition classDef(toml);
        classDef.short_fn = classDefIn.short_fn;
        classDef.input_path = classDefIn.input_path;
        if (!classDef.doc_route.empty()) {
            dataModels.push_back(classDef);
        }
    }

    if (gocmds && !options) {
        options = true;
    }

    // Ignoring classDefs for a moment, process special options. Note: order matters
    if (openapi && !handle_datamodel())
        return false;
    if (options && !handle_options())
        return false;
    if (openapi && !writeOpenApiFile())
        return false;
    if (gocmds && !handle_gocmds())
        return false;
    if (readmes && !handle_readmes())
        return false;
    if (format && !handle_format())
        return false;
    if (lint && !handle_lint())
        return false;
    if (sdk && !handle_sdk())
        return false;

    // Default to run if we get only all

    // Maybe the user only wants to generate code, format, or lint...
    if (all && (options + format + lint + readmes) > 0)
        return false;

    // If not, we need classDefs to work with...
    if (classDefs.empty())
        return usage(usageErrs[ERR_NEEDONECLASS]);

    // If we're dumping, dump...
    if (dump) {
        for (auto cl : classDefs) {
            CToml toml(cl.input_path);
            if (verbose) {
                SHOW_FIELD(CClassDefinition, "fieldArray");
                HIDE_FIELD(CClassDefinition, "field_str");
                manageFields("CParameter:is_enabled,is_flags", false);
                CClassDefinition d(toml);
                cout << d << endl;
                HIDE_FIELD(CClassDefinition, "fieldArray");
                SHOW_FIELD(CClassDefinition, "field_str");
            } else {
                cout << string_q(120, '-') << endl << cl.input_path << endl << string_q(120, '-') << endl;
                cout << toml << endl;
            }
        }
        // Maybe we're done..
        if (!all)
            return false;
    }

    // We need the template files
    CStringArray templs = {"", "blank.yaml", "blank.cpp", "blank.h", "blank.go.tmpl", "blank_options.go.tmpl"};
    for (auto temp : templs) {
        if (!fileExists(getPathToTemplates(temp))) {
            return makeError(ERR_CONFIGMISSING, getPathToTemplates(temp));
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    all = false;
    tsx = false;
    sdk = false;
    openapi = false;
    // END_CODE_INIT

    classDefs.clear();
    counter = CCounter();

    CToml toml(rootConfigToml_makeClass);
    lastFormat = static_cast<timestamp_t>(toml.getConfigInt("settings", "last_format", 0));
    lastLint = static_cast<timestamp_t>(toml.getConfigInt("settings", "last_lint", 0));
    toml.Release();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The `--options` flag generates `COption` code for each of the various tools.");
    notes.push_back("More information on class definition files is found in the documentation.");
    // clang-format on
    // END_CODE_NOTES

    usageErrs[ERR_NOERROR] = "No error";
    // ERROR_STRINGS
    usageErrs[ERR_CLASSDEFNOTEXIST] = "./classDefinitions folder does not exist.";
    usageErrs[ERR_CONFIGMISSING] = "File {0} does not exist.";
    usageErrs[ERR_NEEDONECLASS] = "Please specify at least one className.";
    // ERROR_STRINGS

    CCommandOption::registerClass();
    CClassDefinition::registerClass();
    CCommandOption::registerClass();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool listClasses(const string_q& path, void* data) {
    if (contains(path, "/test/") || contains(path, "/build/"))
        return true;

    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", listClasses, data);

    } else {
        if (contains(path, "apps.toml"))
            return true;

        bool isDef = contains(path, "classDefinitions/");
        bool isTxt = contains(path, ".txt");
        bool isToml = contains(path, ".toml");
        if (isDef && (isTxt || isToml)) {
            COptions* opts = reinterpret_cast<COptions*>(data);

            string_q class_name = path;
            class_name = substitute(substitute(nextTokenClearReverse(class_name, '/'), ".txt", ""), ".toml", "");

            CClassDefinition cl;
            cl.short_fn = class_name;
            cl.input_path = path;
            LOG8("Adding: ", cl.short_fn, " ", cl.input_path, " ", cl.outputPath(".cpp"));
            opts->classDefs.push_back(cl);
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
string_q getPathToDocs(const string_q& _part) {
    return "../docs/" + _part;
}

//--------------------------------------------------------------------------------
string_q getDocsPathContent(const string_q& _part) {
    return getPathToDocs("content/" + _part);
}

//--------------------------------------------------------------------------------
string_q getDocsPathTemplates(const string_q& _part) {
    return getPathToDocs("templates/" + _part);
}

//--------------------------------------------------------------------------------
string_q getDocsPathReadmes(const string_q& _part) {
    return getPathToDocs("readmes/" + _part);
}

//--------------------------------------------------------------------------------
#define getSourcePathA(a) (string_q("../src/") + (a))
string_q getPathToSource(const string_q& rest) {
    string_q cwd = getCWD();
    CStringArray parts;
    explode(parts, cwd, '/');
    string_q ret;
    for (auto part : parts) {
        if (part != "trueblocks-core")
            ret += ("/" + part);
        else
            return ret + "/trueblocks-core/src/" + rest;
    }
    return getSourcePathA(rest);
}

string_q getPathToTemplates(const string_q& part) {
    string_q ret = getPathToSource("dev_tools/makeClass/templates/" + part);
    if (!fileExists(ret)) {
        ret = getHomeFolder() + "Development/trueblocks-core/src/dev_tools/makeClass/templates/" + part;
    }
    return ret;
}

//---------------------------------------------------------------------------------------------------
bool parseEndpointsFile(const char* str, void* data) {
    if (startsWith(str, ';'))
        return true;

    string_q line = str;
    replaceAny(line, "\n\r", "");

    /* The first row is the field names */
    static CStringArray fields;
    if (fields.empty()) {
        explode(fields, line, ',');
        return true;
    }

    COptions* opts = (COptions*)data;
    CCommandOption option;
    option.parseCSV(fields, line);
    opts->endpointArray.push_back(option);

    return true;
}

//---------------------------------------------------------------------------------------------------
bool parseOptionsFile(const char* str, void* data) {
    if (startsWith(str, ';'))
        return true;

    string_q line = str;
    replaceAny(line, "\n\r", "");

    /* The first row is the field names */
    static CStringArray fields;
    if (fields.empty()) {
        explode(fields, line, ',');
        return true;
    }

    COptions* opts = (COptions*)data;
    CCommandOption option;
    option.parseCSV(fields, line);
    if (!option.tool.empty() && option.tool != "all" && option.tool != "tool") {
        string_q key = option.tool + "-" + option.longName + option.option_type;
        if (!opts->cmdExistsMap[key]) {
            opts->cmdOptionArray.push_back(option);
            opts->cmdExistsMap[key] = true;
        }
        opts->routeOptionArray.push_back(option);
        opts->toolMap[option.group + "/" + option.tool] = true;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::verifyDescriptions(void) {
    for (auto ep : endpointArray) {
        if (!ep.is_visible)
            continue;
        for (auto option : cmdOptionArray) {
            if (ep.api_route == option.api_route && option.option_type == "description") {
                if (ep.description != option.description) {
                    ostringstream os;
                    os << "Endpoint descriptions for " << ep.api_route << " do not agree:" << endl;
                    os << "\tendpoint: " << ep.description << endl;
                    os << "\toption:   " << option.description << endl;
                    LOG_WARN(os.str());
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------
bool isChifraRoute(const CCommandOption& cmd, bool depOk) {
    if (depOk && cmd.option_type == "deprecated")
        return true;
    return (cmd.option_type != "deprecated" && cmd.option_type != "description" && cmd.option_type != "note" &&
            cmd.option_type != "alias" && cmd.option_type != "config" && cmd.option_type != "error");
}
