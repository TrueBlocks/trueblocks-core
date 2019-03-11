/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_freshen(void) {

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");
    for (auto addr : addrs)
        if (!freshen_internal(monitorsPath, addr, flags, BOTH))
            return false;
    return true;
}

//------------------------------------------------------------------------------------------------
bool freshen_internal(const string_q& path, const address_t& addr, const string_q& flagsIn, FreshenMode fMode) {

    ostringstream os;
    os << "cd " << path << " ; ";

    if (fMode & INDEX)
        os << "acctScrape " << flagsIn << " " << addr << " --useIndex ; ";
    if (fMode & BLOOM)
        os << "acctScrape " << flagsIn << " " << addr << " --maxBlocks 50000 ; ";

    if (isTestMode())
        cout << substitute(os.str(), blockCachePath(""), "$BLOCK_CACHE/") << endl;
    else {
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        usleep(500000);
    }

    return true;
}
