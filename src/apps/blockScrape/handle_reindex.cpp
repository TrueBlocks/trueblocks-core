/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------------------
bool markNonEmptyBlocks(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", markNonEmptyBlocks, data);

    } else {

        if (endsWith(path, ".txt")) {
            CArchive *pRes = (CArchive*)data;
            ASSERT(pRes && pRes->isOpen());

            cerr << "\tMarking " << path << "              \r";
            blknum_t stop = 0;
            blknum_t start = bnFromPath(path, stop);
            lockSection(true);
            for (blknum_t bn = start ; bn < stop ; bn++) {
                *pRes << (uint32_t)bn << (uint32_t)0 << (uint16_t)0;
                pRes->flush();
            }
            lockSection(false);

        } else {
            cerr << "\tSkipping " << path << "              \r";

        }
        cerr.flush();
    }

    // return true if we should continue (i.e. we should not quit)
    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool build_final_block_index(void) {

    // If the full block index already exists, we don't need to re-create it
    string_q finFile = substitute(finalBlockIndex_v2, ".bin", ".fin");
    if (fileExists(finFile) && fileSize(finalBlockIndex_v2) > 0)
        return true;

    cerr << bGreen << "Rebuilding full block index...\n" << cOff;
    cerr.flush();

    // Otherwise we rebuild it from scratch by visiting each binary block
    CArchive finalBlockCache(WRITING_ARCHIVE);
    if (finalBlockCache.Lock(finalBlockIndex_v2, modeWriteCreate, LOCK_WAIT)) {
        ASSERT(finalBlockCache.isOpen());

        bool finished = forEveryFileInFolder(indexFolder_sorted_v2, markNonEmptyBlocks, &finalBlockCache);
        finalBlockCache.Release();

        if (finished) {
            cerr << bGreen << "Re-indexing completed.\n" << cOff;
            stringToAsciiFile(finFile, "Removing this file will cause the index to rebuild\n");
        } else {
            cerr << bRed << "Re-indexing aborted, will re-run next time blockScrape is started.\n" << cOff;
        }
        cerr.flush();
        return finished;
    }

    cerr << "build_final_block_index failed: " << finalBlockCache.LockFailure() << "\n";
    return false;
}
