/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
// Run the scraper to build (or keep fresh) the address index
//------------------------------------------------------------------------------------------------
bool COptions::handle_daemon(void) {

    // daemon mode does not need a running node since it's only reading the index
    nodeNotRequired();

    size_t sleep = 14;
    CStringArray commands, filters;
    explode(commands, tool_flags, ' ');
    for (size_t i = 0; i < commands.size() ; i++) {
        //COption("-sleep", "number of seconds to sleep between runs"),
        //COption("-filter", "only runs address that start with the filter (default '0x')"),
        if (commands[i] == "--sleep" && i < commands.size() - 1) {
            sleep = str_2_Uint(commands[i+1]);
            i++;
        } else if (commands[i] == "--filter" && i < commands.size() - 1) {
            while (!commands[i+1].empty())
                filters.push_back(nextTokenClear(commands[i+1], ','));
            i++;
        } else {
            return usage("Invalid param: " + commands[i]);
        }
    }
    if (filters.empty())
        filters.push_back("0x");

    // Run until we're told not to
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    size_t nRuns = 0;
    while (nRuns++ < maxRuns && !shouldQuit()) {

        CStringArray files;
        listFilesInFolder(files, monitorsPath + "*.*", false);

        CAccountNameArray accounts;
        for (auto file : files) {
            if (contains(file, ".acct.bin") && !contains(file, ".lck")) {
                replace(file, monitorsPath, "");
                CAccountName item;
                item.addr = nextTokenClear(file, '.');
                getNamedAccount(item, item.addr);
                item.name = substitute(substitute(item.name, "(", ""), ")", "");
                accounts.push_back(item);
            }
        }

        // We want to be able to run even if there's nothing to monitor
        if (accounts.size() == 0)
            cerr << cTeal << "\tnothing to monitor" << cOff << endl;

        for (auto acct : accounts) {
            bool run = false;
            for (auto filter : filters)
                if (startsWith(acct.addr, filter))
                    run = true;
            if (run)
                freshen_internal(monitorsPath, acct.addr, "", freshen_flags);
        }

        cerr << "Sleeping for " << sleep << " seconds" << endl;
        if (!isTestMode())
            usleep((unsigned int)sleep * 1000000);
    }
    return true;
}
