/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_addrs(blknum_t bn, void *data) {

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (verbose)
        cout << "\tpath: " << bTeal << path << cOff << "\n";

    COptions *options = (COptions *)data;
    uint64_t bitBound = options->bitBound;

    static size_t since_hit=0;
    since_hit++;
    if (verbose)
        cout << "For block: " << bn << "\n";

    if (fileExists(path)) {
        CBloomArray blooms;
        readBloomArray(blooms, path);
        for (uint32_t x = 0 ; x < blooms.size() ; x++) {
            string_q addrs = options->address_list;
            while (!addrs.empty()) {
                string_q addr = nextTokenClear(addrs,'|');
                string_q res;
                if (verbose) {
                    cout << "\tcheck " << cRed << bn << "." << x << cOff << " at ";
                    cout << addr << " (";
                    cout << bTeal << (compareBlooms(makeBloom(addr), blooms[x], res) ? "true" : "false") << cOff;
                    cout << cOff << ")";
                    cout << res << "\n";
                } else {
                    if (isBloomHit(makeBloom(addr), blooms[x])) {
                        cout << addr << "\t" << bn << "\n";
                        since_hit = 0;
                    } else
                        cerr << "\t" << bn << string_q((since_hit/bitBound), '.') << "\r";
                }
            }
        }
    } else {
        if (verbose) {
            cerr << "\tNo bloom file at block " << bn << "\n\n";
        } else {
            cerr << "\t" << bn << string_q((since_hit/bitBound), '.') << "\r";
        }
    }
    cerr.flush();
    cout.flush();
    return true;
}
