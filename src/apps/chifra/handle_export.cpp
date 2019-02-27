/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_export(void) {

    if (address.empty())
        return usage("This function requires an address. Quitting...");

    handle_freshen();

    ostringstream os;
    os << "cd " << monitorsPath << " ; ";
    os << "grabABI " << address << " ; ";
    os << "acctExport" << " --for_addr " << address << " " << flags;

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    return true;
}
