/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool statsFunc(blknum_t bn, void *data) {

    CVisitor *v = (CVisitor*)data;

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (fileExists(path)) {
        CBloomArray blooms;
        readBloomArray(blooms, path);
        v->nWithBlooms++;
        if (blooms.size()) {
            v->nBlooms += blooms.size();
            v->nBytes += fileSize(path);
            for (uint32_t i = 0 ; i < blooms.size() ; i++)
                v->nBits += bitsTwiddled(blooms[i]);
        }
    } else {
        v->nWithoutBlooms++;
    }

    blknum_t marker = (bn / v->options.bucketSize) * v->options.bucketSize;
    if (marker != v->lastMarker) {
        if (v->nBlocks)
            v->report(marker); // report
        if (!v->options.isCummulative)
            v->Reset();
        v->lastMarker = marker;
    } else {
        v->interumReport(bn);
    }
    v->nBlocks++;

    return !shouldQuit();
}
