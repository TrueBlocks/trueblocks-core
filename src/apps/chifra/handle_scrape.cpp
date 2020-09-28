/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern bool visitMonitor(const string_q& path, void* data);
extern bool isScraperRunning(const string_q& unsearch);
//------------------------------------------------------------------------------------------------
bool COptions::handle_scrape(void) {
    ENTER("handle_" + mode);

    // scrape mode requires a running node
    nodeRequired();

    if (contains(tool_flags, "help")) {
        ostringstream os;
        os << "blockScrape --help";
        LOG_CALL(os.str());
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        EXIT_NOMSG(true);
    }

    // syntactic sugar
    tool_flags = substitute(substitute(" " + tool_flags, "--start", " start"), " start", "");
    bool daemonMode = false;

    // The presence of 'waitFile' will either pause or kill the scraper. If 'waitFile'
    // disappears and the scraper is paused, the scraper will resume
    string_q waitFile = configPath("cache/tmp/scraper-off.txt");
    bool wasPaused = fileExists(waitFile);
    if (wasPaused)
        LOG_INFO("The scraper is currently paused");

    if (contains(tool_flags, "restart")) {
        //---------------------------------------------------------------------------------
        // If a seperate instance is not running, we can't restart it
        if (!isScraperRunning("restart")) {
            LOG_WARN("Scraper is not running. Cannot restart...");
            EXIT_NOMSG(true);
        }

        // If it's not paused, let the user know...
        if (!wasPaused) {
            LOG_WARN("Scraper is not paused. Cannot restart...");
            EXIT_NOMSG(true);
        }

        // A seperate instance is running, removing the file will restart the scraper
        ::remove(waitFile.c_str());
        LOG_INFO("Scraper will restart shortly...");
        EXIT_NOMSG(true);

    } else if (contains(tool_flags, "pause")) {
        //---------------------------------------------------------------------------------
        // If a seperate instance is not running, we can't pause it
        if (!isScraperRunning("pause")) {
            LOG_WARN("Scraper is not running. Cannot pause...");
            EXIT_NOMSG(true);
        }

        // If it's already paused, let the user know...
        if (wasPaused) {
            LOG_WARN("Scraper is already paused...");
            EXIT_NOMSG(true);
        }

        // It's running, so we can pause it
        stringToAsciiFile(waitFile, Now().Format(FMT_EXPORT));
        LOG_INFO("Scraper will pause shortly...");
        EXIT_NOMSG(true);

    } else if (contains(tool_flags, "quit")) {
        if (!isScraperRunning("quit")) {
            LOG_WARN("Scraper is not running. Cannot quit...");
            EXIT_NOMSG(true);
        }

        // Kill it whether it's currently paused or not
        stringToAsciiFile(waitFile, "quit");
        LOG_INFO("Scraper will quit shortly...");
        EXIT_NOMSG(true);

    } else {
        //---------------------------------------------------------------------------------
        // If it's already running, don't start it again...
        if (isScraperRunning("unused")) {
            LOG_WARN("Scraper is already running. Cannot start it again...");
            EXIT_NOMSG(false);
        }

        // Extract options from the command line that we do not pass on to blockScrape...
        CStringArray optList;
        explode(optList, tool_flags, ' ');
        tool_flags = "";  // reset tool_flag
        for (auto opt : optList) {
            if (opt == "--daemon") {
                opt = "";
                daemonMode = true;

            } else if (!opt.empty() && !startsWith(opt, "-") && !isNumeral(opt)) {
                cerr << "Invalid command " << cYellow << opt << cOff << " to chifra scrape." << endl;
                EXIT_NOMSG(false);
            }
            if (!opt.empty())
                tool_flags += (opt + " ");
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
            usleep(max(useconds_t(5), scrapeSleep) * 1000000);  // sleep for at least five seconds

        } else {
            if (wasPaused)
                cerr << cYellow << "\tScraper restarted..." << cOff << endl;
            wasPaused = false;
            ostringstream os;
            os << "blockScrape " << tool_flags;
            LOG_CALL(os.str());
            // clang-format off
            if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
            // clang-format on

            if (isTestMode()) {
                // Do nothing related in --daemon mode while testing

            } else {
                if (daemonMode) {
                    // Catch the monitors addresses up to the scraper if in --deamon mode
                    CMonitorArray monitors;
                    forEveryFileInFolder(getMonitorPath("") + "*", visitMonitor, &monitors);

                    freshen_internal(FM_PRODUCTION, monitors, "", freshen_flags);
                    for (auto monitor : monitors) {
                        if (monitor.cntBefore != monitor.cntAfter) {
                            ostringstream os1;
                            os1 << "acctExport " << monitor.address << " --freshen";  // << " >/dev/null";
                            LOG_INFO("Calling: ", os1.str(), string_q(40, ' '));
                            // clang-format off
                            if (system(os1.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
                            // clang-format on
                            usleep(250000);  // stay responsive to cntrl+C
                            if (shouldQuit())
                                continue;
                        }
                    }
                    cerr << "\t  freshening: " << cYellow << "    finished." << string_q(50, ' ') << cOff
                         << "                                                           " << endl;
                }
                usleep(scrapeSleep == 0 ? 500000 : scrapeSleep * 1000000);  // stay responsive to cntrl+C
            }
        }

        waitFileExists = fileExists(waitFile);
        if (waitFileExists) {
            if (asciiFileToString(waitFile) == "quit") {
                ::remove(waitFile.c_str());
                cerr << cYellow << "\tScraper quitting..." << cOff << endl;
                EXIT_NOMSG(true);
            }
        }
    }
    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------
bool visitMonitor(const string_q& path, void* data) {
    if (!endsWith(path, ".acct.bin"))  // we only want to process monitor files
        return true;

    CMonitor m;
    m.address = substitute(substitute(path, getMonitorPath(""), ""), ".acct.bin", "");
    if (isAddress(m.address)) {
        m.cntBefore = m.cntAfter = m.getRecordCount();
        CMonitorArray* array = (CMonitorArray*)data;  // NOLINT
        array->push_back(m);
    }

    return true;
}

//----------------------------------------------------------------------------
bool isScraperRunning(const string_q& unsearch) {
    string_q pList = listProcesses("chifra scrape");
    replace(pList, "  ", " ");
    replace(pList, "chifra scrape " + unsearch, "");
    return contains(pList, "chifra scrape");
}
