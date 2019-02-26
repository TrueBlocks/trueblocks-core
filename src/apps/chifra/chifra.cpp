/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "question.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.mode == "init")
            return options.handle_init();
        else if (options.mode == "freshen")
            return options.handle_freshen();
        else if (options.mode == "config")
            return options.handle_config();
        else if (options.mode == "seed")
            return options.handle_seed();
        else if (options.mode == "list")
            return options.handle_list();
        else if (options.mode == "daemon")
            return options.handle_daemon();
        else if (options.mode == "scrape")
            return options.handle_scrape();
        else if (options.mode == "export")
            return options.handle_export();
        else if (options.mode == "ls")
            return options.handle_ls();
        else
            cerr << "Should not happen.";
    }
    return 0;
}

#if 0
FROM MAIN
//        for (uint64_t i = 0 ; i < nQuestions ; i++) {
//            CQuestion *q = &questions[i];
////            replaceAll(q->question, "[{FOLDER}]", substitute(options.sourceFolder, getHomeFolder(), "./"));
//            replaceAll(q->question, "[{",bTeal);
//            replaceAll(q->question, "}]",cOff);
//            if (q->getResponse()) {
//                cout << q->answer;
//                if (q->func) {
//                    if (!(*q->func)(options, q)) {
//                        cerr << cRed << "Command failed...\n" << cOff;
//                        exit(0);
//                    }
//                }
//            } else {
//                ASSERT(0);  // cannot happen
//            }
//        }
//    cout << "\n";

FROM OTHER CODE
//--------------------------------------------------------------
//extern bool createFolders(const COptions& options, CQuestion *q);
//extern bool createRebuild(const COptions& options, CQuestion *q);
//extern bool createCache  (const COptions& options, CQuestion *q);
//extern bool editMakeLists(const COptions& options, CQuestion *q);
//extern bool makeWatches(const COptions& options, CQuestion *q);
//--------------------------------------------------------------
//static CQuestion questions[] = {
//    CQuestion("\n\t[{Welcome to Chifra, an Ethereum Smart Contract Monitoring System}]\n", false, cYellow),
//    CQuestion("\tEnter one or more Ethereum addresses (n for list of common names)?",      true,  cWhite, makeWatches),
//    CQuestion("\tDo you want to install a monitor in the '[{FOLDER}]' folder?",            true,  cWhite),
//    CQuestion("\n\tBuilding monitor...",                                                   false, bBlue, createFolders),
//    CQuestion("\tCreating chifra rebuild command...",                                      false, bBlue, createRebuild),
//    CQuestion("\tCreating configuration file...",                                          false, bBlue, createConfig ),
//    CQuestion("\tCreating monitor cache...",                                               false, bBlue, createCache  ),
//    CQuestion("\tAdding make command do cmake file...",                                    false, bBlue, editMakeLists),
//};
//static uint64_t nQuestions = sizeof(questions) / sizeof(CQuestion);
/*
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
 //    exists += createOneFolder(getCWD(), options.sourceFolder);
 //    exists += createOneFolder(options.sourceFolder,  "source");
 //    exists += createOneFolder(options.sourceFolder,  "parselib");
 //
 //    exists += createOneFolder(getCWD()+"../../monitors/", options.monitorFolder);
 //    exists += createOneFolder(options.monitorFolder, "bin");
 //    exists += createOneFolder(options.monitorFolder, "cache");

 return exists == 6;
 }

 //--------------------------------------------------------------
 bool createRebuild(const COptions& options, CQuestion *q) {

 #if 0
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
 #endif
 return true;
 }
 #endif

 #if 0
 //--------------------------------------------------------------
 bool createCache  (const COptions& options, CQuestion *q) {
 #if 0
 string_q contents;
 asciiFileToString(configPath("chifra/place_holder"), contents);
 stringToAsciiFile(options.monitorFolder + "/cache/.gitignore", contents);
 #endif
 return true;
 }

 //--------------------------------------------------------------
 bool editMakeLists(const COptions& options, CQuestion *q) {
 #if 0
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
 #endif
 return true;
 }

 //--------------------------------------------------------------
 bool makeWatches(const COptions& options, CQuestion *q) {
 return true;
 }
 #endif
 */
#endif
