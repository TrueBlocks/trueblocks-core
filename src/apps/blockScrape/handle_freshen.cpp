/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool updateIndex(CArchive& fullBlockCache, blknum_t bn);
//--------------------------------------------------------------------------
bool handle_freshen(COptions& options) {

    // We open this at the start of the scrape and keep it open until the end of the scrape. Normally,
    // we'd rather open and shut the file, we're protected with lockSection and we flush every file
    // write and the file gets cleaned up on exit, so it's all good.
//TODO(tjayrush): Did this as part of the dAppNode docker hack. Otherwise, blockScrape gets stuck if dAppNode kills us
//    if (options.silent)
    ::remove((fullBlockIndex + ".lck").c_str());
    ::remove("/root/.quickBlocks/cache/fullBlocks.bin.lck");
// TODO: SEE ISSUE #1014

    CArchive fullBlockCache(WRITING_ARCHIVE);
    if (!fullBlockCache.Lock(fullBlockIndex, "a+", LOCK_WAIT)) {
        cerr << "Could not open fullBlock index: " << fullBlockCache.LockFailure() << "\n";
        return false;
    }

    // Write the header if this is the first time through
    if (!fileExists(blockCachePath("logs/block-scrape-data.log"))) {
        cerr << "\t";
        cout << "block-date\trun-date\tduration\tblockNum\tnTxs\tnTrcs\ttrcDepth\tnAddrs\tstatus\tblooms\n";
    }

    // The startBlock is the last block written to the fullBlockIndex. The lastBlock is the front of
    // the chain. Block numbers only get written to the fullBlockIndex if they are finalized, so this
    // means we will revisit blocks until they are finalized (unless we shouldQuit if user hits cntl+C)
    for (blknum_t num = options.startBlock ; num < options.endBlock && !shouldQuit() ; num++) {

        CBlock block;
        block.blockNumber = num;
        CScraperContext sCtx(&options, &block);

        // We need to be aware of re-orgs, so we only write to the fullBlock index if we're sure
        // the block is final. In this way, when we start the next scan we will start from the place
        // where we left off last time. We optionally write the binary block data to disc (this
        // defaults to true). By making this optional, we can delay writing the much larger binary
        // block data until we've identified blocks of interest if so desired. Note: if we are not
        // writing blocks the behaviour is to requery the chain until a block is final. If we are writing
        // blocks we only re-query the chain for blocks that don't exist

        string_q blockFilename = getBinaryFilename(num);
        string_q bloomFilename = substitute(blockFilename, "/blocks/", "/blooms/");
        bool goodHash = false;
        if (fileExists(blockFilename)) {
            // If the file exists, we may be able to skip re-querying the chain
            sCtx.bloomOkay = true;
            sCtx.blockOkay = true;
            readBlockFromBinary(block, blockFilename);
            readBloomArray(sCtx.bloomList, bloomFilename);

            // If it had been finalized, we wouldn't be re-reading it
            ASSERT(!block.finalized);

            // Check to see if the hash has changed
            goodHash = (block.hash == getRawBlockHash(num));

            // Check to see if its finalized
            block.finalized = isBlockFinal(block.timestamp, options.latestBlockTs, (60 * 4));

            if (goodHash) {
                // If it's the same hash, assume it's not final...
                sCtx.status = "not final";
                if (block.finalized) {
                    // ...unless it is...
                    sCtx.status = "final-a";
                    lockSection(true);
                    sCtx.blockOkay = (!options.writeBlocks || writeBlockToBinary(block, blockFilename));
                    if (block.transactions.size())
                        updateIndex(fullBlockCache, num);
                    lockSection(false);
                }
            } else {
                // This is a block re-org, so we will re-scan
                sCtx.status = "reorg-re";
            }
        }

        // If we already have a good hash, we're re-written it if it's now final, or we don't have to if it isn't.
        // If we don't have a good hash (either because we've never seen this block before or it's changed since
        // last read), we need to rescan.
        if (!goodHash) {
            bool hasTxs = sCtx.scrape(block);

            lockSection(true);
            if (hasTxs) {
                sCtx.status += "scanned";
                block.finalized = isBlockFinal(block.timestamp, options.latestBlockTs, (60 * 4));
                sCtx.updateAddrIndex();
                sCtx.blockOkay = (options.writeBlocks ? writeBlockToBinary(block, blockFilename) : true);
                sCtx.bloomOkay = writeBloomArray(sCtx.bloomList, bloomFilename);
                if (block.finalized) {
                    sCtx.status = "final-b";
                    updateIndex(fullBlockCache, num);
                }
            } else {
                // If block has no txs, we want to be able to distinguish. We always write the bloom and update index
                sCtx.status += "skipped";
                bloomFilename = substitute(bloomFilename, ".bin", "-e.bin");
                sCtx.updateAddrIndex();
                sCtx.bloomOkay = writeBloomArray(sCtx.bloomList, bloomFilename);
            }
            lockSection(false);
        }

        ASSERT((block.transactions.size() && (sCtx.blockOkay && sCtx.bloomOkay)) || (!block.transactions.size() && (!sCtx.blockOkay && !sCtx.bloomOkay)));
        cout << sCtx.report(options.endBlock) << endl;
    }

    if (fullBlockCache.isOpen())
        fullBlockCache.Release();

    return true;
}
