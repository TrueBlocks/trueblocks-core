/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::loadOneAddress(const CMonitor& monitor, CMonitoredAppearanceArray& arrayOut) {
    ENTER("loadOneAddress");

    string_q path = monitor.getMonitorPath(monitor.address, FM_PRODUCTION);
    if (!monitor.loadAppearances(path, arrayOut))
        EXIT_FAIL("Could not load appearances for address " + monitor.address);
    stats.nFileRecords += monitor.nRecords();

    for (const CMonitoredAppearance& app : arrayOut) {
        if (app.blk == 0)
            prefundAddrMap[app.txid] = toLower(monitor.address);
        if (app.txid == 99997 || app.txid == 99998 || app.txid == 99999)
            blkRewardMap[app.blk] = monitor.address;
    }

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
bool COptions::loadAllAppearances(void) {
    if (count)
        return true;

    if (unripe) {
        forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, this);

    } else if (staging) {
        forEveryFileInFolder(indexFolder_staging, visitStagingIndexFiles, this);

    } else {
        for (auto monitor : allMonitors) {
            if (!loadOneAddress(monitor, tmp)) {
                LOG_ERR("Could not load monitor for address " + monitor.address);
                return false;
            }

            // TODO(tjayrush): This used to set last encountered block (search for writeLastEncountered)
            // if (freshenOnly) {
            //     blknum_t l astExport = monitor.getLastEncounter ed();
            //     if (blockRange.first == 0)  // we can start where the last export happened on any address...
            //         blockRange.first = l astExport;
            //     if (l astExport < blockRange.first)  // ...but the eariest of the last exports is where we start
            //         blockRange.first = l astExport;
            // }
        }
    }

    if (tmp.size() == 0) {
        if (!freshenOnly)
            LOG_INFO("Nothing to export" + (allMonitors.size() ? (" from " + accountedFor) : "") + ".");
        return false;
    }

    // Sort the file as it may or may not be sorted already
    if (reversed)  // TODO(tjayrush): remove this once account works backwardly
        sort(tmp.begin(), tmp.end(), sortMonitoredAppearanceReverse);

    // This limits the records to only those in the user's specified range, note we may bail early
    // if the blockchain itself isn't caught up to the most recent block in the monitor which may
    // happen if we re-sync the index (and optionally the node) from scratch
    for (size_t i = first_record; i < min(blknum_t(tmp.size()), (first_record + max_records)); i++) {
        CMonitoredAppearance* app = &tmp[i];
        if (app->blk > bp.client) {
            static bool hasFuture = false;
            if (!hasFuture) {
                if (!isTestMode())
                    LOG_WARN("Cache file contains blocks ahead of the chain. Some apps will not be exported.");
                hasFuture = true;
            }
        } else {
            apps.push_back(*app);
        }
    }

    // Make sure the timestamps column is at least as up to date as this monitor
    if (apps.size()) {
        // it's okay to not be able to freshen this. We'll just report less txs
        freshenTimestamps(apps[apps.size() - 1].blk);
        if (!loadTimestamps(&expContext().tsMemMap, expContext().tsCnt)) {
            LOG_ERR("Could not open timestamp file.");
            return false;
        }
    }

    return true;
}
