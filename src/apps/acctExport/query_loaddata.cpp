/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::loadOneAddress(const CMonitor& monitor, CAppearanceArray_base& arrayOut) {
    ENTER("loadOneAddress");

    string_q path = monitor.getMonitorPath(monitor.address);
    size_t nRecords = (fileSize(path) / sizeof(CAppearance_base));
    ASSERT(nRecords);
    stats.nFileRecords += nRecords;

    CAppearance_base* buffer = new CAppearance_base[nRecords];
    if (buffer) {
        bzero((void*)buffer, nRecords * sizeof(CAppearance_base));  // NOLINT

        CArchive txCache(READING_ARCHIVE);
        if (txCache.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            txCache.Read(buffer, sizeof(CAppearance_base), nRecords);
            txCache.Release();
        } else {
            EXIT_FAIL("Could not open cache file.");
        }

        // Add to the apps which may be non-empty
        arrayOut.reserve(arrayOut.size() + nRecords);
        for (size_t i = first_record; i < min(((blknum_t)nRecords), (first_record + max_records)); i++) {
            if (buffer[i].blk == 0)
                prefundAddrMap[buffer[i].txid] = toLower(monitor.address);
            if (buffer[i].txid == 99997 || buffer[i].txid == 99998 || buffer[i].txid == 99999)
                blkRewardMap[buffer[i].blk] = monitor.address;
            arrayOut.push_back(buffer[i]);
        }

        delete[] buffer;

    } else {
        EXIT_FAIL("Could not allocate memory for address " + monitor.address);
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
            // if (freshen) {
            //     blknum_t lastExport = monitor.getLastEncounter ed();
            //     if (scanRange.first == 0)  // we can start where the last export happened on any address...
            //         scanRange.first = lastExport;
            //     if (lastExport < scanRange.first)  // ...but the eariest of the last exports is where we start
            //         scanRange.first = lastExport;
            // }
        }
    }

    if (tmp.size() == 0) {
        if (!freshenOnly)
            LOG_INFO("Nothing to export" + (allMonitors.size() ? (" from " + accountedFor) : "") + ".");
        return false;
    }

    // Sort the file as it may or may not be sorted already
    sort(tmp.begin(), tmp.end());

    apps.push_back(tmp[0]);
    for (auto app : tmp) {
        CAppearance_base* prev = &apps[apps.size() - 1];
        // TODO(tjayrush): I think this removes dups. Is it really necessary?
        if (app.blk != prev->blk || app.txid != prev->txid) {
            if (app.blk > bp.client) {
                static bool hasFuture = false;
                if (!hasFuture) {
                    if (!isTestMode())
                        LOG_WARN("Cache file contains blocks ahead of the chain. Some apps will not be exported.");
                    hasFuture = true;
                }
            } else {
                apps.push_back(app);
            }
        }
    }
    if (tmp.size() > apps.size()) {
        // there were dups
        // TODO: We should re-write the file here to remove the dups
        // Make sure to preserve lastVisited (but only if later than what already exists)
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
