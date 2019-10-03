/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    if (addrs.empty())
        EXIT_USAGE("'chifra list' requires an Ethereum address.");

    if (!freshen_internal(FM_PRODUCTION, addrs, "", freshen_flags))
        EXIT_FAIL("'chifra list' freshen_internal returned false");

    for (auto addr : addrs) {
        ostringstream os;
        os << "cacheMan " << " --fix " << addr << " >&2 ; ";
        os << "acctExport --appearances " << tool_flags << " " << addr << " ; ";
        LOG4("Calling " + os.str());
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    }

    EXIT_NOMSG4(true);
}
