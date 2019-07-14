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

    string_q tmpFile = configPath("cache/tmp/scraper-wait.txt");
    bool wait = fileExists(tmpFile);
    if (!scrape_mode.empty()) {
        if (scrape_mode == "start") {
            ::remove(tmpFile.c_str());
            cerr << cYellow << "Starting scraper..." << cOff << endl;
        } else {
            stringToAsciiFile(tmpFile, Now().Format(FMT_EXPORT));
            cerr << cYellow << "Stopping scraper..." << cOff << endl;
        }
        return true;
    }

    size_t sleep = 2;
    CStringArray commands;
    explode(commands, tool_flags, ' ');
    for (size_t i = 0; i < commands.size() ; i++) {
        //COption("-sleep", "number of seconds to sleep between runs"),
        if (commands[i] == "--sleep" && i < commands.size() - 1) {
            sleep = str_2_Uint(commands[i+1]);
            commands[i] = commands[i+1] = "";
            i++;
        } else if (commands[i] == "--wait") {
            wait = true;
            commands[i] = "";
            stringToAsciiFile(tmpFile, "wait");
        }
    }

    // pass through
    tool_flags = "";
    for (auto cmd : commands)
        tool_flags += (cmd + " ");

    // Run until we're told not to
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    size_t nRuns = 0;
    while (nRuns++ < maxRuns && !shouldQuit()) {

        if (wait) {
            static int cnt = 0;
            if (!(++cnt%5))
                cerr << cTeal << "Your scraper is not enabled. Sleeping shortly..." << cOff << endl;
            usleep(((unsigned int)5) * 1000000);

        } else {
            ostringstream os;
            os << "blockScrape --verbose " << tool_flags;
            if (isTestMode())
                cout << os.str() << endl;
            else {
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            }

            if (sleep != 0)
                cerr << Now().Format(FMT_EXPORT) << " - sleeping for " << sleep << " seconds" << endl;
            if (!isTestMode()) {
                usleep((unsigned int)sleep * 1000000);
                if (sleep == 0)
                    usleep(500000); // stay responsive to cntrl+C
            }
        }
        wait = fileExists(tmpFile);
    }
    return true;
}
