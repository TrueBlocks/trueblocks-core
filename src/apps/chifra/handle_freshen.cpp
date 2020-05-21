/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool freshen_internal(freshen_e mode, CMonitorArray& fa, const string_q& tool_flags, const string_q& freshen_flags) {
    ENTER("freshen_internal");
    nodeNotRequired();

    ostringstream base;
    base << "acctScrape " << tool_flags << " " << freshen_flags << " [ADDRS] ;";

    // Build collections of five addresses at a time
    size_t cnt = 0;
    string_q fiveAddresses;
    for (auto f : fa) {
        fiveAddresses += (f.address + " ");
        if (!(++cnt % 5)) {  // we don't want to do too many addrs at a time
            fiveAddresses += "|";
            cnt = 0;
        }
    }

    // Process them until we're done
    uint64_t cur = 0;
    while (!fiveAddresses.empty()) {
        string_q thisFive = nextTokenClear(fiveAddresses, '|');
        string_q cmd = substitute(base.str(), "[ADDRS]", thisFive);
        LOG_CALL(cmd);
        // clang-format off
        uint64_t n = countOf(thisFive, ' ');
        if (fa.size() > 1)
            LOG_INFO("Scraping addresses ", cur, "-", (cur+n-1), " of ", fa.size(), string_q(80, ' '));
        cur += n;
        if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        if (!fiveAddresses.empty())
            usleep(500000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }

    for (CMonitor& f : fa)
        f.cntAfter = f.getRecordCount();

    EXIT_NOMSG(true);
}
