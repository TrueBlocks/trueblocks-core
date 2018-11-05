/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
