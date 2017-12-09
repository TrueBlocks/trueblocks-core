/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool processExtract(COptions& options) {

    if (!options.isExtract)
        return true;

    cerr << "Transactions for contract " << options.extractID << " are being extracted.\n";

    CAcctCacheItemArray extract;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++) {
        cerr << "\tExtracting contract " << options.extractID << " from file: " << options.filenames[i] << "\n";

        SFArchive cache(READING_ARCHIVE);
        if (!cache.Lock(options.filenames[i], binaryReadOnly, LOCK_NOWAIT))
            return usage("Could not open file: " + options.filenames[i] + ". Quitting.");

        while (!cache.Eof()) {
            uint32_t which = 0; uint64_t blockNum = 0, transID = 0;
            cache >> which >> blockNum >> transID;
            if (which == options.extractID)
                extract[extract.getCount()] = CAcctCacheItem(blockNum, transID, (int32_t)which);
        }

        cache.Release();
    }
    extract.Sort(sortByBlock);

    SFArchive extractFile(WRITING_ARCHIVE);
    if (extractFile.Lock("./extract.bin", binaryWriteCreate, LOCK_WAIT)) {
        for (uint32_t i = 0 ; i < extract.getCount() ; i++)
            extractFile << extract[i].which << extract[i].blockNum << extract[i].transIndex;
        extractFile.Release();
        cerr << "\t" << extract.getCount() << " records were extracted into ./extract.bin.\n";
    } else {
        return usage("Could not create file: ./extract.bin. Quitting...");
    }

    // We must fix the file to remove duplicates
    if (!options.mode.Contains("fix"))
        options.mode += "|fix";
    options.mode.ReplaceAll("||", "|");

    // We are only interested now in the merged file
    options.filenames.Clear();
    options.filenames[options.filenames.getCount()] = "./extract.bin";

    return true;
}
