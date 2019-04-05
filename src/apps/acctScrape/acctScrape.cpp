/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (!options.useBlooms)
            options.scrapeCnt = 10000000;  // TODO(tjayrush): Not right

        if (options.useBlooms)
            forEveryBloomFile(visitBloomFilters, &options, options.startScrape, options.scrapeCnt);
        else {
            string_q path = (options.new_version ? indexFolder_binary_v2 : indexFolder_sorted_v2);
            forEveryFileInFolder(path, visitIndexFiles, &options);
        }

    }
    options.finalReport();
    acctlib_cleanup();

    return 0;
}
