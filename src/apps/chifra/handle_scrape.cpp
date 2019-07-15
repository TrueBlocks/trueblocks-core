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

    string_q waitFile = configPath("cache/tmp/scraper-off.txt");
    if (contains(tool_flags, "restart")) {
        ::remove(waitFile.c_str());
        if (nRunning("chifra scrape") < 2) {
            cerr << cYellow << "Scraper is not running. Cannot restart..." << cOff << endl;
            return true;
        }
        cerr << cYellow << "Scraper is restarting..." << cOff << endl;
        return true;
    } else if (contains(tool_flags, "pause")) {
        stringToAsciiFile(waitFile, Now().Format(FMT_EXPORT));
        cerr << cYellow << "Scraper is pausing..." << cOff << endl;
        return true;
    } else if (contains(tool_flags, "quit")) {
        stringToAsciiFile(waitFile, "quit");
        cerr << cYellow << "Scraper is quitting..." << cOff << endl;
        return true;
    }

    if (nRunning("chifra scrape") > 1) {
        cerr << "'chifra scrape' is already running. Quitting..." << endl;
        return false;
    }

    useconds_t sleep = 0;
    CStringArray commands;
    explode(commands, tool_flags, ' ');
    for (size_t i = 0; i < commands.size() ; i++) {
        if (commands[i] == "--sleep" && i < commands.size() - 1) {
            sleep = (useconds_t)str_2_Uint(commands[i+1]);
            commands[i] = commands[i+1] = "";
            i++;
        } else if (commands[i] == "--start") {
            commands[i] = "";
            i++;
        }
    }

    // pass through
    tool_flags = "";
    for (auto cmd : commands)
        tool_flags += (cmd + " ");

    // The waitFile is used to pause and re-engage the scraper in docker mode
    bool waitFileExists = fileExists(waitFile);
    if (waitFileExists)
        cerr << cYellow << "\tThe scraper is paused" << cOff << endl;
    bool paused = waitFileExists;

    // Run forever...
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    size_t nRuns = 0;
    while (nRuns++ < maxRuns && !shouldQuit()) {

        if (waitFileExists) {
            if (!paused)
                cerr << cYellow << "\tScraper paused..." << cOff << endl;
            paused = true;
            usleep(max(useconds_t(5), sleep) * 1000000); // sleep for five seconds

        } else {
            if (paused)
                cerr << cYellow << "\tScraper restarted..." << cOff << endl;
            paused = false;
            ostringstream os;
            os << "blockScrape " << tool_flags;
            if (isTestMode())
                cout << os.str() << endl;
            else {
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            }

            if (!isTestMode())
                usleep(sleep == 0 ? 500000 : sleep * 1000000); // stay responsive to cntrl+C
        }

        waitFileExists = fileExists(waitFile);
        if (asciiFileToString(waitFile) == "quit") {
            ::remove(waitFile.c_str());
            cerr << cYellow << "\tScraper killed..." << cOff << endl;
            return true;
        }
    }
    return true;
}
