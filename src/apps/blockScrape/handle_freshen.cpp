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

        bool needToScrape = true;
        if (fileExists(getBinaryFilename(num))) {
            readBlockFromBinary(scraper.block, getBinaryFilename(num));
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
            // Because we haven't scraped this block, but we want to write all its data, we need to re-scrape here.
            if (!needToScrape)
                scraper.scrapeBlock();
            scraper.updateAddrIndex();
            lockSection(false);
            if (!options.writeBlocks)
                ::remove(getBinaryFilename(num).c_str());

        } else {
            // We want to avoid rescraping the block if we can, so we store it here. We may delete it when the
            // block gets finalized if we're not supposed to be writing blocks
            if (!fileExists(getBinaryFilename(num))) {
                lockSection(true);
                writeBlockToBinary(scraper.block, getBinaryFilename(num));
                lockSection(false);
            }
        }

        cout << scraper.report(options.endBlock) << endl;
    }

    return true;
}
