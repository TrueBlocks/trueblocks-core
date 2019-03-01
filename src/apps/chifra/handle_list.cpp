/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    string_q f = flags; flags = "";
    freshen_internal(INDEX);
    flags = f;

    ostringstream os;
    os << "cd " << monitorsPath << " ; ";
    os << "cacheMan -d " << addrs[0] << ".acct.bin ; ";

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    return true;
}
