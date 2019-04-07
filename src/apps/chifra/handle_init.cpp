/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_init(void) {

    // init mode technically does not require a running node since it only reads the index
    nodeNotRequired();

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    string_q stagingPath = monitorsPath + "staging/";
    establishFolder(stagingPath);

    CAddressArray runs;
    for (auto addr : addrs) {
        string_q prodName;
        prodName = monitorsPath + addr + ".acct.bin";
        if (fileExists(prodName)) {
            cerr << "The file " << cTeal << "./" << addr << ".acct.bin" << cOff;
            cerr << " already exists." << endl;
        } else {
            runs.push_back(addr);
        }
    }

    if (!runs.size())
        return true;

    if (!freshen_internal(stagingPath, runs, "", freshen_flags))
        return false;

    for (auto addr : runs) {
        string_q stageName, prodName;
        stageName = stagingPath + addr + ".acct.bin";
        prodName = monitorsPath + addr + ".acct.bin";

        if (verbose)
            cerr << "Moving " << cTeal << stageName << cOff << " to " << cTeal << prodName << cOff << endl;

        if (!isTestMode())
            moveFile(stageName, prodName);
        else
            cerr << "Would have moved "
                    << substitute(stageName, getCachePath(""), "$BLOCK_CACHE/") << " to "
                    << substitute(prodName, getCachePath(""), "$BLOCK_CACHE/") << endl;

        stageName = stagingPath + addr + ".last.txt";
        prodName = monitorsPath + addr + ".last.txt";
        if (verbose)
            cerr << "Moving " << cTeal << stageName << cOff << " to " << cTeal << prodName << cOff << endl;
        if (!isTestMode())
            moveFile(stageName, prodName);
        else
            cerr << "Would have moved "
                    << substitute(stageName, getCachePath(""), "$BLOCK_CACHE/") << " to "
                    << substitute(prodName, getCachePath(""), "$BLOCK_CACHE/") << endl;
    }

    return true;
}
