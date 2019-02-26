/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
extern bool moveToProduction(const string_q& str, void *data);
#define toProduction(a) substitute(a, "/staging/", "/")
extern string_q colors[];
extern uint64_t nColors;
bool COptions::handle_init(void) {

    if (isTestMode())
        return true;

    if (watches.size() > 0)
        return usage("This folder has already been initialized. Quitting...");

    if (!remainder.empty()) {
        if (!startsWith(remainder, "0x"))
            return usage("The init command accepts an address/name pair. Quitting...");
        while (!remainder.empty()) {
            CAccountWatch watch;
            watch.address = nextTokenClear(remainder, '|');
            watch.name = trim(remainder, '|');
            if (watch.name.empty())
                watch.name = watch.address;
            watch.color = colors[watches.size()%nColors];
            watches.push_back(watch);
            if (monitorName.empty())
                monitorName = watch.name;
            cerr << cGreen << cOff << "\tAdded watch: " << watch.color << watch.address << cOff << " (" << watch.name << ")" << endl;
        }

    } else {
        CQuestion prompt("Enter '<address> <name>' pairs ('q' to quit, 'n' for names, 'h' for help)", true, cTeal, NULL);
        prompt.getResponse();
//#define DEBUGY
#ifdef DEBUGY
        prompt.answer = "0xb9e7f8568e08d5659f5d29c4997173d84cdf2607 Swarm";
#endif
        while (!prompt.answer.empty()) {
            if (prompt.answer == "h" || prompt.answer == "help") {
                handle_help();
                prompt.answer = "cont";
            } else if (prompt.answer == "n" || prompt.answer == "names") {
                handle_names();
                prompt.answer = "cont";
            } else if (prompt.answer == "q" || prompt.answer == "quit" || prompt.answer == "exit") {
                prompt.answer = "";
            } else {
                CAccountWatch watch;
                replaceAll(prompt.answer,"\t"," ");
                replaceAll(prompt.answer,"  "," ");
                watch.address = nextTokenClear(prompt.answer, ' ');
                if (!isAddress(watch.address)) {
                    cerr << cRed << "\tError: " << cOff << "Invalid ethereum address. Please try again..." << endl;
                    prompt.answer = "cont";
                } else if (prompt.answer.empty()) {
                    cerr << cRed << "\tError: " << cOff << "You must provide a name and an address. Please try again..." << endl;
                    prompt.answer = "cont";
                } else {
                    watch.name = nextTokenClear(prompt.answer, ' ');
                    if (monitorName.empty())
                        monitorName = watch.name;
                    watch.color = colors[watches.size()%nColors];
                    watches.push_back(watch);
                    cerr << cGreen << cOff << "\tAdded watch: " << watch.color << watch.address << cOff << " (" << watch.name << ")" << endl;
                }
            }
#ifdef DEBUGY
            prompt.getResponse();
#endif
        }
    }

    if (watches.size() == 0) {
        cerr << "\tQuitting without create a monitor" << endl;
        return false;
    }

    return makeNewMonitor();
}

//----------------------------------------------------------------
bool moveToProduction(const string_q& path, void *data) {
//    COptions *options = reinterpret_cast<COptions*>(data);
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", moveToProduction, data);
    } else {
        if (endsWith(path, ".toml") ||
            endsWith(path, ".txt") ||
            endsWith(path, ".acct.bin") ||
            endsWith(path, ".bak"))
            moveFile(path, toProduction(path));
    }
    return true;
}

//----------------------------------------------------------------
bool COptions::makeNewMonitor(void) {
    const char* STR_WATCH2 =
    "    { address = \"{ADDR}\", name = \"{NAME}\", firstBlock = {FB}, color = \"{COLOR}\" },\n";

    string_q stagingPath = blockCachePath("monitors/staging/" + toLower(monitorName) + "/");
    establishFolder(stagingPath + "cache/");
    string_q fileName = stagingPath + "config.toml";
    if (fileExists(fileName) || fileExists(toProduction(fileName)))
        return usage("This monitor already exists. Quitting...");
    cerr << cTeal << "Creating configuration file: " << fileName << endl;

    string_q config;
    asciiFileToString(configPath("chifra/config.toml"), config);
    replace(config, "[{NAME}]", monitorName);

    string_q watches2;
    for (auto watch : watches) {
        string_q addr = watch.address;
        cout << cTeal << "\tAdding monitor for address '" << addr << "'..." << cOff << endl;

        watches2 += STR_WATCH2;
        replaceAll(watches2, "{ADDR}", addr);
        replaceAll(watches2, "{NAME}", watch.name);
        replaceAll(watches2, "{FB}", uint_2_Str(0));
        replaceAll(watches2, "{COLOR}", watch.color);
    }

    replace(config, "[{JSON_WATCH}]", "list = [\n" + watches2 + "]\n");
    for (auto watch : watches)
        remainder += (toLower(watch.name) + "|");
    if (isTestMode()) {
        cout << fileName << endl;
        cout << config;
    } else {
        stringToAsciiFile(fileName, config);
        if (verbose > 1)
            cout << config << endl;
        handle_freshen(stagingPath);
        establishFolder(toProduction(stagingPath + "cache/"));
        forEveryFileInFolder(stagingPath, moveToProduction, this);
        ::rmdir((stagingPath + "cache/").c_str());
        ::rmdir(stagingPath.c_str());
    }
    return true;
}
