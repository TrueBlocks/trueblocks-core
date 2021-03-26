/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::loadOneAddress(const CMonitor& monitor, CAppearanceArray_base& arrayOut) {
    ENTER("loadOneAddress");

    string_q path = monitor.getMonitorPath(monitor.address);
    size_t nRecords = (fileSize(path) / sizeof(CAppearance_base));
    ASSERT(nRecords);
    nTransactions += nRecords;

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
    ENTER("loadAllAppearances");

    if (count)
        return true;

    CAppearanceArray_base tmp;
    for (auto monitor : allMonitors) {
        if (hackAppAddr.empty()) {
            CAccountName acct;
            acct.address = monitor.address;
            getNamedAccount(acct, monitor.address);
            hackAppName = acct.name;
            hackAppAddr = acct.address;
        }
        if (!loadOneAddress(monitor, tmp))
            EXIT_FAIL("Could not load monitor for address " + monitor.address);
        if (freshen) {
            // If we're freshening...
            blknum_t lastExport = monitor.getLastExportedBlock();
            if (exportRange.first == 0)  // we can start where the last export happened on any address...
                exportRange.first = lastExport;
            if (lastExport < exportRange.first)  // ...but the eariest of the last exports is where we start
                exportRange.first = lastExport;
        }
    }

    if (tmp.size() == 0) {
        if (!freshen)
            LOG4("Nothing to export" + (allMonitors.size() ? (" from " + allMonitors[0].address) : "") + ".");
        return false;
    }

    // Sort the file as it may or may not be sorted already
    sort(tmp.begin(), tmp.end());

    apps.push_back(tmp[0]);
    for (auto app : tmp) {
        CAppearance_base* prev = &apps[apps.size() - 1];
        // TODO(tjayrush): I think this removes dups. Is it really necessary?
        if (app.blk != prev->blk || app.txid != prev->txid) {
            if (app.blk > latestBlock) {
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
    nProcessing = apps.size();

    // Make sure the timestamps column is at least as up to date as this monitor
    if (apps.size()) {
        // it's okay to not be able to freshen this. We'll just report less txs
        freshenTimestamps(apps[apps.size() - 1].blk);
        if (!loadTimestamps(&expContext().tsMemMap, expContext().tsCnt))
            EXIT_FAIL("Could not open timestamp file.");
    }

    EXIT_NOMSG(true);
}
