/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_rm(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    for (auto addr : addrs) {
        string_q fn = getMonitorPath(addr);
        string_q fnLast = getMonitorLast(addr);
        string_q fnExpt = getMonitorExpt(addr);
        string_q fnBals = getMonitorBals(addr);
        string_q fnConf = substitute(fn, ".acct.bin", ".toml");
        if (!fileExists(fn) && !fileExists(fnLast) && !fileExists(fnExpt) && !fileExists(fnBals) && !fileExists(fnConf)) {
            LOG_WARN("Monitor not found for address " + addr + ".");

        } else {
            int ch = 'n';
            if (!isApiMode() && !contains(tool_flags, "--yes")) {
                cerr << "Remove monitor for " << addr << "? (y=yes) >";
                cerr.flush();
                ch = getchar();
            }

            if (ch == 'y' || ch == 'Y' || contains(tool_flags, "--yes")) {
                ostringstream os;
                os << "cd " << getMonitorPath("") << " ; ";
                os << "rm -f " << addr << ".* ; ";
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

            } else {
                EXIT_USAGE("Monitor not removed.");

            }
        }
    }

    EXIT_NOMSG4(true);
}
