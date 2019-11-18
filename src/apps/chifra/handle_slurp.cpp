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
        NOTE_CALL(os.str());
        int ret = system(os.str().c_str());
        ret = 0;  // Don't remove. Silences compiler warnings
        EXIT_NOMSG8(true);
    }

    if (addrs.empty())
        EXIT_USAGE("'chifra slurp' requires an Ethereum address.");

    for (auto addr : addrs) {
        ostringstream os;
        os << "ethslurp " << tool_flags << " " << addr;
        NOTE_CALL(os.str());
        int ret = system(os.str().c_str());
        ret = 0;  // Don't remove. Silences compiler warnings
    }

    EXIT_NOMSG8(true);
}
