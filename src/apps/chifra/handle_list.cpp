/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {

    // list mode does not require a running node since it only reads the account cache
    nodeNotRequired();

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    for (auto addr : addrs) {
        if (!freshen_internal(monitorsPath, addr, ""))
            return false;
        ostringstream os;
        os << "cd " << monitorsPath << " ; ";
        os << "cacheMan -d " << addr << ".acct.bin ; ";
        if (isTestMode())
            cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
        else
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    }

    return true;
}
