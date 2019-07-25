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
        if (!fileExists(fn) && !fileExists(fnLast)) {
            LOG_WARN("Monitor not found for address " + addr + ".");

        } else {
            cerr << "Remove monitor for " << addr << "? (y=yes) >";
            cerr.flush();
            int ch = getchar();
            if (ch == 'y' || ch == 'Y') {
                ostringstream os;
                os << "cd " << getMonitorPath("") << " ; ";
                os << "rm -f " << addr << ".* ; ";
                if (isTestMode())
                    LOG_INFO(substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/"));
                else
                    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

            } else {
                LOG_INFO("Monitor not removed.");

            }
        }
    }

    EXIT_NOMSG4(true);
}
