/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_balances(void) {

    ENTER4("handle_" + mode);
    nodeRequired();
    //if (!nodeHasBalances())
    //    EXIT_USAGE("This function requires an Ethereum archive node to work.");

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    if (!freshen_internal(FM_PRODUCTION, addrs, "", freshen_flags))
        EXIT_FAIL("'chifra export' freshen_internal returned false");

    for (auto addr : addrs) {
        ostringstream os;
        os << "cd " << getMonitorPath("") << " ; ";
        os << "cacheMan " << " -d " << addr << ".acct.bin | cut -f1 | sed 's/^/getState --noHeader --ether " << tool_flags << " " << addr << " /' >/tmp/results ; ";
        LOG1("Calling " + os.str());
        if (isTestMode())
            cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
        else
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        CStringArray lines;
        asciiFileToLines("/tmp/results", lines);
        for (auto line : lines) {
            if (system(line.c_str())) { }  // Don't remove. Silences compiler warnings
            usleep(50000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
        }
    }

    EXIT_NOMSG4(true);
}
