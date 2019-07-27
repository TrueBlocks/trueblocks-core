/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_daemon(void) {

    // address scraping does not require a running node
    nodeNotRequired();

    size_t nRuns = 0;
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    while (nRuns++ < maxRuns && !shouldQuit()) {
        // Keep this inside the loop so newly created monitors get automatically picked up
        CStringArray files;
        listFilesInFolder(files, getMonitorPath("*.acct.bin"), false);
        CAddressArray runs;
        if (files.size()) {
            for (auto file : files) {
                file = substitute(substitute(file, getMonitorPath(""), ""), ".acct.bin", "");
                if (isAddress(file))
                    runs.push_back(file);
            }
            if (runs.size())
                freshen_internal(FM_PRODUCTION, runs, "", freshen_flags);
            else
                cerr << "Nothing to monitor." << endl;
        } else {
            cerr << "Nothing to monitor." << endl;
        }
        if (!isTestMode())
            usleep(scrapeSleep == 0 ? 500000 : scrapeSleep * 1000000); // stay responsive to cntrl+C
    }

    return true;
}
