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
    COption("tsx", "t", "", OPT_SWITCH, "create typescript routes"),
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

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("files", files, files.empty());
    LOG_TEST_BOOL("all", all);
    LOG_TEST_BOOL("options", options);
    LOG_TEST_BOOL("gocmds", gocmds);
    LOG_TEST_BOOL("readmes", readmes);
    LOG_TEST_BOOL("format", format);
    LOG_TEST_BOOL("lint", lint);
    LOG_TEST_BOOL("tsx", tsx);
    LOG_TEST_BOOL("dump", dump);
    LOG_TEST_BOOL("openapi", openapi);
    // END_DEBUG_DISPLAY

    establishFolder(getDocsPathContent(""));
    establishFolder(getDocsPathContent("api/"));
    establishFolder(getDocsPathContent("data-model/"));
    establishFolder(getDocsPathContent("docs/"));
    establishFolder(getDocsPathContent("docs/chifra/"));

    if (tsx)
        handle_tsx();

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
    LOG4("Processing ", classDefs.size(), " class definition files.");

    for (auto classDefIn : classDefs) {
        CToml toml(classDefIn.input_path);
        CClassDefinition classDef(toml);
        classDef.short_fn = classDefIn.short_fn;
        classDef.input_path = classDefIn.input_path;
        if (!classDef.doc_api.empty())
            dataModels.push_back(classDef);
    }

    if (gocmds && !options)
        options = true;

    // Ignoring classDefs for a moment, process special options. Note: order matters
    if (openapi && !handle_datamodel())
        return false;
    if (options && !handle_options())
        return false;
    if (gocmds && !handle_gocmds())
        return false;
    if (readmes && !handle_readmes())
        return false;
    if (format && !handle_format())
        return false;
    if (lint && !handle_lint())
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
    if (!folderExists(configPath("makeClass/")))
        return false;  // usage(usageErrs[ERR_CONFIGMISSING]);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    all = false;
    tsx = false;
    openapi = false;
    // END_CODE_INIT

    classDefs.clear();
    counter = CCounter();

    CToml toml(configPath("makeClass.toml"));
    lastFormat = static_cast<timestamp_t>(toml.getConfigInt("settings", "lastFormat", 0));
    lastLint = static_cast<timestamp_t>(toml.getConfigInt("settings", "lastLint", 0));
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
    usageErrs[ERR_CONFIGMISSING] = "[{CONFIG_FOLDER}]makeClass/ folder does not exist.";
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
