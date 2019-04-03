/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_scrape(void) {

    // scrape mode requires a running node
    nodeRequired();

    size_t sleep = 14;
    CStringArray commands;
    explode(commands, flags, ' ');
    for (size_t i = 0; i < commands.size() ; i++) {
        //COption("-sleep", "number of seconds to sleep between runs"),
        if (commands[i] == "--sleep" && i < commands.size() - 1) {
            sleep = str_2_Uint(commands[i+1]);
            commands[i] = commands[i+1] = "";
            i++;
        }
    }

    // pass through
    flags = "";
    for (auto cmd : commands)
        flags += (cmd + " ");

    // Run until we're told not to
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    size_t nRuns = 0;
    while (nRuns++ < maxRuns && !shouldQuit()) {

        ostringstream os;
        os << "blockScrape " << flags;
        if (isTestMode())
            cout << os.str() << endl;
        else {
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        }

        cerr << "Sleeping for " << sleep << " seconds" << endl;
        if (!isTestMode())
            usleep((unsigned int)sleep * 1000000);
    }
    return true;
}
