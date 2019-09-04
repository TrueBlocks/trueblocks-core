/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_export(void) {

    ENTER4("handle_" + mode);
    nodeRequired();

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    if (!freshen_internal(FM_PRODUCTION, addrs, "", freshen_flags))
        EXIT_FAIL("'chifra export' freshen_internal returned false");

    size_t cnt = 0;
    for (auto addr : addrs) {
        ostringstream os;
        os << "grabABI " << addr << " 1>/dev/null 2>&1; ";
        os << "acctExport " << addr << " " << tool_flags;
        LOG4("Calling " + os.str());
        if (isTestMode())
            cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
        else {
            if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            if (++cnt < addrs.size())
                usleep(500000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
        }
    }
    EXIT_NOMSG4(true);
}
