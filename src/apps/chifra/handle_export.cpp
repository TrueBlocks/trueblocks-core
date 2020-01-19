/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_export(void) {
    ENTER8("handle_" + mode);
    nodeRequired();

    if (contains(tool_flags, "help")) {
        ostringstream os;
        os << "acctExport --help";
        LOG_CALL(os.str());
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        EXIT_NOMSG8(true);
    }

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    CFreshenArray fa;
    for (auto a : addrs)
        fa.push_back(CFreshen(a));
    if (!freshen_internal(FM_PRODUCTION, fa, "", freshen_flags))
        EXIT_FAIL("'chifra export' freshen_internal returned false");

    size_t cnt = 0;
    for (auto addr : addrs) {
        ostringstream os;
        os << "acctExport " << addr << " " << tool_flags;
        CStringArray cmds;
        explode(cmds, os.str(), ';');
        bool quit = false;
        for (size_t i = 0; i < cmds.size() && !quit; i++) {
            LOG_CALL(cmds[i]);
            // clang-format off
            quit = (system(cmds[i].c_str()) != 0);
            // clang-format on
            if (verbose)
                cerr << "command: " << trim(cmds[i]) << " returned with '" << quit << "'" << endl;
        }

        if (++cnt < addrs.size())
            usleep(500000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }
    EXIT_NOMSG8(true);
}
