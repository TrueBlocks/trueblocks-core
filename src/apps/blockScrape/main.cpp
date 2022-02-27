/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
        bool ret1 = options.scrape_blocks();
        LOG_INFO(ret1 ? "  ...pass completed." : "  ...pass did not complete.");
    }

    pinlib_cleanup();
    return 0;
}
