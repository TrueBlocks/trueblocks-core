/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_slurp(void) {
    ENTER8("handle_" + mode);
    nodeNotRequired();

    if (contains(tool_flags, "help")) {
        ostringstream os;
        os << "ethslurp --help";
        LOG_CALL(os.str());
        // clang-format off
        if (system(os.str().c_str())) {} // Don't remove cruft. Silences compiler warnings
        // clang-format on
        EXIT_NOMSG8(true);
    }

    if (addrs.empty())
        EXIT_USAGE("'chifra slurp' requires an Ethereum address.");

    for (auto addr : addrs) {
        ostringstream os;
        os << "ethslurp " << tool_flags << " " << addr;
        LOG_CALL(os.str());
        // clang-format off
        if (system(os.str().c_str())) {} // Don't remove cruft. Silences compiler warnings
        // clang-format on
    }

    EXIT_NOMSG8(true);
}
