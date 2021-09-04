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

const char* STR_DELETED = "Monitor [{ADDRESS}] was deleted but not removed";
const char* STR_UNDELETED = "Monitor [{ADDRESS}] was undeleted";
const char* STR_REMOVED = "Monitor [{ADDRESS}] was permentantly removed";

const char* STR_NOTFOUND = "Monitor [{ADDRESS}] not found";
const char* STR_DELETEFIRST = "Monitor [{ADDRESS}] must be deleted first";
const char* STR_ALREADYDELETED = "Monitor [{ADDRESS}] is already deleted";

//------------------------------------------------------------------------------------------------
bool COptions::process_rm(const CAddressArray& addrs) {
    CStringArray results;
    for (auto crudCmd : crudCommands) {
        for (auto addr : addrs) {
            CMonitor monitor;
            monitor.address = addr;
            if (!monitor.monitorExists()) {
                results.push_back(monitor.Format(STR_NOTFOUND));
                LOG_WARN(monitor.Format(STR_NOTFOUND));
            } else {
                if (crudCmd == "remove") {
                    if (monitor.isDeleted()) {
                        if (isTestMode()) {
                            results.push_back(monitor.Format(STR_REMOVED) + " NOT REMOVED BECAUSE TESTING.");
                        } else {
                            monitor.removeMonitor();
                            results.push_back(monitor.Format(STR_REMOVED));
                        }
                    } else {
                        results.push_back(monitor.Format(STR_DELETEFIRST));
                    }
                } else {
                    if (crudCmd == "delete") {
                        if (monitor.isDeleted()) {
                            results.push_back(monitor.Format(STR_ALREADYDELETED));
                        } else {
                            monitor.deleteMonitor();
                            results.push_back(monitor.Format(STR_DELETED));
                        }
                    } else if (crudCmd == "undelete") {
                        if (monitor.isDeleted()) {
                            monitor.undeleteMonitor();
                            results.push_back(monitor.Format(STR_UNDELETED));
                        } else {
                            results.push_back(monitor.Format(STR_DELETEFIRST));
                        }
                    } else {
                        // do nothing for --create and --edit
                    }
                }
                LOG_INFO(results[results.size() - 1]);
            }
        }
    }

    if (isApiMode()) {
        expContext().exportFmt = JSON1;
        cout << exportPreamble("", "");
        string_q msg;
        for (auto result : results) {
            if (!firstOut)
                msg += ",";
            msg += ("\"" + result + "\"");
            firstOut = false;
        }
        if (msg.empty())
            msg = "{ \"msg\": \"nothing was removed\" }";
        cout << msg;
        cout << exportPostamble(errors, "") << endl;
    }

    return false;  // do not continue
}
