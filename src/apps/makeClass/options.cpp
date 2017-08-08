/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"

CParams params[] = {
    CParams("~className",    "name of C++ class(es) to process"),
//    CParams("-clean",        "remove generated .cpp and .h file from current folder (if found)"),
    CParams("-edit",         "edit <className(s)> definition file in local folder"),
    CParams("-filter",       "process only files with :filter in their names"),
    CParams("-list",         "list all definition files found in the local folder"),
    CParams("-namespace",    "surround the code with a --namespace:ns"),
    CParams("-silent",       "on error (no classDefinition file) exit silently"),
    CParams("-run",          "run the class maker on associated <className(s)>"),
    CParams("-all",          "clear, edit, list, or run all class definitions found in the local folder"),
    CParams("",              "Creates a C++ class based on definitions found in ./classDefinition/<className>.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        if ((arg == "-e" || arg == "-edit")) {
            if (isRun)
                return usage("Incompatible options '-r' and '-e'. Choose one or the other.");
            isEdit = true;
            isRemove = isList = false;  // last in wins

//        } else if (arg == "-c" || arg == "-clean") {
//            if (isRun)
//                return usage("Incompatible options '-r' and '-c'. Choose one or the other.");
//            isRemove = true;
//            isEdit = isList = false;  // last in wins

        } else if (arg == "-l" || arg == "-list") {
            if (isRun)
                return usage("Incompatible options '-r' and '-l'. Choose one or the other.");
            isList = true;
            isRemove = isEdit = false;  // last in wins

        } else if (arg == "-s" || arg == "--silent") {
            silent = true;

        } else if (arg.startsWith("-n:") || arg.startsWith("--namespace:")) {

            namesp = arg.Substitute("-n:", "").Substitute(".--namespace:", "");

        } else if (arg.Contains("-f")) {
            SFString orig = arg;
            SFString arg1 = nextTokenClear(arg, ':');
            if (arg1 != "-f" && arg1 != "--filter")
                return usage("Unknown parameter: " + orig);
            filter = arg;

        } else if (arg == "-r" || arg == "-run") {
            if (isEdit || isRemove || isList)
                return usage("Incompatible options with '-r'. Only '-a' option works with '-r'.");
            isRun = true;

        } else if (arg == "-a" || arg == "-all") {
            isAll = true;

        } else if (!arg.startsWith('-')) {
            if (!classNames.empty())
                classNames += "|";
            classNames += arg.Substitute("classDefinitions/","").Substitute(".txt","");

        } else if (arg != "-t" && arg != "-h" && !arg.Contains("-v")) {
            return usage("Unknown parameter: " + arg);
        }
    }

    SFString errMsg;
    if (!folderExists("./classDefinitions/"))
        errMsg = "./classDefinitions folder does not exist. Quitting...";

    if (!folderExists(configPath("makeClass/")))
        errMsg = (configPath("makeClass") + " folder does not exist. Quitting...");

    if (isAll || isList) {
        classNames = EMPTY;  // rebuild the class list from the classDefinitions folder
        forEveryFileInFolder("./classDefinitions/", listClasses, this);
    }

    if (classNames.empty()) {
        if (!filter.empty())
            errMsg = "Found no classes that matched the filter: " + filter;
        else
            errMsg = "You must specify at least one className (or -a or -l)";
    }

    if (!isList && !isEdit && !isRemove && !isRun)
        errMsg = "You must specify at least one of --run, --list, --edit, or --clear";

    if (silent && !errMsg.empty())
        return false;

    if (!errMsg.empty())
        return usage(errMsg);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    isEdit = false;
    isRemove = false;
    isRun = false;
    isList = false;
    isAll = false;
    silent = false;
    namesp = "";
    classNames = "";
    filter = "";

    useVerbose = true;
    useTesting = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    Init();
    // header = "";
    // footer = "";
    // seeAlso = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//---------------------------------------------------------------------------------------------------
bool listClasses(const SFString& path, void *data) {
    if (path.endsWith("/")) {
        forAllFiles(path + "*", listClasses, data);

    } else {
        if (path.Contains(".txt")) {
            SFString file = path;
            file = nextTokenClearReverse(file, '/').Substitute(".txt", "");
            COptions *opts = reinterpret_cast<COptions*>(data);
            bool include = true;
            if (!opts->filter.empty()) {
                SFString contents = asciiFileToString(path);
                include = contents.Contains(opts->filter);
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
