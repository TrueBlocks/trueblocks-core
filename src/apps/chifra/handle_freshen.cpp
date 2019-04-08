/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool freshen_internal(const string_q& path, const address_t& addr, const string_q& tool_flags, const string_q& freshen_flags) {

    // freshen mode technically does not require a running node since it only reads the index
    nodeNotRequired();

    ostringstream os;
    os << "cd " << path << " ; ";
    os << "acctScrape " << tool_flags << " " << freshen_flags << " " << addr << " ; ";

    if (isTestMode())
        cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
    else {
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        usleep(500000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }

    return true;
}

//------------------------------------------------------------------------------------------------
bool freshen_internal(const string_q& path, const CAddressArray& addrs, const string_q& tool_flags, const string_q& freshen_flags) {

#if 0
    for (auto addr : addrs)
        if (!freshen_internal(path, addr, tool_flags, freshen_flags))
            return false;
#else
    // freshen mode technically does not require a running node since it only reads the index
    nodeNotRequired();

    ostringstream base;
    base << "cd " << path << " ; ";
    base << "acctScrape " << tool_flags << " " << freshen_flags << " [ADDRS] ;";

    string_q addrStr;
    for (auto addr : addrs) {
        addrStr += (addr + " ");
        if (!(countOf(addrStr, ' ') % 5)) // we don't want to do more than 25 addresses at a time
            addrStr += "|";
    }

    while (!addrStr.empty()) {
        string_q cmd = base.str();
        replace(cmd, "[ADDRS]", nextTokenClear(addrStr, '|'));
        if (isTestMode())
            cout << substitute(cmd, getCachePath(""), "$BLOCK_CACHE/") << endl;
        else {
            if (system(cmd.c_str())) { }  // Don't remove. Silences compiler warnings
            usleep(500000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
        }
    }
#endif
    return true;
}
