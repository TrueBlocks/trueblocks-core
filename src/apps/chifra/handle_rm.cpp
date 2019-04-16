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

    // ls mode does not require a running node
    nodeNotRequired();

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    for (auto addr : addrs) {
        string_q fn = getMonitorPath(addr);
        if (!fileExists(fn)) {
            cout << "monitor not found for address " << addr << "." << endl;
        } else {
            cout << "remove monitor for " << addr << "? (y=yes) >";
            cout.flush();
            int ch = getchar();
            if (ch == 'y' || ch == 'Y') {
                ostringstream os;
                os << "cd " << getMonitorPath("") << " ; ";
                os << "rm -f " << addr << ".* ; ";
                if (isTestMode())
                    cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
                else
                    if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            } else {
                cout << "...not removed." << endl;
            }
        }
    }

    return true;
}
