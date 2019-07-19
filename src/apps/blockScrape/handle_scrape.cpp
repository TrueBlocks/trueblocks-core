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

/*
do we handle the case where 0x00000...000000 represents as 0x0
statistics reporting
maxIndexRows = getGlobalConfig("blockScrape")->getConfigInt("settings", "maxIndexRows", 350000);
marked miners (99999) but not uncle miners (99998) and not non-miners (99997)
we no longer 'snap to grid'
firstBlock is removed as an option since we always start where we last left off
endBlock is removed as an option since we always use nBlocks
maxBlocks was renamed to nBlocks
the program should only run once
we used to report on the fact that the index was ahead of the chain
check that acctScrape is not running at every spin
stats: traceCount, maxTraceDepth, nAddrsInBlock;
we used to do timing results
we used to store blocks in a binary cache - no longer - not even an option
#define dashIfNA(val) (status == "cache" ? "-" : uint_2_Str((val)))
    ostringstream os;
    os.precision(4);
    cerr << ((block.finalized ? greenCheck : whiteStar) + " ");
    cerr << padRight(uint_2_Str(last - block.blockNumber), 4) << ": ";
    os << fixed << setprecision(3);
    os << bBlack;
    os << Now().Format(FMT_EXPORT) << "\t";
    os << ts_2_Date(block.timestamp).Format(FMT_EXPORT) << "\t";
    os << (double(options->latestBlockTs - block.timestamp) / 60.) << "\t";
    os << TIC()                       << "\t" << cYellow;
    os << block.blockNumber           << "\t";
    os << block.transactions.size()   << "\t";
    os << dashIfNA(traceCount)        << "\t";
    os << dashIfNA(maxTraceDepth)     << "\t";
    os << dashIfNA(nAddrsInBlock)     << "\t";
    os << dashIfNA(addrList.addrTxMap->size()) << "\t";
    os << padNum6(curLines)           << "\t" << cTeal;
    os << status                      << cOff;
    if (status == "final" && !(block.blockNumber % SIZE_REPORT)) { // every 1,000 blocks we write extra informat
        string_q path = getGlobalConfig("")->getConfigStr("settings", "parityPath", "");
        string_q res = substitute(doCommand("du -k -d0 " + path), "\t", " ");
        os << "\text: " << options->latestBlockNum << "\t" << (options->latestBlockNum - block.blockNumber) << "\t" << nextTokenClear(res, ' ');
    }
    
    return os.str();
}
*/

#endif
