/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.shouldScrape()) {

            if (options.txCache.Lock(options.cacheFilename, modeWriteAppend, LOCK_WAIT)) {

                if (options.useIndex)
                    forEveryFileInFolder(indexFolder_prod, visitIndexFiles, &options);
                else
                    forEveryBloomFile(visitBloomFilters, &options, options.firstBlock, options.nBlocks);

                options.txCache.Release();

            } else {
                return options.usage("Cannot open transaction cache '" + options.cacheFilename + "'. Quitting...");
            }
        }
    }

    options.finalReport();

    acctlib_cleanup();
    return 0;
}
