#include "utillib.h"
#include "options.h"

static const COption params[] = {
    COption("files", "", "list<path>", OPT_REQUIRED | OPT_POSITIONAL, "one or more class definition files"),
    COption("all", "a", "", OPT_SWITCH, "list, or run all class definitions found in the local folder"),
    COption("", "", "", OPT_DESCRIPTION, "Automatically writes C++ for various purposes."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

bool parseStringList2(COptionsBase* opt, CStringArray& strings, const string& argIn) {
    strings.push_back(argIn);
    return true;
}

bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    CStringArray files;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseStringList2(this, files, arg))
                return false;
        }
    }

    string_q aliasesFn = "../templates/aliases.csv";
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

    establishFolder("../content/");
    establishFolder("../content/api/");
    establishFolder("../content/data-model/");
    establishFolder("../content/docs/");
    establishFolder("../content/chifra/");
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

    forEveryFileInFolder("./classDefinitions/", listClasses, this);
    for (auto classDefIn : classDefs) {
        CToml toml(classDefIn.input_path);
        CClassDefinition classDef;
        classDef.ReadSettings(toml);
        classDef.short_fn = classDefIn.short_fn;
        classDef.input_path = classDefIn.input_path;
        if (!classDef.doc_route.empty()) {
            dataModels.push_back(classDef);
        }
    }

    return handle_readmes();
}

void COptions::Init(void) {
    registerOptions(nParams, params);
    all = false;
    classDefs.clear();
}

COptions::COptions(void) : classFile("") {
    Init();
}

COptions::~COptions(void) {
}

bool listClasses(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", listClasses, data);
    } else {
        bool isDef = contains(path, "classDefinitions/");
        bool isToml = contains(path, ".toml");
        if (isDef && isToml) {
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
    }

    return true;
}
