/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_addrs(blknum_t bn, void *data) {

    COptions *options = (COptions *)data;

    string_q path = getBinaryCacheFilename(CT_BLOOMS, bn);
    if (!fileExists(path)) {
        if (verbose || bn < firstTransactionBlock)
            cerr << "Skipping " << path << endl;
        return !shouldQuit();
    }

    size_t wid = 12;
    cout << cYellow << "Checking: " << cTeal << bn << cOff << endl;
    if (verbose && !isTestMode()) {
        cout << cYellow << padLeft("path:    ", wid) << cTeal << path << cOff;
        cout << " (" << fileSize(path) << " bytes)" << endl;
    }

    CBloomArray blooms;
    readBloomFromBinary(blooms, path);

    CAddressArray addrs;
    explode(addrs, options->address_list, '|');

    for (auto addr : addrs) {
        cout << cYellow << padLeft("address: ", wid) << cTeal << addr << cOff << " ";
        for (auto bloom : blooms) {
            switch (verbose) {
                case 0:
                    cout << (isBloomHit(makeBloom(addr), bloom) ? greenCheck : redX) << " ";
                    break;
                case 1:
                    cout << (isBloomHit(makeBloom(addr), bloom) ? greenCheck+" (hit) " : redX+" (miss) ");
                    break;
                case 2:
                case 3:
                    cout << "\n\t";
                    cout << (isBloomHit(makeBloom(addr), bloom) ? greenCheck+" (hit) " : redX+" (miss) ");
                    cout << cYellow << "bits: "  << cTeal << bitsTwiddled(bloom) << cOff << " ";
                    cout << compareBlooms(makeBloom(addr), bloom);
                    break;
            }
        }
        cout << endl;
    }
    cout << endl;
    return true;
}
