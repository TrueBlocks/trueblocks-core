/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
static CQuestion questions[] = {
    CQuestion("\n\t[{Welcome to Chifra, an Ethereum Smart Contract Monitoring System}]\n", false, cYellow),
    CQuestion("\tDo you want to install a monitor in the '[{FOLDER}]' folder?",            true,  cWhite),
    CQuestion("\n\tBuilding monitor...",                                                   false, bBlue, createFolders),
    CQuestion("\tCreating chifra rebuild command...",                                      false, bBlue, createRebuild),
    CQuestion("\tCreating configuration file...",                                          false, bBlue, createConfig ),
    CQuestion("\tCreating monitor cache...",                                               false, bBlue, createCache  ),
    CQuestion("\tAdding make command do cmake file...",                                    false, bBlue, editMakeLists),
};
static uint64_t nQuestions = sizeof(questions) / sizeof(CQuestion);

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("binary", quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (uint64_t i = 0 ; i < nQuestions ; i++) {
            CQuestion *q = &questions[i];
            replaceAll(q->question, "[{FOLDER}]", substitute(options.sourceFolder, getHomeFolder(), "./"));
            replaceAll(q->question, "[{",bTeal);
            replaceAll(q->question, "}]",cOff);
            if (q->getResponse()) {
                cout << q->answer;
                if (q->func) {
                    if (!(*q->func)(options, q)) {
                        cerr << cRed << "Command failed...\n" << cOff;
                        exit(0);
                    }
                }
            } else {
                ASSERT(0);  // cannot happen
            }
        }
    }
    cout << "\n";
    return 0;
}

//--------------------------------------------------------------
bool createOneFolder(const string_q& parent, const string_q& folder) {

    if (!folderExists(parent)) {
        cerr << "Parent folder " << parent << " does not exist in 'createOneFolder'. Quitting...\n";
        return false;
    }

    string_q newFolder = (contains(folder, "/") ? folder : parent + "/" + folder);
    cout << bGreen << "\t\tCreating folder: " << substitute(newFolder, getHomeFolder(), "./") << cOff << "\n";

    if (isTestMode())
        return true;

    string_q curFolder = getCWD();
    if (chdir(parent.c_str())) {} // {} avoids a compiler warning on Ubuntu
    mkdir(newFolder.c_str(), (mode_t)0755);
    if (chdir(curFolder.c_str())) {} // {} avoids a compiler warning on Ubuntu
    return folderExists(newFolder);
}

//--------------------------------------------------------------
bool createFolders(const COptions& options, CQuestion *q) {

    int exists = 0;
    exists += createOneFolder(getCWD(), options.sourceFolder);
    exists += createOneFolder(options.sourceFolder,  "source");
    exists += createOneFolder(options.sourceFolder,  "parselib");

    exists += createOneFolder(getCWD()+"../../monitors/", options.monitorFolder);
    exists += createOneFolder(options.monitorFolder, "bin");
    exists += createOneFolder(options.monitorFolder, "cache");

    return exists == 6;
}

//--------------------------------------------------------------
bool createRebuild(const COptions& options, CQuestion *q) {

    string_q script = "cd parselib\ngrabABI ";
    string_q addrs = options.addrList;
    while (!addrs.empty()) {
        string_q addr = nextTokenClear(addrs, '|');
        script += (addr + " ");
    }
    script += "-s -n $1\n";

    string_q cmd = "cd " + options.sourceFolder + "; chmod u+x ./rebuild ; ./rebuild -g 2>&1 | grep -v Reading ; cd - 2>&1";

    cout << q->color << "\tBuilding parser library...\n\t";
    cout << bGreen << "\t";
    if (!isTestMode()) {
        stringToAsciiFile(options.sourceFolder + "/rebuild", script);
        cout << substitute(doCommand(cmd), "\n", "\n\t\t");
    } else {
        cout << substitute(script, "\n", " ");
        cout << cmd << "\n";
    }
    cout << cOff << "\n";
    return true;
}

//--------------------------------------------------------------
const char* STR_WATCH2 =
"    { address = \"{ADDR}\", name = \"{NAME}\", firstBlock = {FB}, color = \"{COLOR}\" },\n";

