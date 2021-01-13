/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    pinlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.state = options.getCurrentState();
        while (options.state != STATE_STOPPED && !shouldQuit()) {
            usleep(1000000);
            if (isRunning("acctScrape")) {
                cout << "Not running because of acctScrape: " << Now().Format(FMT_EXPORT) << "\r";
                cout.flush();
            } else if (options.state == STATE_PAUSED) {
                cout << "Paused: " << Now().Format(FMT_EXPORT) << "\r";
                cout.flush();
            } else {
                cerr << cYellow << "Scraper is starting..." << cOff << endl;
                if (!options.scrape_once()) {
                    options.mode[0] = "quit";
                    options.changeState();
                    pinlib_cleanup();
                    return 0;
                }
            }
            options.state = options.getCurrentState();
        }
    }

    options.mode[0] = "quit";
    options.changeState();
    pinlib_cleanup();
    return 0;
}
