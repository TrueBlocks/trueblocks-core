/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_init(void) {
    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    for (auto addr : addrs) {
        string_q stagingPath = monitorsPath + "staging/";
        establishFolder(stagingPath);

        string_q stageName, prodName;
        stageName = stagingPath + addr + ".acct.bin";
        prodName = monitorsPath + addr + ".acct.bin";

        if (!fileExists(prodName)) {
            if (!freshen_internal(stagingPath, addr, ""))
                return false;
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

        } else {
            cerr << "The file ";
            cerr << cTeal << "./" << addr << ".acct.bin" << cOff;
            cerr << " already exists. Quitting..." << endl;
        }
    }

    return true;
}
