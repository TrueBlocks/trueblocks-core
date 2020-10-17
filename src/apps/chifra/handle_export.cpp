/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_export(void) {
    ENTER("handle_" + mode);
    nodeRequired();

    if (contains(tool_flags, "help")) {
        ostringstream os;
        os << (mode == "list" ? "acctScrape" : "acctExport") << " --help";
        LOG_CALL(os.str());
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        EXIT_NOMSG(true);
    }

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    CMonitorArray fa;
    for (auto a : addrs) {
        CMonitor m;
        m.address = a;
        m.cntBefore = m.getRecordCount();
        m.needsRefresh = false;
        fa.push_back(m);
    }
    if (!freshen_internal(FM_PRODUCTION, fa, "", freshen_flags))
        EXIT_FAIL("'chifra " + mode + "' freshen_internal returned false");

    if (contains(tool_flags, "--count") || contains(tool_flags, "-U")) {
        ostringstream os;
        os << "acctExport " << tool_flags;
        for (auto a : addrs) {
            os << " " << a;
        }
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        EXIT_NOMSG(true);
    }

    size_t cnt = 0;
    for (auto addr : addrs) {
        ostringstream os;
        if (mode == "list")
            os << "acctExport --appearances " << tool_flags << " " << addr;
        else
            os << "acctExport " << addr << " " << tool_flags;
        CStringArray cmds;
        explode(cmds, os.str(), ';');
        bool quit = false;
        for (size_t i = 0; i < cmds.size() && !quit; i++) {
            LOG_CALL(cmds[i]);
            // clang-format off
            quit = (system(cmds[i].c_str()) != 0);
            // clang-format on
            LOG8("command: ", trim(cmds[i]), " returned with '", quit, "'");
        }
        if (++cnt < addrs.size())
            usleep(500000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }
    EXIT_NOMSG(true);
}
