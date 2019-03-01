/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_init(void) {
    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    string_q f = flags; flags = "";
    if (contains(f, "--force"))
        freshen_internal(BOTH);
    else
        freshen_internal(INDEX);
    flags = f;

    return true;
}
