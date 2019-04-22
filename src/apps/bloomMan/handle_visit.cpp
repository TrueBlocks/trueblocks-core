/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_visit(blknum_t bn, void *data) {

    string_q path = getBinaryCacheFilename(CT_BLOOMS, bn);
    if (!fileExists(path)) {
        if (verbose)
            cerr << "Skipping " << path << "\r";
        return !shouldQuit();
    }

    CBloomArray blooms;
    readBloomFromBinary(blooms, path);

    cout << "\n" << cYellow << "Visiting block: " << cTeal << bn << cOff << endl;
    if (verbose && !isTestMode()) {
        cout << "\t" << cYellow << "path: " << cTeal << path << cOff;
        cout << " (" << fileSize(path) << " bytes)" << endl;
    }

    uint64_t cnt = 0;
    for (auto bloom : blooms) {
        cout << "\t";
        cout << cYellow << "bloom: " << cTeal << cnt++ << " ";
        cout << cYellow << "bits: "  << cTeal << bitsTwiddled(bloom) << endl;
        cout << cOff << (verbose > 1 ? bloom_2_BitStr(bloom) : bloom_2_ByteStr(bloom));
        cout << endl;
    }

    return !shouldQuit();
}
