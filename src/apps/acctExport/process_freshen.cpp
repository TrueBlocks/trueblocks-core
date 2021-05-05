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

    if (visitTypes & VIS_FINAL)
        forEveryFileInFolder(indexFolder_blooms, visitFinalIndexFiles, this);

    if (visitTypes & VIS_STAGING)
        forEveryFileInFolder(indexFolder_staging, visitStagingIndexFiles, this);

    if (visitTypes & VIS_UNRIPE)
        forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, this);

    for (auto monitor : allMonitors) {
        monitor.moveToProduction();
        LOG4(monitor.address, " freshened to ", monitor.getLastVisited(true /* fresh */));
    }

    return true;
}
