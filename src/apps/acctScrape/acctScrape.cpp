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

        if (options.monitors[0].txCache == NULL)
            options.monitors[0].txCache = new CArchive(WRITING_ARCHIVE);

        if (options.monitors[0].txCache &&
            options.monitors[0].txCache->Lock(getTransCachePath(options.monitors[0].address), modeWriteAppend, LOCK_WAIT)) {

            if (options.useIndex)
                forEveryFileInFolder(indexFolder_prod, visitIndexFiles, &options);
            else
                forEveryBloomFile(visitBloomFilters, &options, options.startScrape, options.scrapeCnt);

            options.monitors[0].txCache->Release();

        } else {
            return options.usage("Cannot open transaction cache '" + getTransCachePath(options.monitors[0].address) + "'. Quitting...");
        }
    }

    options.finalReport();

    acctlib_cleanup();
    return 0;
}
