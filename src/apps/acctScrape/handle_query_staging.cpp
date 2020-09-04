/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitStagingIndexFiles(const string_q& path, void* data) {
#if 0
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitStagingIndexFiles, data);

    } else {

        // There should only be a single file in the ./staging folder (other than
        // 0000000000-temp.txt which we skip). The file contains all blocks the scraper
        // has seen but has not yet consolidated. Here, we read the file which is
        // fixed length: [address, blockNum, txid (59 bytes)]. We read the entire file
        // into memory at once, sort it by address (the file is sorted by block) and
        // then binary search for the address we're looking for. Should be super fast.
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        if (!contains(path, "staging/0") || !endsWith(path, ".txt") || contains(path, "-temp")) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        timestamp_t unused;
        options->fileRange.first = bnFromPath(path, options->fileRange.second, unused);
        // TODO - Not right: ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that --start and --end options are ignored when scanning
        if (!rangesIntersect(options->scanRange, options->fileRange)) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        LOG4("Scanning ", path);
        LOG_PROGRESS("Scanning staging", options->fileRange.first, options->scanRange.second);
        options->stats.nChecked++;

        // if (!establishIndexChunk(indexPath))
        //   EXIT_FAIL("Could not download index chunk " + indexPath + ". Quitting...");

        size_t nRecords = fileSize(path) / 59;
        options->stats.nRecords += nRecords;
        CAppearanceArray_base items;
        items.reserve(nRecords + 100); // little bit of extra room

        CStringArray lines;
        asciiFileToLines(path, lines);
        sort(lines.begin(), lines.end());
        bool in = false;
        bool done = false;
        for (size_t i = 0 ; i < lines.size() && !done ; i++) {
            for (auto& monitor : options->monitors) {
                if (contains(lines[i], monitor.address)) {
                    CAppearance_base app;
                    CUintArray parts;
                    explode(parts, lines[i], '\t');
                    app.blk = (uint32_t)parts[1];
                    app.txid = (uint32_t)parts[2];
                    items.push_back(app);
                    in = true;
                    cerr << lines[i] << endl;
                } else {
                    cerr << lines[i] << "\r";
                    cerr.flush();
                    if (in)
                        done = true;
                }
                lockSection(true);
                if (items.size()) {
                    monitor.writeAnArray(items);
                    options->stats.nPositive++;
                } else {
                    options->stats.nSkipped++;
                }
                monitor.writeLastBlock(options->fileRange.first + 1);
                lockSection(false);
            }
        }
        //string_q result = indexHit ? " index hit " + hits : " false positive";
        //LOG_PROGRESS("Scanning", options->fileRange.first, options->scanRange.second);
        return !shouldQuit();
    }
    ASSERT(0); // should not happen
#endif
    return !shouldQuit();
}
