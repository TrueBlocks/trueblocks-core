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
            if (isRunning("acctScrape") || options.state == STATE_PAUSED) {
                cerr << "Block scraper is paused: " << Now().Format(FMT_EXPORT) << "\r";
                cerr.flush();
            } else {
                if (options.tools & TOOL_INDEX) {
                    cerr << cYellow << "Block scraper is running...";
                    cerr << (options.scrape_blocks() ? "completed..." : "did not complete...");
                }
                if (options.tools & TOOL_MONITORS) {
                    cerr << cYellow << "Monitor scraper is running...";
                    cerr << (options.scrape_monitors() ? "completed..." : "did not complete...");
                }
                cerr << "running again in " << options.sleep << " seconds... " << cOff << endl;
                // FIX_THIS_CODE
                freshenTimestamps(getLatestBlock_cache_ripe());
            }
            // We need to sleep, but we want to wake up frequently enough to check if user has
            // told us to quit, pause or restart. (The `sleep` value is in seconds.)
            uint32_t nHalfSeconds = (uint32_t)(options.sleep * 2);
            ScrapeState prevState = options.state;
            for (size_t n = 0; n < nHalfSeconds && !shouldQuit() && options.state == prevState; n++) {
                usleep((useconds_t)(500000));
                options.state = options.getCurrentState();
                cerr << ".";
                cerr.flush();
            }
        }
    }

    options.cleanup();
    pinlib_cleanup();
    return 0;
}
