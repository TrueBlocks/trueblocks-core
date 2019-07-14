/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/

#ifdef OLD_CODE
if (startBlock > client)
cerr << cTeal << "INFO: " << cOff << "The scraper is at the front of the chain." << endl;
return startBlock <= client;
//
//  Determine most recently finalized block (lastFinal)
//      'startBlock' = lastFinal + 1
//      'endBlock' is front of chain at start of run or startBlock + maxBlocks
//
//  For each block between startBlock and endBlock
//      Assume we need to scrape the block
//      If block is in binary cache (we've seen it before)
//          Read the block from cache (faster than querying the node)
//          Query node for block hash only
//          If hashes are different
//              We need to re-scan this block
//      Else
//          We need to scan this block
//
//      If we need to scan the block
//          Scan the block
//
//      If the block is final (i.e. five minutes old)
//          If we do not have a full block, scrape the block here
//          If the block has no transations
//              Remove it from the cache
//          Else if we are not storing blocks permanantly (default is to not store blocks)
//              Remove the block from cache
//          Else if the block is not in the cache, but we're writing blocks
//              Write the block to the cache
//          Write the finalized block to the index
//          If the index is 'big enough'
//              Sort the index
//              Compress the index
//              Create the bloom
//              (Optionally) store the index and the bloom in IPFS
//      Else
//          Write the non-final block to non-final index
//          Write the block to the binary cache (it may be removed later)

bool acctScrRunning = isRunning("acctScrape", false);
for (blknum_t num = options.startBlock ; num < options.endBlock && !shouldQuit() && !acctScrRunning ; num++) {
    CScraper scraper(&options, num);
    scraper.status = "scan";
    bool needToScrape = true;

    string_q fn = getBinaryCacheFilename(CT_BLOCKS, num);
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

    scraper.block.finalized = isBlockFinal(scraper.block.timestamp, options.latestBlockTs);
    if (scraper.block.finalized) {
        scraper.status = "final";
        lockSection(true);
        // If we haven't scraped yet, we need to scrape it here
        if (!needToScrape)
            scraper.scrapeBlock();
        // Process the block cache...
        if (scraper.block.transactions.size() == 0) {
            // We never keep empty blocks
            ::remove(fn.c_str());
        } else if (!options.writeBlocks) {
            // If we're not writing blocks, remove this one
            if (fileExists(fn.c_str()))
                ::remove(fn.c_str());
        } else if (!fileExists(fn)) {
            // We may not yet have written this block (it was final the first time we saw it), so write it
            writeBlockToBinary(scraper.block, fn);
        }
        if (!scraper.addToStagingList()) {
            lockSection(false);
            return false;
        }
        lockSection(false);

    } else {
        // We want to avoid rescraping the block if we can, so we store it here. We may delete it when the
        // block gets finalized if we're not supposed to be writing blocks

        // TODO(tjayrush): Should I be writing this block to binary even if it exists (especially if it was a rescan)
        scraper.addToPendingList();
        if (!fileExists(fn)) {
            lockSection(true);
            writeBlockToBinary(scraper.block, fn);
            lockSection(false);
        }
    }

    cout << scraper.report(options.endBlock) << endl;
    acctScrRunning = isRunning("acctScrape", false);
}
#endif