extern string_q fixup(const string_q& in);
//--------------------------------------------------------------
bool createConfig(const COptions& options, CQuestion *q) {

    string_q config;
    asciiFileToString(configPath("chifra/config.toml"), config);
    replace(config, "[{NAME}]", options.monitorName);

    string_q watches2;
    cout << q->color << q->question << cOff << "\n";
    string_q addrs = options.addrList;
    uint32_t cnt=0;
    while (!addrs.empty()) {
        string_q addr = nextTokenClear(addrs, '|');

        CQuestion qq("\tAdding monitor for address '" + addr + "'...", false, q->color, NULL);
        qq.getResponse();

        CQuestion name("\t\tName " + int_2_Str(cnt+1) + "?", true, bGreen, NULL);name.nl=false;
        while (name.answer.empty())
            name.getResponse();

        CQuestion first("\t\tfirstBlock?", true, bGreen, NULL);first.nl=false;
        while (first.answer.empty())
            first.getResponse();
        string_q colors[] = {
            "green_c", "blue_c", "red_c", "magenta_c", "yellow_c", "teal_c", "white_b",
            "green_b", "blue_b", "red_b", "magenta_b", "yellow_b", "teal_b", "black_b",
        };
        uint64_t nColors = sizeof(colors) / sizeof(string_q);

        watches2 += STR_WATCH2;
        replaceAll(watches2, "{ADDR}", addr);
        replaceAll(watches2, "{NAME}", name.answer);
        replaceAll(watches2, "{FB}", fixup(first.answer));
        replaceAll(watches2, "{COLOR}", colors[cnt % nColors]);
        if (addrs.empty())
            replaceReverse(watches2, ",", "");
    }

    replace(config, "[{JSON_WATCH}]", "list = [\n" + watches2 + "]\n");
    if (isTestMode()) {
        cout << options.monitorFolder + "/config.toml\n";
        cout << config;
    } else {
        stringToAsciiFile(options.monitorFolder + "/config.toml", config);
        if (verbose > 1)
            cout << config << "\n";
    }

    return true;
}

//--------------------------------------------------------------
bool createCache  (const COptions& options, CQuestion *q) {
    string_q contents;
    asciiFileToString(configPath("chifra/place_holder"), contents);
    stringToAsciiFile(options.monitorFolder + "/cache/.gitignore", contents);
    return true;
}

//--------------------------------------------------------------
bool editMakeLists(const COptions& options, CQuestion *q) {
    string_q cmakeFile;
    asciiFileToString("./CMakeLists.txt", cmakeFile);
    if (!contains(cmakeFile, makeValidName(options.monitorName) + "/parselib")) {
        cmakeFile += "\n";
        cmakeFile += "add_subdirectory(" + makeValidName(options.monitorName) + "/parselib)\n";
        cmakeFile += "add_subdirectory(" + makeValidName(options.monitorName) + ")\n";
        stringToAsciiFile("./CMakeLists.txt", cmakeFile);
    }

    string_q hatchAll;
    asciiFileToString("./hatch.mak", hatchAll);
    if (!contains(hatchAll, makeValidName(options.monitorName))) {
        hatchAll += "\t@cd " + options.monitorName + " ; echo ; echo \"rebuilding " + options.monitorName + "\" ; ./rebuild -g\n";
        stringToAsciiFile("./hatch.mak", hatchAll);
    }

    return true;
}

//--------------------------------------------------------------
string_q fixup(const string_q& in) {
    if (!contains(in, "-"))
        return in;
    return substitute(in, "-", ", lastBlock = ");
}

//--------------------------------------------------------------
bool CQuestion::getResponse(void) {
    cout << color << question;
    if (wantsInput) {
        cout << " ('q' to quit)" << (nl ? "\n\t" : ": ") << "> " << cOff;
        char buff[1024];
        if (fgets(buff, 1024, stdin)) {} // leave this here--avoids a warning
        answer = substitute(string_q(buff), "\n", "");
        if (answer % "q" || answer % "quit") {
            cerr << "Quitting...\n";
            exit(0);
        }
    } else {
        cout << "\n";
    }
    cout << cOff;
    return true;
}
