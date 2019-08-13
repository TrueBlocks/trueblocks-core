/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_status(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    LOG5("tool_flags: " + tool_flags);
    ostringstream os;
    os << "cacheStatus " << tool_flags << " ; ";
    if (isTestMode())
        cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    EXIT_NOMSG4(true);
}
