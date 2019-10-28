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

    ENTER8("handle_" + mode);
    nodeNotRequired();

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    CStringArray removed;
    bool hasYes = contains(tool_flags, "--yes");
    for (auto addr : addrs) {

        bool exists = false;
        exists |= fileExists(getMonitorPath(addr));
        exists |= fileExists(getMonitorLast(addr));
        exists |= fileExists(getMonitorExpt(addr));
        exists |= fileExists(getMonitorBals(addr));
        exists |= fileExists(substitute(getMonitorPath(addr), ".acct.bin", ".toml"));

        if (!exists) {
            if (!hasYes)
                LOG_WARN("Monitor not found for address " + addr + ".");

        } else {
            int ch = 'n'; // default to no in both command line and api cases
            if (!isApiMode() && !hasYes) {
                cerr << "Remove monitor for " << addr << "? (y=yes) >";
                cerr.flush();
                ch = getchar();
            }

            if (ch == 'y' || ch == 'Y' || hasYes) {
                ostringstream os;
                os << "cd " << getMonitorPath("") << " && ";
                os << "rm -f " << addr << ".*";
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
                removed.push_back("{ \"removed\": \"" + addr + "\" }");

            } else {
                EXIT_USAGE("Monitor not removed.");

            }
        }
    }

    if (isApiMode()) {
        cout << exportPreamble(JSON1, "", "X");
        string_q msg;
        bool first = true;
        for (auto remove : removed) {
            if (!first)
                msg += ",";
            msg += remove;
            first = false;
        }
        if (msg.empty())
            msg = "{ \"msg\": \"noting was removed\" }";
        cout << msg;
        cout  << exportPostamble(JSON1, errors, "") << endl;
    }

    EXIT_NOMSG8(true);
}
