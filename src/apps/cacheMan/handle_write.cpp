/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleWrite(const string_q& outputFilename, const CAcctCacheItemArray& dataArray, CACHEFILTERFUNC filterFunc) const {

    cerr << "\tWriting...";
    uint64_t nWritten = 0;

    string_q contents;
    asciiFileToString("./cache/lastBlock.txt", contents);
    blknum_t currentLast = str_2_Uint(contents);

    CArchive txCache(WRITING_ARCHIVE);
    if (!txCache.Lock(outputFilename, binaryWriteCreate, LOCK_WAIT))
        return usage("Could not open merge file: " + outputFilename + ". Quitting.");

    CAcctCacheItem prev;
    for (uint32_t i = 0 ; i < dataArray.size() && !shouldQuit() ; i++) {
        CAcctCacheItem item = dataArray[i];
        if (prev != item && (!filterFunc || (*filterFunc)(((COptions*)this)->removals, item))) {
            lockSection(true);
            txCache << item.blockNum << item.transIndex;
            if (item.blockNum > currentLast && filterFunc == NULL)
                writeLastBlock(item.blockNum);
            lockSection(false);
            cerr << (!(nWritten % 5000) ? "." : "");
            nWritten++;
        }
        prev = item;
    }
    txCache.Release();

    cerr << cYellow << nWritten << cOff << " records written, " << cYellow << (dataArray.size() - nWritten) << cOff << " records ignored.\n";
    cerr << "\tWritten to " << cTeal << outputFilename << cOff << "\n";

    return !shouldQuit();
}

