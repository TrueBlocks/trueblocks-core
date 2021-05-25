/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
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
