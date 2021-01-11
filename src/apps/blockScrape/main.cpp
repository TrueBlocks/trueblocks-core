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

    // for (auto command : options.commandLines)
    {
        if (!options.parseArguments(options.commandLines[0]))
            return 0;
        options.start_scraper();
    }

    pinlib_cleanup();

    return 0;
}
