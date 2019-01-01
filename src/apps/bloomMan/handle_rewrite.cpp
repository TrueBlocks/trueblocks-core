/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
// Note: We always re-write the bloom unless it has no addresses (which may
// happen if the miner address is zero in a zero-transaction block.
bool handle_rewrite(blknum_t bn, void *data) {

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");

    CBlock block;
    getBlock(block, bn);

    CAddressArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);

    if (array.empty())
        return !shouldQuit();

    CBloomArray blooms;
    for (size_t j = 0 ; j < array.size() ; j++) {
        COptions *options = (COptions *)data;
        addAddrToBloom(array[j], blooms, options->bitBound);
        if (verbose)
            cerr << "Adding " << array[j] << endl;
        if (verbose > 1)
            cerr << compareBlooms(makeBloom(array[j]), blooms[blooms.size()-1]) << endl;
    }

    if (!isTestMode())
        writeBloomArray(blooms, path);

    cout << "\n" << cYellow << "Rewrote block: " << cTeal << bn << cOff << endl;
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
