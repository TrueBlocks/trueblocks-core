/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool freshen_internal(freshen_e mode, CFreshenArray& fa, const string_q& tool_flags, const string_q& freshen_flags) {
    ENTER8("freshen_internal");
    nodeNotRequired();

    ostringstream base;
    base << "acctScrape " << tool_flags << " " << freshen_flags << " [ADDRS] ;";

    // Build groups of five addresses at a time
    size_t cnt = 0;
    string_q groupsOfFive;
    for (auto f : fa) {
        groupsOfFive += (f.address + " ");
        if (!(++cnt % 5)) {  // we don't want to do too many addrs at a time
            groupsOfFive += "|";
            cnt = 0;
        }
    }

    // Process them until we're done
    while (!groupsOfFive.empty()) {
        string_q thisGroup = nextTokenClear(groupsOfFive, '|');
        string_q cmd = substitute(base.str(), "[ADDRS]", thisGroup);
        LOG_CALL(cmd);
        // clang-format off
        if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        if (!groupsOfFive.empty())
            usleep(500000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }

    for (CFreshen& f : fa)
        f.cntAfter = fileSize(getMonitorPath(f.address)) / sizeof(CAppearance_base);

    EXIT_NOMSG8(true);
}
