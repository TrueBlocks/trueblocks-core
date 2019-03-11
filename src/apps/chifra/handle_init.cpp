/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_init(void) {
    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    for (auto addr : addrs) {
        string_q stagingPath = monitorsPath + "staging/";
        establishFolder(stagingPath);

        string_q stageName, prodName;
        stageName = stagingPath + addr + ".acct.bin";
        prodName = monitorsPath + addr + ".acct.bin";

        if (!fileExists(prodName)) {
            if (!freshen_internal(stagingPath, addr, "", (contains(flags, "--force") ? BOTH : INDEX)))
                return false;
           if (verbose)
                cerr << "Moving " << cTeal << stageName << cOff << " to " << cTeal << prodName << cOff << endl;
            if (!isTestMode())
                moveFile(stageName, prodName);
            else
                cerr << "Would have moved " << stageName << " to " << prodName << endl;

            stageName = stagingPath + addr + ".last.txt";
            prodName = monitorsPath + addr + ".last.txt";
            if (verbose)
                cerr << "Moving " << cTeal << stageName << cOff << " to " << cTeal << prodName << cOff << endl;
            if (!isTestMode())
                moveFile(stageName, prodName);
            else
                cerr << "Would have moved " << stageName << " to " << prodName << endl;

        } else {
            cerr << "The file ";
            cerr << cTeal << "./" << addr << ".acct.bin" << cOff;
            cerr << " already exists. Quitting..." << endl;
        }
    }

    return true;
}

#if 0
//----------------------------------------------------------------
bool COptions::makeNewMonitor(void) {
    const char* STR_WATCH2 =
    "    { address = \"{ADDR}\", name = \"{NAME}\", firstBlock = {FB}, color = \"{COLOR}\" },\n";

    string_q stagingPath = blockCachePath("monitors/staging/" + toLower(monitorName) + "/");
    establishFolder(stagingPath + "c ache/");
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
        establishFolder(toProduction(stagingPath + "c ache/"));
        forEveryFileInFolder(stagingPath, moveToProduction, this);
        ::rmdir((stagingPath + "c ache/").c_str());
        ::rmdir(stagingPath.c_str());
    }

    return true;
}
#endif
