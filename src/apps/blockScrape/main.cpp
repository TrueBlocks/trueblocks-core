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

        // We keep track of what state we're in...
        options.state = options.getCurrentState();

        // ...and run forever (until user tells us to quit)...
        while (options.state != STATE_STOPPED && !shouldQuit()) {
            if (isRunning("acctExport") || options.state == STATE_PAUSED) {
                // User may have started the account scraper since we started. Here, we
                // go to sleep for a short while to allow that program to complete...
                cerr << "Block scraper is paused: " << Now().Format(FMT_EXPORT) << "\r";
                cerr.flush();

            } else {
                // User can tell us to process the index...
                if (options.tools & TOOL_INDEX) {
                    cerr << cYellow << "Block scraper is running...";
                    cerr << (options.scrape_blocks() ? "completed..." : "did not complete...");
                }

                // ...or the monitors (or both).
                if (options.tools & TOOL_MONITORS) {
                    cerr << cYellow << "Monitor scraper is running...";
                    cerr << (options.scrape_monitors() ? "completed..." : "did not complete...");
                }

                cerr << "running again in " << options.sleep << " seconds... " << cOff << endl;

                // FIX_THIS_CODE
                freshenTimestamps(getLatestBlock_cache_ripe());
            }

            // We need to sleep, but we want to wake up frequently enough to check to see if user has
            // told hit the control-C or sent pause, quit or restart. (The `sleep` value is in seconds.)
            uint32_t nHalfSeconds = (uint32_t)(options.sleep * 2);
            ScrapeState prevState = options.state;
            for (size_t n = 0; n < nHalfSeconds && !shouldQuit() && options.state == prevState; n++) {
                usleep((useconds_t)(500000));
                options.state = options.getCurrentState();
            }
        }
    }

    // clean up the control file...
    options.cleanupAndQuit();

    pinlib_cleanup();
    return 0;
}
