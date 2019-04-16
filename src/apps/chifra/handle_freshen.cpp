/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool freshen_internal(FreshenMode mode, const CAddressArray& addrs, const string_q& tool_flags, const string_q& freshen_flags) {

    // technically does not require a running node since it only reads the index
    nodeNotRequired();

    ostringstream base;
    base << "acctScrape " << tool_flags << " " << freshen_flags << " [ADDRS] ;";

    string_q prodAddrs, stageAddrs;
    for (auto addr : addrs) {
        if (!fileExists(getMonitorPath(addr))) {
            stageAddrs += (addr + " ");
            if (!(countOf(stageAddrs, ' ') % 5)) // we don't want to do too many addrs at a time
                stageAddrs += "|";
        } else {
            prodAddrs += (addr + " ");
            if (!(countOf(prodAddrs, ' ') % 5)) // we don't want to do too many addrs at a time
                prodAddrs += "|";
        }
    }

    // do the staging addresses first (if any)
    while (!stageAddrs.empty()) {
        string_q cmd = substitute(base.str(), " ;", " --staging ;");
        string_q staged = nextTokenClear(stageAddrs, '|');
        cerr << "Initializing: " << cTeal << staged << cOff << endl;
        replace(cmd, "[ADDRS]", staged);
        if (isTestMode())
            cout << substitute(cmd, getCachePath(""), "$BLOCK_CACHE/") << endl;
        else {
            if (system(cmd.c_str())) { }  // Don't remove. Silences compiler warnings

            string_q stageName, prodName;
            while (!staged.empty()) {
                address_t addr = nextTokenClear(staged, ' ');
                stageName = getMonitorPath(addr, FM_STAGING);
                prodName = getMonitorPath(addr);
                if (verbose)
                    cerr << "Moving " << cTeal << stageName << cOff << " to " << cTeal << prodName << cOff << endl;
                if (isTestMode()) {
                    cerr << "Would have moved "
                    << substitute(stageName, getCachePath(""), "$BLOCK_CACHE/") << " to "
                    << substitute(prodName, getCachePath(""), "$BLOCK_CACHE/") << endl;
                } else {
                    moveFile(stageName, prodName);
                }
                stageName = getMonitorLast(addr, FM_STAGING);
                prodName = getMonitorLast(addr);
                if (verbose)
                    cerr << "Moving " << cTeal << stageName << cOff << " to " << cTeal << prodName << cOff << endl;
                if (!isTestMode())
                    moveFile(stageName, prodName);
                else
                    cerr << "Would have moved "
                    << substitute(stageName, getCachePath(""), "$BLOCK_CACHE/") << " to "
                    << substitute(prodName, getCachePath(""), "$BLOCK_CACHE/") << endl;
            }
            if (!stageAddrs.empty())
                usleep(500000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
        }
    }

    // now do the production addresses (if any)
    while (!prodAddrs.empty()) {
        string_q cmd = base.str();
        replace(cmd, "[ADDRS]", nextTokenClear(prodAddrs, '|'));
        if (isTestMode())
            cout << substitute(cmd, getCachePath(""), "$BLOCK_CACHE/") << endl;
        else {
            if (system(cmd.c_str())) { }  // Don't remove. Silences compiler warnings
            if (!prodAddrs.empty())
                usleep(500000); // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
        }
    }

    return true;
}
