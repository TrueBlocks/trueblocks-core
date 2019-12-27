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

    if (contains(tool_flags, "help")) {
        optionOn(OPT_HELP);
        return usage();
    }

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
        exists |= fileExists(getMonitorCnfg(addr));
        string_q delFn = getMonitorPath(addr + ".deleted");
        exists |= fileExists(delFn);

        if (!exists) {
            if (!hasYes)
                LOG_WARN("Monitor not found for address " + addr + ".");

        } else {
            int ch = 'n';  // default to no in both command line and api cases
            if (!hasYes) {
                if (isApiMode()) {
                    bool exists = fileExists(delFn);
                    if (exists)
                        ::remove(delFn.c_str());
                    else
                        stringToAsciiFile(delFn, Now().Format(FMT_EXPORT));
                    return true;
                } else {
                    cerr << "Remove monitor for " << addr << "? (y=yes) >";
                    cerr.flush();
                    ch = getchar();
                }
            }

            if (ch == 'y' || ch == 'Y' || hasYes) {
                ostringstream os;
                os << "cd " << getMonitorPath("") << " && ";
                os << "rm -f " << addr << ".*";
                LOG_CALL(os.str());
                // clang-format off
                if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
                // clang-format on
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
            msg = "{ \"msg\": \"nothing was removed\" }";
        cout << msg;
        cout << exportPostamble(JSON1, errors, "") << endl;
    }

    EXIT_NOMSG8(true);
}
