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

// #define LOG_HERE LOG_INFO
#define LOG_HERE(...)
//------------------------------------------------------------------------------------------------
bool COptions::process_freshen(void) {
    // Clean the monitor stage of previously unfinished scrapes
    cleanMonitorStage();

    if (unripe) {
        // Note We used to think we needed to process the unripe here, but since
        // we changed the meaning of --unripe from "add additional unripe record to the
        // exported data" to "export only unripe data", we no longer do this here. We
        // accumulate the unripe data only in the loadMonitors routine. In this
        // way, the caller is responsible to maintain knowledge about the provided export.
        // Much simpler for the backend.

    } else if (staging) {
        // TODO: BOGUS - scrape to front of chain
        // TODO(tjayrush): Reconisider whether to move scrapes of staging to production here or
        // TODO(tjayrush): wait until the stage consolidates. By not moving it here, we get
        // TODO(tjayrush): repeated scans through the stage on blocks that we've already
        // TODO(tjayrush): processed for a given monitor even though we know that once a
        // TODO(tjayrush): block goes on the stage, we won't be rescraping it.

    } else {
        LOG_HERE(bYellow, "exportRange        ", (exportRange.second - exportRange.first), " ", exportRange.first, "-",
                 exportRange.second, cOff);
        LOG_HERE(bYellow, "needRange          ", (needRange.second - needRange.first), " ", needRange.first, "-",
                 needRange.second, cOff);
        LOG_HERE(bYellow, "metaData.finalized ", meta.finalized, " ", (meta.finalized - needRange.second), cOff);
        LOG_HERE(bYellow, "metaData.staging   ", meta.staging, " ", (meta.staging - meta.finalized), cOff);
        LOG_HERE(bYellow, "metaData.ripe      ", meta.ripe, " ", (meta.ripe - meta.staging), cOff);
        LOG_HERE(bYellow, "metaData.unripe    ", meta.unripe, " ", (meta.unripe - meta.ripe), cOff);
        LOG_HERE(bYellow, "metaData.client    ", meta.client, " ", (meta.client - meta.unripe), cOff);

        if ((needRange.second - needRange.first) > 0) {
            // TODO: BOGUS - scrape to front of chain
            forEveryFileInFolder(indexFolder_blooms, visitToFreshen_fromFinalized, this);
        }
        for (auto monitor : allMonitors) {
            monitor.moveToProduction(monitor.isStaging);
            LOG4(monitor.address, " freshened to ", monitor.getNextBlockToVisit(false));
        }
    }

    return true;
}
