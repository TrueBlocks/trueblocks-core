/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_freshen(const string_q& pathForce) {
    ostringstream os;
    if (!pathForce.empty()) {
        os << "cd " << pathForce << " ; ";
        os << "acctScrape --useIndex >import.txt ; cacheMan -i ; acctScrape --maxBlocks 10000000";
    } else {
        CStringArray monitors;
        explode(monitors, remainder, '|');
        for (auto monitor : monitors) {
            os << "cd " << blockCachePath("monitors/" + monitor + "/") << " ; ";
            os << "acctScrape --useIndex >import.txt ; cacheMan -i ; acctScrape --maxBlocks 10000000";
        }
    }
    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    return true;
}

/*
 // stats issue
 // api issue
 // write blocks issue
 // what is the 'blockCounted = false' issue?
 // is there a options->blkStats.nHit++ or options->transStats.nHit++ issue?
 // can we remove the oneBlock issue
 // Send the data to an api if we have one
 // Also, we optionally write blocks if we're told to do so
 return !shouldQuit();

 if (isFreshen) {
 txCache.Lock("./" + watches[0].address, modeWriteAppend, LOCK_WAIT);
 forEveryFileInFolder(indexFolder_prod, visitIndexFiles, this);
 txCache.Release();
 return false;
 }
 } else {
 namesFile = CFilename(configPath("names/names.txt"));
 loadNames();
 CQuestion addrs("Enter one or more Ethereum addresses (empty line when finished)", true, bGreen, NULL);
 while (addrs.answer.empty()) {
 addrs.getResponse();
 if (isAddress(addrs.answer)) {
 string_q name;
 for (auto item : namedAccounts)
 if (addrs.answer == item.addr)
 name = item.name;
 if (name.empty()) {
 CQuestion nameQ("\tWhat do you want to name this address?", true, bGreen, NULL);
 while (nameQ.answer.empty())
 nameQ.getResponse();
 name = nameQ.answer;
 }
 CAccountWatch watch;
 watch.address = addrs.answer;
 watch.name = name;
 watch.color = con vertColor(colors[watches.size() % nColors]);
 watches.push_back(watch);
 cout << "\tAdded " << watch.color << watch.address << cOff << " " << watch.name << endl;
 addrs.answer = "";
 } else if (addrs.answer == "n" || addrs.answer == "name") {
 for (size_t i = 0 ; i < namedAccounts.size() ; i++) {
 if (!(i%2) && i != 0)
 cout << endl;
 cout << namedAccounts[i].addr << " " << padRight(namedAccounts[i].name,25) << "  ";
 }
 if (!(namedAccounts.size()%2))
 cout << endl;
 addrs.answer = "";
 } else {
 if (!addrs.answer.empty())
 return usage("Please enter a valid Ethereum address. Quitting...");
 addrs.answer = "done";
 }
 }
 }

 if (watches.size() == 0)
 return usage("You must add at least one address. Quitting...");

 for (auto watch : watches) {
 watch.color = "";
 cout << watch << endl;
 }

 extern bool createConfig (const COptions& options, CQuestion *q);
 CQuestion qq("\tCreating configuration file...", false, bBlue, createConfig);
 createConfig(*this, &qq);
 */
