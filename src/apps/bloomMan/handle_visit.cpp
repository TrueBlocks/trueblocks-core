/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern string_q formatBloom(const bloom_t& b1, bool bits);
//-------------------------------------------------------------------------
bool handle_visit(blknum_t bn, void *data) {

    COptions *options = (COptions *)data;

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (verbose)
        cout << "path: " << bTeal << path << cOff << endl;

    CBlock block;
    getBlock(block, bn);

    CAddressArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);

    cout << (options->isReWrite ? "Rewriting block " : "Visiting block ") << bn << endl;

    CBloomArray newBlooms;
    for (size_t j = 0 ; j < array.size() ; j++) {
        addAddrToBloom(array[j], newBlooms, options->bitBound);
        if (verbose)
            cerr << "adding " << array[j] << "\n";
        if (verbose > 1)
            cerr << compareBlooms(makeBloom(array[j]), newBlooms[0]) << "\n";
    }

    if (options->isReWrite) {
        if (!isTestMode())
            writeBloomArray(newBlooms, path);
        cerr << "rewrote " << path << "\n";
    }

    for (size_t bl = 0 ; bl < newBlooms.size() ; bl++) {
        cout << cYellow << "\n\tbloom: " << cTeal << bl << cYellow << " bits: " << cTeal << bitsTwiddled(newBlooms[bl]) << cOff;
        if (verbose)
            cout << bloom_2_BitStr(newBlooms[bl]) << "\n";
        else
            cout << bloom_2_ByteStr(newBlooms[bl]) << "\n";
    }
    cerr.flush();
    cout.flush();

    return !shouldQuit();
}
