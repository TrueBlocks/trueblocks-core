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

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    if (!freshen_internal(FM_PRODUCTION, addrs, "", freshen_flags))
        EXIT_FAIL("'chifra export' freshen_internal returned false");

    size_t cnt = 0;
    for (auto addr : addrs) {
        ostringstream os;
        os << "acctExport " << addr << " " << tool_flags;
        LOG4("Calling " + os.str());
        CStringArray cmds;
        explode(cmds, os.str(), ';');
        bool quit = false;
        for (size_t i = 0; i < cmds.size() && !quit; i++) {
            int ret = system(cmds[i].c_str());
            quit = (ret != 0);
            if (verbose)
                cerr << "command: " << trim(cmds[i]) << " returned with '" << quit << "'" << endl;
        }

        if (++cnt < addrs.size())
            usleep(500000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }
    EXIT_NOMSG8(true);
}
