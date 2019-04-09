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

        if (options.visit & VIS_FINAL)
            forEveryFileInFolder(indexFolder_finalized_v2, visitFinalIndexFiles, &options);

        if (options.visit & VIS_STAGING)
            forEveryFileInFolder(indexFolder_staging_v2, visitStagingIndexFiles, &options);

        if (options.visit & VIS_PENDING)
            forEveryFileInFolder(indexFolder_pending_v2, visitPendingIndexFiles, &options);
    }

    acctlib_cleanup();
    return 0;
}
