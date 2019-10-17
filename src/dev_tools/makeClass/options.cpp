/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("className", "", "<string>", OPT_REQUIRED | OPT_POSITIONAL, "name of C++ class(es) to process"),
    COption("open", "o", "", OPT_SWITCH, "edit <className(s)> definition file in local folder"),
    COption("run", "r", "", OPT_SWITCH, "run the class maker on associated <className(s)>"),
    COption("js", "j", "<string>", OPT_FLAG, "export javaScript components for 'class'"),
    COption("filter", "f", "<string>", OPT_FLAG, "process only files with :filter in their names"),
    COption("list", "l", "", OPT_SWITCH, "list all definition files found in the local folder"),
    COption("header", "h", "", OPT_SWITCH, "write headers files only"),
    COption("source", "c", "", OPT_SWITCH, "write source files only"),
    COption("namespace", "n", "<string>", OPT_FLAG, "surround the code with a --namespace:ns"),
    COption("silent", "s", "", OPT_SWITCH, "on error (no classDefinition file) exit silently"),
    COption("all", "a", "", OPT_SWITCH, "clear, edit, list, or run all class definitions found in the local folder"),
    COption("edit", "e", "", OPT_HIDDEN | OPT_SWITCH, "edit <className(s)> definition file in local folder"),
    COption("options", "p", "", OPT_HIDDEN | OPT_SWITCH, "process options (generate code, check data) and quit"),
    COption("", "", "", OPT_DESCRIPTION, "Creates C++ code based on definition file at ./classDefinition/<className>."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

// BEG_CODE_LOCAL_INIT
// END_CODE_LOCAL_INIT

    bool didJs = false;
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
// END_CODE_AUTO

        } else if (arg == "-p" || arg == "--options") {
            handle_options();
            return false;

        } else if ((arg == "-e" || arg == "--edit" || arg == "-o" || arg == "--open")) {
            if (isRun)
                return usage("Incompatible options '-r' and '-e'. Choose one or the other.");
            isEdit = true;
            isRemove = isList = false;  // last in wins

        } else if (arg == "-c" || arg == "--source") {
            writeSource = true;

        } else if (arg == "-h" || arg == "--header") {
            writeHeader = true;

        } else if (startsWith(arg, "--js:")) {
            arg = substitute(arg, "--js:", "");
            if (!exportJson(arg))
                return false;
            didJs = true;

        } else if (arg == "-l" || arg == "--list") {
            if (isRun)
                return usage("Incompatible options '-r' and '-l'. Choose one or the other.");
            isList = true;
            isRemove = isEdit = false;  // last in wins

        } else if (arg == "-s" || arg == "--silent") {
            silent = true;

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--namespace:")) {

            namesp = substitute(substitute(arg, "-n:", ""), ".--namespace:", "");

        } else if (contains(arg, "-f")) {
            string_q orig = arg;
            string_q arg1 = nextTokenClear(arg, ':');
            if (arg1 != "-f" && arg1 != "--filter")
                return usage("Unknown parameter: " + orig);
            filter = arg;

        } else if (arg == "-r" || arg == "--run") {
            if (isEdit || isRemove || isList)
                return usage("Incompatible options with '-r'. Only '-a' option works with '-r'.");
            isRun = true;

        } else if (arg == "-a" || arg == "--all") {
            isAll = true;

        } else if (!startsWith(arg, '-')) {
            if (!classNames.empty())
                classNames += "|";
            classNames += substitute(substitute(arg, "classDefinitions/", ""), ".txt", "");

        } else if (arg != "-t" && arg != "-h" && !contains(arg, "-v")) {
            return usage("Unknown parameter: " + arg);
        }
    }

    string_q errMsg;
    if (!folderExists("./classDefinitions/"))
        errMsg = "./classDefinitions folder does not exist. Quitting...";

    if (!folderExists(configPath("makeClass/")))
        errMsg = (configPath("makeClass/") + " folder does not exist. Quitting...");

    if (isAll || isList) {
        classNames = "";  // rebuild the class list from the classDefinitions folder
        forEveryFileInFolder("./classDefinitions/", listClasses, this);
    }

    if (classNames.empty()) {
        if (!filter.empty())
            errMsg = "Found no classes that matched the filter: " + filter;
        else
            errMsg = "You must specify at least one className (or -a or -l)";
    }

    if (!isList && !isEdit && !isRemove && !isRun) {
        if (didJs)
            return true;
        errMsg = "You must specify at least one of --run, --list, --open, or --clear";
    }

    if (silent && !errMsg.empty()) {
        return false;
    }

    if (!errMsg.empty()) {
        return usage(errMsg);
    }

    if (getEnvStr("NO_HEADER") == "true")
        writeSource = true;

    if (getEnvStr("NO_SOURCE") == "true")
        writeHeader = true;

    // If neither is lit, light them both
    if (!writeHeader && !writeSource) {
        writeHeader = writeSource = true;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

// BEG_CODE_INIT
// END_CODE_INIT

    isEdit = false;
    isRemove = false;
    isRun = false;
    isList = false;
    isAll = false;
    silent = false;
    namesp = "";
    classNames = "";
    filter = "";
    writeHeader = false;
    writeSource = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//---------------------------------------------------------------------------------------------------
bool listClasses(const string_q& path, void *data) {
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", listClasses, data);

    } else {
        if (contains(path, ".txt")) {
            string_q file = path;
            file = substitute(nextTokenClearReverse(file, '/'), ".txt", "");
            COptions *opts = reinterpret_cast<COptions*>(data);
            bool include = true;
            if (!opts->filter.empty()) {
                string_q contents;
                asciiFileToString(path, contents);
                include = contains(contents, opts->filter);
            }
            if (include) {
                if (!opts->classNames.empty())
                    opts->classNames += "|";
                opts->classNames += file;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
bool visitField(const CFieldData& field, void *data) {
    ostream *pOs = (ostream*)data;
    *pOs << "<Row ";
    *pOs << "name=\"" << field.getName() << "\" ";
    *pOs << "type=\"string\" ";
    *pOs << "value={item." << field.getName() << "} ";
    *pOs << "display={item." << field.getName() << "} ";
    *pOs << "route=\"\" ";
    *pOs << "/>" << endl;
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::exportJson(const string_q& cl) {
    if (cl.empty())
        return usage("Cannot export js for empty class.");

    CBaseNode *item = createObjectOfType(cl);
    if (!item)
        return usage("Class " + cl + " not found.");
    CRuntimeClass *pClass = item->getRuntimeClass();
    pClass->forEveryField(visitField, &cout);
    return true;
}

