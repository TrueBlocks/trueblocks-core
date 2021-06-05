/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------
bool visitUnripeIndexFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitUnripeIndexFiles, data);

    } else {
        // There may be a number of files in the unripe folder. They all have the
        // same format and file naming convention as the staging folder, the only
        // difference being is that these files are not sorted by address. We send
        // that in as a parameter.
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        if (!contains(path, "unripe/0") || !endsWith(path, ".txt") || contains(path, "-temp")) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        timestamp_t unused;
        options->fileRange.first = bnFromPath(path, options->fileRange.second, unused);
        ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that `start` and `end` options are ignored when scanning
        if (!rangesIntersect(options->listRange, options->fileRange)) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        options->stats.nStageChecked++;

        return options->queryFlatFile(path, true /* sorted */);
    }
    ASSERT(0);  // should not happen
    return !shouldQuit();
}
