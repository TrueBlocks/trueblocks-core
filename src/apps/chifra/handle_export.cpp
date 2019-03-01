/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_export(void) {

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    string_q f = flags; flags = "";
    freshen_internal(INDEX);
    flags = f;

    ostringstream os;
    os << "cd " << monitorsPath << " ; ";
    os << "grabABI " << addrs[0] << " ; ";
    os << "acctExport" << " --for_addr " << addrs[0] << " " << flags;

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    return true;
}
