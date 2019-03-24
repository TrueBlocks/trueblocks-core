/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------------------
bool handle_freshen(COptions& options) {

    //
    //  The final block index stores finalized blocks
    //      'startBlock' is first non-final block
    //      'endBlock' is current front of chain
    //
    //  For each non-final block
    //      Assume we need to scan the block
    //      If we've scanned the block before (note: if we're not storing blocks, this will not happen
    //          read the hash and timestamp from a file
    //          If the hash has not changed
    //              we do not need to re-scan
    //
    //      Scan if we need to
    //
    //      If the block is final
    //          write the finalized block
    //          update the address index
    //          update the final block index

    timestamp_t ageFinal = (60 * 4);
    for (blknum_t num = options.startBlock ; num < options.endBlock && !shouldQuit() ; num++) {

        CScraper scraper(&options, num);
        scraper.status = "scan";

        string_q fn = getBinaryFilename(num);
        bool needToScrape = true;
        if (fileExists(fn)) {
            readBlockFromBinary(scraper.block, fn);
            if (scraper.block.hash != getRawBlockHash(num)) {
                needToScrape = true;
                scraper.status = "rescan";
            } else {
                needToScrape = false;
                scraper.status = "cache";
            }
        }

        if (needToScrape)
            scraper.scrapeBlock();

        scraper.block.finalized = isBlockFinal(scraper.block.timestamp, options.latestBlockTs, ageFinal);
        if (scraper.block.finalized) {
            scraper.status = "final";
            lockSection(true);
            // We need the block's data, but we haven't re-scraped, so we need to rescrape here.
            if (!needToScrape)
                scraper.scrapeBlock();
            // Process the block cache...
            if (scraper.block.transactions.size() == 0) {
                // We never keep empty blocks
                ::remove(fn.c_str());
            } else if (!options.writeBlocks) {
                // If we're not writing blocks, remove this one
                ::remove(fn.c_str());
            } else if (!fileExists(fn)) {
                // We may not yet have written this block (it was final the first time we saw it), so write it
                writeBlockToBinary(scraper.block, fn);
            }
            if (!scraper.finalizeList())
                return false;
            lockSection(false);

        } else {
            // We want to avoid rescraping the block if we can, so we store it here. We may delete it when the
            // block gets finalized if we're not supposed to be writing blocks
            scraper.stageList();
            if (!fileExists(fn)) {
                lockSection(true);
                writeBlockToBinary(scraper.block, fn);
                lockSection(false);
            }
        }

        cout << scraper.report(options.endBlock) << endl;
    }

    return true;
}
