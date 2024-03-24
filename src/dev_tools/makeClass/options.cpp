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
    COption("files", "", "list<path>", OPT_REQUIRED | OPT_POSITIONAL, "one or more class definition files"),
    COption("all", "a", "", OPT_SWITCH, "list, or run all class definitions found in the local folder"),
    COption("readmes", "m", "", OPT_SWITCH, "create readme files for each tool and app"),
    COption("openapi", "A", "", OPT_SWITCH, "export openapi.yaml file for API documentation"),
    COption("protobuf", "p", "", OPT_SWITCH, "compile protobufs"),
    COption("", "", "", OPT_DESCRIPTION, "Automatically writes C++ for various purposes."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//--------------------------------------------------------------------------------
bool parseStringList2(COptionsBase* opt, CStringArray& strings, const string& argIn) {
    strings.push_back(argIn);
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    CStringArray files;
    bool readmes = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg == "-m" || arg == "--readmes") {
            readmes = true;

        } else if (arg == "-A" || arg == "--openapi") {
            openapi = true;

        } else if (arg == "-p" || arg == "--protobuf") {
            protobuf = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseStringList2(this, files, arg))
                return false;
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

    string_q endpointsFile = getPathToSource("other/data-models/cmd-line-endpoints.csv");
    if (!fileExists(endpointsFile)) {
        endpointsFile = "./other/data-models/cmd-line-endpoints.csv";
        if (!fileExists(endpointsFile)) {
            return usage("Could not find " + endpointsFile);
        }
    }
    forEveryLineInAsciiFile(endpointsFile, parseEndpointsFile, this);

    string_q optionsFile = getPathToSource("other/data-models/cmd-line-options.csv");
    if (!fileExists(optionsFile)) {
        optionsFile = "./other/data-models/cmd-line-options.csv";
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
        CClassDefinition classDef;
        classDef.ReadSettings(toml);
        classDef.short_fn = classDefIn.short_fn;
        classDef.input_path = classDefIn.input_path;
        if (!classDef.doc_route.empty()) {
            dataModels.push_back(classDef);
        } else if (!classDef.go_model.empty()) {
            LOG_WARN("go_type is on, but doc_route is not in ", classDef.class_name);
            exit(0);
        }
    }

    // Ignoring classDefs for a moment, process special options. Note: order matters
    if (openapi && !handle_datamodel())
        return false;
    if (openapi && !writeOpenApiFile())
        return false;
    if (readmes && !handle_readmes())
        return false;
    if (protobuf && !handle_protobuf())
        return false;

    // Default to run if we get only all

    // Maybe the user only wants to generate code
    if (all && readmes > 0)
        return false;

    // If not, we need classDefs to work with...
    if (classDefs.empty())
        return usage(usageErrs[ERR_NEEDONECLASS]);

    // We need the template files
    CStringArray templs = {"", "blank.yaml"};
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

    all = false;
    openapi = false;

    classDefs.clear();
    counter = CCounter();

    CToml toml(rootConfigToml_makeClass);
    lastFormat = static_cast<timestamp_t>(toml.getConfigInt("settings", "last_format", 0));
    toml.Release();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    Init();

    notes.push_back("More information on class definition files is found in the documentation.");

    usageErrs[ERR_NOERROR] = "No error";
    usageErrs[ERR_CLASSDEFNOTEXIST] = "./classDefinitions folder does not exist.";
    usageErrs[ERR_CONFIGMISSING] = "File {0} does not exist.";
    usageErrs[ERR_NEEDONECLASS] = "Please specify at least one className.";

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
bool parseEndpointsFile(const char* linePtr, void* data) {
    string_q line = linePtr;
    replaceAny(line, "\n\r", "");

    if (startsWith(line, ';'))
        return true;

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

//------------------------------------------------------------------------------------------------------------
string_q get_corrected_caps(const string_q& capsIn) {
    string_q x = "fmt|chain|noHeader";
    string_q capsOut = substitute(capsIn, "default", x);
    replace(capsOut, "caching", "cache|decache");
    replace(capsOut, "verbose|", "");
    replace(capsOut, "version|", "");
    replace(capsOut, "noop|", "");
    replace(capsOut, "noColor|", "");
    replace(capsOut, "help|", "");
    return capsOut;
}
