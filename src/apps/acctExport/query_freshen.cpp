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
