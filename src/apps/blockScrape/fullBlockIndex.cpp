/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------------------
bool markFullBlocks(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", markFullBlocks, data);

    } else {

        if (contains(path, ".bin")) {
            CArchive *pRes = (CArchive*)data;
            ASSERT(pRes && pRes->isOpen());

            cout << "\tMarking " << path << "              \r";
            lockSection(true);
            pRes->Write(bnFromPath(path));
            pRes->flush();
            lockSection(false);

        } else {
            cout << "\tSkipping " << path << "              \r";

        }
        cout.flush();
    }

    // return true if we should continue (i.e. we should not quit)
    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool establishBlockIndex(void) {

    // If the full block index already exists, we don't need to re-create it
    string_q finFile = substitute(fullBlockIndex, ".bin", ".fin");
    if (fileExists(finFile) && fileSize(fullBlockIndex) > 0)
        return true;

    cerr << bGreen << "Rebuilding full block index...\n" << cOff;
    cerr.flush();

    // Otherwise we rebuild it from scratch by visiting each binary block
    CArchive fullBlockCache(WRITING_ARCHIVE);
    if (fullBlockCache.Lock(fullBlockIndex, binaryWriteCreate, LOCK_WAIT)) {
        lockSection(true);
        ASSERT(fullBlockCache.isOpen());
        bool finished = forEveryFileInFolder(bloomFolder, markFullBlocks, &fullBlockCache);
        fullBlockCache.Release();
        lockSection(false);
        if (finished) {
            cerr << bGreen << "Re-indexing completed.\n" << cOff;
            stringToAsciiFile(finFile, "Removing this file will cause the index to rebuild\n");
        } else {
            cerr << bRed << "Re-indexing aborted, will re-run next time blockScrape is started.\n" << cOff;
        }
        cerr.flush();
        return finished;
    }

    cerr << "establishBlockIndex failed: " << fullBlockCache.LockFailure() << "\n";
    return false;
}

//--------------------------------------------------------------------------------
bool updateIndex(CArchive& fullBlockCache, blknum_t bn) {

    ASSERT(fullBlockCache.isOpen());
    fullBlockCache.Write(bn);
    fullBlockCache.flush();

    return true;
}
