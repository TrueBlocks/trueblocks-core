/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    COption("files", "", "list<path>", OPT_REQUIRED | OPT_POSITIONAL, "one or more class definition files"),
    COption("list", "l", "", OPT_SWITCH, "list all definition files found in the local ./classDefinitions folder"),
    COption("run", "r", "", OPT_SWITCH, "run the class maker on associated <class_name(s)>"),
    COption("edit", "e", "", OPT_HIDDEN | OPT_SWITCH, "edit <class_name(s)> definition file in local folder"),
    COption("all", "a", "", OPT_SWITCH, "list, or run all class definitions found in the local folder"),
    COption("js", "j", "<string>", OPT_FLAG, "export javaScript code from the class definition"),
    COption("options", "o", "", OPT_SWITCH, "export options code (check validity in the process)"),
    COption("nspace", "n", "<string>", OPT_FLAG, "surround generated c++ code with a namespace"),
    COption("filter", "f", "<string>", OPT_FLAG, "process only files whose filename or contents contain 'filter'"),
    COption("test", "t", "", OPT_SWITCH, "for both code generation and options generation, process but do not write changes"),
    COption("", "", "", OPT_DESCRIPTION, "Automatically writes C++ for various purposes."),
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray files;
    bool list = false;
    bool run = false;
    bool edit = false;
    string_q js = "";
    bool options = false;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-l" || arg == "--list") {
            list = true;

        } else if (arg == "-r" || arg == "--run") {
            run = true;

        } else if (arg == "-e" || arg == "--edit") {
            edit = true;

        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (startsWith(arg, "-j:") || startsWith(arg, "--js:")) {
            js = substitute(substitute(arg, "-j:", ""), "--js:", "");

        } else if (arg == "-o" || arg == "--options") {
            options = true;

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--nspace:")) {
            nspace = substitute(substitute(arg, "-n:", ""), "--nspace:", "");

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--filter:")) {
            filter = substitute(substitute(arg, "-f:", ""), "--filter:", "");

        } else if (arg == "-t" || arg == "--test") {
            test = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseStringList2(this, files, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    for (auto file : files) {
        CClassDefinition cl;
        if (fileExists(file)) {
            cl.className = substitute(substitute(file, "./classDefinitions/", ""), ".txt", "");
            cl.inputPath = file;
        } else {
            cl.className = file;
            cl.inputPath = "./classDefinitions/" + file + ".txt";
        }
        classDefs.push_back(cl);
    }

    if (options)
        return !handle_options();

    if (!js.empty())
        return handle_json_export(js);

    if (contains(command, "-j"))
        return usage(errStrs[ERR_EMPTYJSFILE]);

    if (!all && !folderExists("./classDefinitions/"))
        return usage(errStrs[ERR_CLASSDEFNOTEXIST]);

    if (!folderExists(configPath("makeClass/")))
        return usage(errStrs[ERR_CONFIGMISSING]);

    if ((run + list + edit) > 1)
        return usage(errStrs[ERR_CHOOSEONE]);

    if (!run && !list && !edit && !all)
        return usage(errStrs[ERR_CHOOSEONE]);

    mode = (run ? RUN : edit ? EDIT : LIST);
    LOG8("run: ", run, " edit: ", edit, " list: ", list, " classes: ", all, " mode: ", mode);
    if (list || all) {
        classDefs.clear();
        if (!folderExists("./classDefinitions")) {  // if not in a folder with one class def, try to produce all classDefs
            nspace = "qblocks";
            forEveryFileInFolder("../", listClasses, this);
        }  else {
            forEveryFileInFolder("./classDefinitions/", listClasses, this);
        }
    }

    if (classDefs.empty())
            return usage(!filter.empty() ? errStrs[ERR_NOFILTERMATCH] : errStrs[ERR_NEEDONECLASS]);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_FMT);

    // BEG_CODE_INIT
    all = false;
    nspace = "";
    filter = "";
    test = false;
    // END_CODE_INIT

    mode = NONE;
    classDefs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();

    // BEG_CODE_NOTES
    notes.push_back("The `--options` flag generates `COption` code for each of the various tools.");
    notes.push_back("The `--class` flag generates c++ code for each definition found in the local folder.");
    notes.push_back("More information on class definition files is found in the documentation.");
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    errStrs[ERR_NOERROR] = "No error";
    errStrs[ERR_CLASSDEFNOTEXIST] = "./classDefinitions folder does not exist. Quitting...";
    errStrs[ERR_CONFIGMISSING] = "[{CONFIG_FOLDER}]makeClass/ folder does not exist. Quitting...";
    errStrs[ERR_EMPTYJSFILE] = "Cannot export javscript for an empty class. Quitting...";
    errStrs[ERR_CHOOSEONE] = "Please chose exactly one of --run, --list, or --edit. Quitting...";
    errStrs[ERR_NOFILTERMATCH] = "No definitions found that matched the filter: [{FILTER}]. Quitting...";
    errStrs[ERR_NEEDONECLASS] = "Please specify at least one className. Quitting...";
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//---------------------------------------------------------------------------------------------------
bool listClasses(const string_q& path, void *data) {
    if (contains(path, "/test/"))
        return true;

    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", listClasses, data);

    } else {
        if (contains(path, "classDefinitions/") && contains(path, ".txt")) {
            COptions *opts = reinterpret_cast<COptions*>(data);

            string_q class_name = path;
            class_name = substitute(nextTokenClearReverse(class_name, '/'), ".txt", "");

            bool include = true;
            if (!opts->filter.empty()) {
                string_q contents;
                asciiFileToString(path, contents);
                include = (class_name == opts->filter) || contains(contents, opts->filter);
            }

            if (include) {
                CClassDefinition cl;
                cl.className = class_name;
                cl.inputPath = path;
                LOG8("Adding: ", cl.className, " ", cl.inputPath, " ", cl.outputPath(".cpp"));
                opts->classDefs.push_back(cl);
            }
        }
    }
    return true;
}
