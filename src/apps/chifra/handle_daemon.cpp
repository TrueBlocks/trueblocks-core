/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
// Run the scraper to build (or keep fresh) the address index
//------------------------------------------------------------------------------------------------
bool COptions::handle_daemon(void) {

    ostringstream os;
    os << "cd " << monitorsPath << " ; ";
    os << "chifraDaemon.py " << flags;
    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
    else {
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    }
    return true;

}
