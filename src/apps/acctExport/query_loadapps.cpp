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
    for (CMonitor& monitor : allMonitors) {
        curMonitor = &monitor;
        if (!monitor.readAppearances(visitOnLoad, this)) {
            LOG_ERR("Could not load appearances for address " + monitor.address);
            return false;
        }
        string_q path = monitor.getPathToMonitor(monitor.address, false);
        if (monitor.getRecordCnt(path) == 0) {
            // We don't continue if we have no transactions. We used to report an
            // error here, but this is not really an error
            LOG_WARN("No records found for address ", monitor.address);
            return false;
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
                findName(mon.address, ledgerManager.name);
                ledgerManager.name.isContract = !getCodeAt(mon.address, meta.client).empty();
            }
            monTmp.push_back(app);
        }
    }

    // Sort the file as it may or may not be sorted already
    if (reversed) {  // TODO(tjayrush): remove this comment once account works backwardly
        sort(monTmp.begin(), monTmp.end(), sortMonitoredAppearanceReverse);
    } else if (neighbors) {
        sort(monTmp.begin(), monTmp.end(), sortMonitoredAppearances);
    }

    for (size_t i = 0; i < monTmp.size(); i++) {
        CAppearance_mon* app = &monTmp[i];
        if (app->blk < exportRange.first || app->blk > exportRange.second) {
            // do nothing
        } else {
            if ((i + 1) >= first_record && (max_records == 250 || ledgerManager.appArray.size() < max_records)) {
                if (app->blk > meta.client) {
                    static bool hasFuture = false;
                    if (!hasFuture) {
                        if (!isTestMode())
                            LOG_WARN("Cache file contains blocks ahead of the chain. Some apps will not be exported.");
                        hasFuture = true;
                    }
                } else {
                    ledgerManager.appArray.push_back(*app);
                }
            }
        }
    }

    // Make sure the timestamps column is at least as up to date as this monitor
    if (ledgerManager.appArray.size()) {
        // it's okay to not be able to freshen this. We'll just report less txs
        return bn_2_Timestamp(1);  // loads the timestamp file and returns non-zero
    }

    LOG_INFO("Nothing to export" + (allMonitors.size() ? (" from " + ledgerManager.accountedFor) : "") + ".");
    return allMonitors.size() > 0;
}
