/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool processMerge(COptions& options) {

    if (!options.isMerge)
        return true;

    cerr << cYellow << "You've asked to merge the following cache files:\n" << cOff;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++)
        cerr << "\t" << options.filenames[i] << "\n";
    cerr << iMagenta << "\tThe result will be placed in the file ./merged.bin.\n" << cOff;
    cerr << "\tContinue? (y=yes) > ";
    int ch = (isTestMode() ? 'y' : getchar());
    if (ch != 'y' && ch != 'Y') {
        cerr << "Merge request ignored.\n";
        return false;  // return false so we don't continue
    }

    cerr << "Caches are being merged.\n";
    CAcctCacheItemArray merged;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++) {
        cerr << "\tMerging file: " << options.filenames[i] << "\n";
        SFArchive cache(READING_ARCHIVE);
        if (!cache.Lock(options.filenames[i], binaryReadOnly, LOCK_NOWAIT))
            return usage("Could not open file: " + options.filenames[i] + ". Quitting.");
        while (!cache.Eof()) {
            uint32_t which = 0; uint64_t blockNum = 0, transID = 0;
            cache >> which >> blockNum >> transID;
            merged[merged.getCount()] = CAcctCacheItem(blockNum, transID, (int32_t)which);
        }
        cache.Release();
    }
    merged.Sort(sortByBlock);

    SFArchive mergeFile(WRITING_ARCHIVE);
    if (!mergeFile.Lock("./merged.bin", binaryWriteCreate, LOCK_WAIT))
        return usage("Could not open merge file: ./merged.bin. Quitting.");
    for (uint32_t i = 0 ; i < merged.getCount() ; i++)
        mergeFile << merged[i].which << merged[i].blockNum << merged[i].transIndex;
    mergeFile.Release();

    // We must fix the file to remove duplicates
    if (!options.mode.Contains("fix"))
        options.mode += "|fix";
    options.mode.ReplaceAll("||", "|");
    options.mode = Strip(options.mode,'|');

    // We are only interested now in the merged file
    options.filenames.Clear();
    options.filenames[options.filenames.getCount()] = "./merged.bin";

    return true;
}
