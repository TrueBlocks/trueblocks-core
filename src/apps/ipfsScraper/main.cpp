/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (!options.handle_scrape())
            return 0;
    }

    cerr << handle_reporting();
    // FIX_THIS_CODE
    freshenTimestamps(getLatestBlock_cache_ripe());

    acctlib_cleanup();

    return 0;
}
