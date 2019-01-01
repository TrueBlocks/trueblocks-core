/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_check(blknum_t bn, void *data) {

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (verbose)
        cout << "\tpath: " << bTeal << path << cOff << "\n";
    if (!fileExists(path))
        return !shouldQuit();

    CBlock block;
    getBlock(block, bn);

    CAddressArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);

    CBloomArray blooms;
    readBloomArray(blooms, path);
    for (size_t j = 0 ; j < array.size() ; j++) {
        bool hit = false;
        bloom_t hitBloom;
        for (size_t x = 0 ; x < blooms.size() && !hit ; x++) {
            if (isBloomHit(makeBloom(array[j]), blooms[x])) {
                hit = true;
                hitBloom = blooms[x];
            }
        }
        if (hit)
            cerr << "\tAddress " << array[j] << " in bloom " << bn << (verbose ? "\n" : "                           \r");
        else
            cout << "\tAddress " << array[j] << " not in bloom for block " << bn << "\n";

        if (verbose) {
            string_q res;
            compareBlooms(makeBloom(array[j]), hitBloom, res);
            if (!res.empty())
                cerr << res << "\n";
        }
    }
    cerr.flush();
    cout.flush();

    return !shouldQuit();
}
