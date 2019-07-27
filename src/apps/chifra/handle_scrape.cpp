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

    // syntactic sugar
    tool_flags = substitute(substitute(" "+tool_flags, "--start", " start"), " start", "");
    bool daemonMode = false;

    // The presence of 'waitFile' will either pause of kill the scraper. If 'waitFile'
    // disappears, the scraper will resume
    string_q waitFile = configPath("cache/tmp/scraper-off.txt");
    bool wasPaused = fileExists(waitFile);
    if (wasPaused)
        cerr << cYellow << "The scraper is currently paused" << cOff << endl;

    if (contains(tool_flags, "restart")) {
        //---------------------------------------------------------------------------------
        // If it's not running, we can't restart it
        if (nRunning("chifra scrape") < 2) {
            cerr << cYellow << "Scraper is not running. Cannot restart..." << cOff << endl;
            return true;
        }

        // It's running, so we can restart it (even if it's not currently paused -- won't hurt)
        ::remove(waitFile.c_str());

        // If it's not paused, let the user know
        if (!wasPaused) {
            cerr << cYellow << "Scraper is not paused. Cannot restart..." << cOff << endl;
            return true;
        }

        // We will restart pause the next time we get a chance
        cerr << cYellow << "Scraper will restart shortly..." << cOff << endl;
        return true;

    } else if (contains(tool_flags, "pause")) {
        //---------------------------------------------------------------------------------
        // If it's not running, we can't pause it
        if (nRunning("chifra scrape") < 2) {
            cerr << cYellow << "Scraper is not running. Cannot pause..." << cOff << endl;
            return true;
        }

        // It's running, so we can pause it (even if it's already paused -- won't hurt)
        stringToAsciiFile(waitFile, Now().Format(FMT_EXPORT));

        // If it's paused already, let the user know
        if (wasPaused) {
            cerr << cYellow << "Scraper is aleardy paused..." << cOff << endl;
            return true;
        }

        // We will pause the next time we get a chance
        cerr << cYellow << "Scraper will pause shortly..." << cOff << endl;
        return true;

    } else if (contains(tool_flags, "quit")) {
        //---------------------------------------------------------------------------------
        // If it's not running, we can't kill it
        if (nRunning("chifra scrape") < 2) {
            cerr << cYellow << "Scraper is not running. Cannot quit..." << cOff << endl;
            return true;
        }

        // Kill it whether it's currently paused or not
        stringToAsciiFile(waitFile, "quit");

        // We will pause the next time we get a chance
        cerr << cYellow << "Scraper will quit shortly..." << cOff << endl;
        return true;

    } else {
        //---------------------------------------------------------------------------------
        // Look for options that we don't pass on to blockScrape...
        CStringArray optList;
        explode(optList, tool_flags, ' ');
        tool_flags = "";
        for (auto opt : optList) {
            if (opt == "--daemon") {
                opt = "";
                daemonMode = true;

            } else if (!opt.empty() && !startsWith(opt, "-") && !isNumeral(opt)) {
                cerr << "Invalid command " << cYellow << opt << cOff << " to chifra scrape." << endl;
                return false;
            }
            if (!opt.empty())
                tool_flags += (opt + " ");
        }

        if (nRunning("chifra scrape") > 1) {
            cerr << "'chifra scrape' is already running." << endl;
            return false;
        }

        cerr << cYellow << "Scraper is starting with " << tool_flags << "..." << cOff << endl;
    }

    // Run forever...unless told to pause or stop (shouldQuit is true if control+C was hit.
    bool waitFileExists = fileExists(waitFile);
    size_t nRuns = 0;
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    while (nRuns++ < maxRuns && !shouldQuit()) {

        if (waitFileExists) {
            if (!wasPaused)
                cerr << cYellow << "\tScraper paused..." << cOff << endl;
            wasPaused = true;
            usleep(max(useconds_t(5), scrapeSleep) * 1000000); // sleep for at least five seconds

        } else {
            if (wasPaused)
                cerr << cYellow << "\tScraper restarted..." << cOff << endl;
            wasPaused = false;
            ostringstream os;
            os << "blockScrape " << tool_flags;
            if (isTestMode())
                cout << os.str() << endl;
            else {
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            }

            if (daemonMode) {
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
                        freshen_internal(FM_PRODUCTION, runs, "--daemon", freshen_flags);
                }
            }

            if (!isTestMode())
                usleep(scrapeSleep == 0 ? 500000 : scrapeSleep * 1000000); // stay responsive to cntrl+C
        }

        waitFileExists = fileExists(waitFile);
        if (waitFileExists) {
            if (asciiFileToString(waitFile) == "quit") {
                ::remove(waitFile.c_str());
                cerr << cYellow << "\tScraper quitting..." << cOff << endl;
                return true;
            }
        }
    }

    return true;
}
