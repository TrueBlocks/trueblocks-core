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

    ENTER("handle_" + mode);
    nodeNotRequired();

    size_t sleep = (isTestMode() ? 1 : 14);
    CStringArray commands, filters;
    explode(commands, tool_flags, ' ');
    for (size_t i = 0; i < commands.size() ; i++) {
        if (commands[i] == "--sleep" && i < commands.size() - 1) {
            sleep = str_2_Uint(commands[i+1]);
            i++;
        } else if (commands[i] == "--filter" && i < commands.size() - 1) {
            while (!commands[i+1].empty())
                filters.push_back(nextTokenClear(commands[i+1], ','));
            i++;
        } else if (!builtInCmd(commands[i]))
            EXIT_USAGE("Invalid param " + commands[i]);
    }
    if (filters.empty())
        filters.push_back("0x");

    // Run until we're told not to
    size_t maxRuns = (isTestMode() ? 2 : UINT64_MAX);
    size_t nRuns = 0;
    while (nRuns++ < maxRuns && !shouldQuit()) {

        CStringArray files;
        listFilesInFolder(files, getMonitorPath("*.*"), false);

        CAccountNameArray accounts;
        for (auto file : files) {
            if (contains(file, ".acct.bin") && !contains(file, ".lck")) { // we skip monitors that are already running
                replace(file, getMonitorPath(""), "");
                CAccountName item;
                item.addr = nextTokenClear(file, '.');
                getNamedAccount(item, item.addr);
                item.name = substitute(substitute(item.name, "(", ""), ")", "");
                accounts.push_back(item);
            }

            // We want to be able to run even if there's nothing to monitor
            if (accounts.size() == 0)
                cerr << cTeal << "\tnothing to monitor" << cOff << endl;

            CAddressArray runs;
            for (auto acct : accounts) {
                for (auto filter : filters)
                    if (startsWith(acct.addr, filter))
                        runs.push_back(acct.addr);
            }

            if (runs.size())
                freshen_internal(FM_PRODUCTION, runs, "", freshen_flags);
        }

        if (files.size() == 0)
            LOG_WARN("Nothing to monitor");
        LOG_INFO("Sleeping for ", sleep, " seconds");

        if (!isTestMode())
            usleep((unsigned int)sleep * 1000000);
    }
    EXIT_NOMSG(true);
}
