/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitStagingIndexFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitStagingIndexFiles, data);

    } else {
        // Pick up some useful data for either method...
        COptions* options = reinterpret_cast<COptions*>(data);

        // Filenames in the staging folder take the form 'lastBlock.bin' where
        // lastBlock is the latest block in the file. The file likely contains
        // previous blocks as well (all those blocks since the last consolidation).
        // The file is sorted by block. The records are fixed length (59 bytes).
        // We copy the staging file to a temporary file, read its entire contents
        // into memory, sort the data, and do a binary search for the address we're
        // looking for. There is likely only a single file in this folder, so the performance
        // should be pretty good.
        if (!contains(path, "staging/0") || !endsWith(path, ".txt") || contains(path, "-temp"))
            return !shouldQuit();

        blknum_t unused1;
        timestamp_t unused2;
        options->fileRange.first = bnFromPath(path, unused1, unused2);

        // LOG_PROGRESS("Scanning staging", options->);
        // LOG4("Scanning ", path);
        // return options->visitBinaryFile(path, data) && !shouldQuit();
    }
    return !shouldQuit();
}
