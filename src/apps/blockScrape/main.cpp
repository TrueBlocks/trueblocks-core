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

    if (isRunning("acctExport")) {
        // User may have started the account scraper since we started. Here, we
        // go to sleep for a short while to allow that program to complete...
        LOG_INFO("Block scraper is paused: ", Now().Format(FMT_EXPORT), "\r");

    } else {
        LOG_INFO(cYellow, "Block scraper is running...", cOff);
        bool ret1 = options.scrape_blocks();
        LOG_INFO(ret1 ? "  ...pass completed." : "  ...pass did not complete.");

        // TODO(tjayrush): We should try to scrape timestamps with blaze while we're doing this scan
        // TODO(tjayrush): try to capture timestamps during blaze scraping
        freshenTimestamps(getBlockProgress(BP_RIPE).ripe);
    }

    pinlib_cleanup();
    return 0;
}
