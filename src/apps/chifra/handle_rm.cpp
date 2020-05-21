/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_rm(void) {
    ENTER("handle_" + mode);
    nodeNotRequired();

    if (contains(tool_flags, "help")) {
        optionOn(OPT_HELP);
        return usage();
    }

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    CStringArray removed;
    bool hasYes = editCmd == "remove";  // contains(tool_flags, "--yes");
    for (auto addr : addrs) {
        CMonitor monitor;
        monitor.address = addr;

        if (!monitor.exists()) {
            if (!hasYes)
                LOG_WARN("Monitor not found for address " + addr + ".");

        } else {
            int ch = 'n';  // default to no in both command line and api cases
            if (!hasYes) {
                if (isApiMode()) {
                    if (monitor.isDeleted())
                        monitor.undeleteMonitor();
                    else
                        monitor.deleteMonitor();
                    return true;
                } else {
                    cerr << "Remove monitor for " << addr << "? (y=yes) >";
                    cerr.flush();
                    ch = getchar();
                }
            }

            if (ch == 'y' || ch == 'Y' || hasYes) {
                cleanMonitor(addr);
                removed.push_back("{ \"removed\": \"" + addr + "\" }");

            } else {
                if (isApiMode())
                    removed.push_back("{ \"removed\": \"" + addr + "\" }");
                else
                    EXIT_USAGE("Monitor not removed.");
            }
        }
    }

    if (isApiMode()) {
        expContext().exportFmt = JSON1;
        cout << exportPreamble("", "");
        string_q msg;
        bool first = true;
        for (auto remove : removed) {
            if (!first)
                msg += ",";
            msg += remove;
            first = false;
        }
        if (msg.empty())
            msg = "{ \"msg\": \"nothing was removed\" }";
        cout << msg;
        cout << exportPostamble(errors, "") << endl;
    }

    EXIT_NOMSG(true);
}
