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

//-----------------------------------------------------------------------
bool visitOnLoad(CAppearance_mon& app, void* data) {
    COptions* opt = (COptions*)data;
    ASSERT(opt->curMonitor);
    if (app.blk == 0)
        opt->prefundAddrMap[app.txid] = opt->curMonitor->address;
    if (app.txid == 99996 || app.txid == 99997 || app.txid == 99998 || app.txid == 99999)
        opt->blkRewardMap[app.blk] = opt->curMonitor->address;
    opt->stats.nFileRecords++;
    return true;
}

//-----------------------------------------------------------------------
bool COptions::loadMonitors(void) {
    if (count)
        return true;

    if (unripe) {
        forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, this);

    } else if (staging) {
        // TODO: BOGUS - scrape to front of chain
        LOG_INFO("Staging option is currently not available.");
        // forEveryFileInFolder(indexFolder_staging, visitToFreshen_fromStaging, this);

    } else {
        for (CMonitor& monitor : allMonitors) {
            curMonitor = &monitor;
            if (!monitor.loadAppearances(visitOnLoad, this)) {
                LOG_ERR("Could not load appearances for address " + monitor.address);
                return false;
            }
            string_q path = monitor.getPathToMonitor(monitor.address, false);
            if (monitor.getRecordCnt(path) == 0) {
                // We don't continue if we have no transactions. We used to report an
                // error here, but this is not really an error
                if (!freshen)
                    LOG_WARN("No records found for address ", monitor.address);
                return false;
            }
        }
    }

    bool reloaded = false;
    CAppearanceArray_mon monTmp;
    for (CMonitor& mon : allMonitors) {
        for (const CAppearance_mon& app : mon.apps) {
            if (app.blk == 0) {
                if (!reloaded) {
                    clearNames();
                    clearPrefundBals();
                    loadNamesWithPrefunds();
                    reloaded = true;
                }
                findName(mon.address, accountedFor);
                accountedFor.isContract = !getCodeAt(mon.address, meta.client).empty();
            }
            monTmp.push_back(app);
        }
    }

    // Sort the file as it may or may not be sorted already
    if (reversed)  // TODO(tjayrush): remove this comment once account works backwardly
        sort(monTmp.begin(), monTmp.end(), sortMonitoredAppearanceReverse);

    // This limits the records to only those in the user's specified range, note we may bail early
    // if the blockchain itself isn't caught up to the most recent block in the monitor which may
    // happen if we re-sync the index (and optionally the node) from scratch
    for (size_t i = first_record; i < min(blknum_t(monTmp.size()), (first_record + max_records)); i++) {
        CAppearance_mon* app = &monTmp[i];
        if (app->blk > meta.client) {
            static bool hasFuture = false;
            if (!hasFuture) {
                if (!isTestMode())
                    LOG_WARN("Cache file contains blocks ahead of the chain. Some apps will not be exported.");
                hasFuture = true;
            }
        } else {
            monApps.push_back(*app);
        }
    }

    // Make sure the timestamps column is at least as up to date as this monitor
    if (monApps.size()) {
        // it's okay to not be able to freshen this. We'll just report less txs
        if (!getTimestampAt(1))  // loads the timestamp file and returns non-zero
            return false;

    } else {
        if (!freshen)
            LOG_INFO("Nothing to export" + (allMonitors.size() ? (" from " + accountedFor.address) : "") + ".");
        return false;
    }

    return true;
}
