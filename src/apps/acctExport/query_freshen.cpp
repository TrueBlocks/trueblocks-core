/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::process_freshen(void) {
    // Clean the monitor stage of previously unfinished scrapes
    cleanMonitorStage();

    // TODO(tjayrush): Reconisider whether to move scrapes of staging to production here or
    // TODO(tjayrush): wait until the stage consolidates. By not moving it here, we get repeated scans through
    // TODO(tjayrush): the stage on blocks that we've already processed for a given monitor even though we
    // TODO(tjayrush): know that once a block goes on the stage, we won't be rescraping it.
    if (unripe || staging) {
        // Note that we used to think we needed to process the unripe here, but since
        // we changed the meaning of --unripe from "add additional unripe record to the
        // exported data" to "export only unripe data", we no longer do this here. We
        // accumulate the unripe data only in the loadAllAppearances routine. In this
        // way, the caller is responsible to maintain knowledge about the provided export.
        // Much simpler for the backend.

    } else {
        forEveryFileInFolder(indexFolder_blooms, visitFinalIndexFiles, this);
        for (auto monitor : allMonitors) {
            monitor.moveToProduction();
            LOG4(monitor.address, " freshened to ", monitor.getLastVisited(true /* fresh */));
        }
    }

    return true;
}
