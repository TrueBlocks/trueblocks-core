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

    size_t cnt = 0, cnt2 = 0;
    string_q tenAddresses;
    for (auto f : fa) {
        bool needsUpdate = true;
        if (needsUpdate) {
            LOG4(cTeal, "Needs update ", f.address, string_q(80, ' '), cOff);
            tenAddresses += (f.address + " ");
            if (!(++cnt % 10)) {  // we don't want to do too many addrs at a time
                tenAddresses += "|";
                cnt = 0;
            }
        } else {
            LOG4(cTeal, "Scraping addresses ", f.address, " ", cnt2, " of ", fa.size(), string_q(80, ' '), cOff, "\r");
        }
        cnt2++;
    }

    // Process them until we're done
    uint64_t cur = 0;
    while (!tenAddresses.empty()) {
        string_q thisFive = nextTokenClear(tenAddresses, '|');
        string_q cmd = substitute(base.str(), "[ADDRS]", thisFive);
        LOG_CALL(cmd);
        // clang-format off
        uint64_t n = countOf(thisFive, ' ');
        if (fa.size() > 1)
            LOG_INFO(cTeal, "Scraping addresses ", cur, "-", (cur+n-1), " of ", fa.size(), string_q(80, ' '), cOff);
        cur += n;
        if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        if (!tenAddresses.empty())
            usleep(250000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }

    for (CMonitor& f : fa)
        f.needsRefresh = (f.cntBefore != f.getRecordCount());

    EXIT_NOMSG(true);
}
