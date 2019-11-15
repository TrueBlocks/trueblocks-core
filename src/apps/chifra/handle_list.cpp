/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {

    ENTER8("handle_" + mode);
    nodeNotRequired();

    if (contains(tool_flags, "help")) {
        ostringstream os;
        os << "acctScrape --help";
        NOTE_CALL(os.str());
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        EXIT_NOMSG8(true);
    }

    if (addrs.empty())
        EXIT_USAGE("'chifra list' requires an Ethereum address.");

    if (!freshen_internal(FM_PRODUCTION, addrs, "", freshen_flags))
        EXIT_FAIL("'chifra list' freshen_internal returned false");

    for (auto addr : addrs) {
        ostringstream os;
        os << "cacheMan " << " --fix " << addr << " >&2 && ";
        os << "acctExport --appearances " << tool_flags << " " << addr;
        NOTE_CALL(os.str());
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
    }

    EXIT_NOMSG8(true);
}
