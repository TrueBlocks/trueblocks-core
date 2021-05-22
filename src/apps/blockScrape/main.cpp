/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    pinlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    ASSERT(options.commandLines.size() > 0);  // no support for --file: option
    // for (auto command : options.commandLines) {
    if (!options.parseArguments(options.commandLines[0]))
        return 0;

    // We keep track of what state we're in (paused, running, etc.)...
    string_q unused;
    options.state = options.getCurrentState(unused);

    // ...and run forever (until user tells us to quit)...
    while (options.state != STATE_STOPPED && !shouldQuit()) {
        if (isRunning("acctExport") || options.state == STATE_PAUSED) {
            // User may have started the account scraper since we started. Here, we
            // go to sleep for a short while to allow that program to complete...
            LOG_INFO("Block scraper is paused: ", Now().Format(FMT_EXPORT), "\r");

        } else {
            bool ret1 = true;
            if (options.tools & TOOL_INDEX) {
                LOG_INFO(cYellow, "Block scraper is running...", cOff);
                ret1 = options.scrape_blocks();
            }

            bool ret2 = true;
            if (options.tools & TOOL_MONITORS) {
                LOG_INFO(cYellow, "Monitor scraper is running...", cOff);
                ret2 = options.scrape_monitors();
                LOG_INFO(cYellow, "   finished...", cOff);
            }

            LOG_INFO(ret1 && ret2 ? "  ...pass completed." : "  ...pass did not complete.");

            // TODO(tjayrush): We should try to scrape timestamps with blaze while we're doing this scan
            // TODO(tjayrush): try to capture timestamps during blaze scraping
            freshenTimestamps(getBlockProgress(BP_RIPE).ripe);
        }

        // We need to sleep, but we want to wake up frequently enough to check to see if user has
        // told hit the control-C or sent pause, quit or restart. (The `sleep` value is in seconds.)
        if (!isLiveTest()) {  // don't sleep if we're in live testing
            uint32_t nHalfSeconds = (uint32_t)(options.sleep * 2);
            ScrapeState prevState = options.state;
            for (size_t n = 0; n < nHalfSeconds && !shouldQuit() && options.state == prevState; n++) {
                usleep((useconds_t)(500000));
                options.state = options.getCurrentState(unused);
                if (!(n % 4))
                    LOG_INFO("Running again in ", (options.sleep - (n * .5)), ".0 seconds...\r");
            }
            LOG_INFO("Running again in 0 seconds...    ");
        } else {
            options.state = options.getCurrentState(unused);
        }
    }
    // }

    // clean up the control file...
    options.cleanupAndQuit();

    pinlib_cleanup();
    return 0;
}
