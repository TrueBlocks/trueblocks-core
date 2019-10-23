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
    COption("classes", "", "list(<string>)", OPT_REQUIRED | OPT_POSITIONAL, "one or more class definition files to process"),
    COption("list", "l", "", OPT_SWITCH, "list all definition files found in the local ./classDefinitions folder"),
    COption("run", "r", "", OPT_SWITCH, "run the class maker on associated <class_name(s)>"),
    COption("edit", "e", "", OPT_HIDDEN | OPT_SWITCH, "edit <class_name(s)> definition file in local folder"),
    COption("all", "a", "", OPT_SWITCH, "list, or run all class definitions found in the local folder"),
    COption("js", "j", "<string>", OPT_FLAG, "export javaScript code and quit"),
    COption("options", "o", "", OPT_SWITCH, "export options code (check data, generate code) and quit"),
    COption("nspace", "n", "<string>", OPT_FLAG, "surround the code with a namespace"),
    COption("filter", "f", "<string>", OPT_FLAG, "process only files whose filename or contents contain 'filter'"),
    COption("", "", "", OPT_DESCRIPTION, "Creates one or more C++ classes based on the definition file at ./classDefinition/<class_name>."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

// BEG_CODE_LOCAL_INIT
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

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

// END_CODE_AUTO
        } else {
            if (!classNames.empty())
                classNames += "|";
            classNames += substitute(substitute(arg, "classDefinitions/", ""), ".txt", "");

        }
    }

    if (options)
        return !handle_options();

    if (!js.empty())
        return exportJson(js);

    if (contains(command, "-j") && js.empty())
        return usage("Cannot export javscript for an empty class. Quitting...");

    if (!folderExists("./classDefinitions/"))
        return usage("./classDefinitions folder does not exist. Quitting...");

    if (!folderExists(configPath("makeClass/")))
        return usage(configPath("makeClass/") + " folder does not exist. Quitting...");

    if ((run + list + edit) > 1)
        return usage("Please chose only one of --run, --list, or --edit. Quitting...");

    if (!run && !list && !edit)
        return usage("Please chose one of --run, --list, or --edit. Quitting...");

    mode = (run ? RUN : edit ? EDIT : LIST);
    LOG4("run: ", run, " edit: ", edit, " list: ", list, " mode: ", mode);
    if (list || all) {
        classNames = "";  // rebuild the class list from the classDefinitions folder
        forEveryFileInFolder("./classDefinitions/", listClasses, this);
    }

    if (classNames.empty()) {
        if (!filter.empty())
            return usage("Found no classes that matched the filter: " + filter + ". Quitting...");
        else
            return usage("You must specify at least one className (or -a or -l). Quitting...");
    }

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
// END_CODE_INIT

    mode = NONE;
    classNames = "";
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "classes", "<class_name> [class_name...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        return "";

    }
    return str;
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
                include = (file == opts->filter) || contains(contents, opts->filter);
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
    CBaseNode *item = createObjectOfType(cl);
    if (!item)
        return usage("Class " + cl + " not found.");
    CRuntimeClass *pClass = item->getRuntimeClass();
    pClass->forEveryField(visitField, &cout);
    return true;
}

