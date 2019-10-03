/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_slurp(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    if (addrs.empty())
        EXIT_USAGE("'chifra slurp' requires an Ethereum address.");

    LOG5("tool_flags: " + tool_flags);
    for (auto addr : addrs) {
        ostringstream os;
        os << "ethslurp " << tool_flags << " " << addr << " ; ";
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    }

    EXIT_NOMSG4(true);
}
