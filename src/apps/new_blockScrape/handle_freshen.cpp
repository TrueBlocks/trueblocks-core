/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------------------
bool handle_freshen(COptions& options) {

#if 0
    class CAddressRecord {
    public:
        address_t address;
        size_t    offset;
        size_t    nRecs;
        CAddressRecord(void) : address(""), offset(NOPOS), nRecs(NOPOS) { }
    };
    typedef vector<CAddressRecord> CAddressRecordArray;

    class CAppearRecord {
    public:
        blknum_t bn;
        blknum_t tx_id;
        CAppearRecord(void) : bn(NOPOS), tx_id(NOPOS) { }
    };
    typedef vector<CAppearRecord> CAppearRecordArray;

    class CIndexFile {
    public:
        CAddressRecordArray addrs;
        CAppearRecordArray  appears;
    };
#endif

#if 0
    // Open the file at the start of the scrape and keep it open until the end of the scrape.
    // Normally, we'd rather open and close the file with each write, but we're protected with
    // lockSection here and we flush on every write. Plus the open file gets closed when
    // the program quits, we should be okay.
    CArchive fullBlockCache(WRITING_ARCHIVE);
    if (!fullBlockCache.Lock(fullBlockIndex, modeWriteAppend, LOCK_WAIT)) {
        cerr << "Could not open fullBlock index: " << fullBlockCache.LockFailure() << "\n";
        return false;
    }

//    // Write the header if this is the first time through
//    if (!fileExists(blockCachePath("logs/block-scrape-data.log"))) {
//        cerr << "\t";
//        cout << "block-date\trun-date\tduration\tblockNum\tnTxs\tnTrcs\ttrcDepth\tnAddrs\tstatus\tblooms\n";
//    }
#endif

    // 'startBlock' is the most recent block written to the fullBlockIndex. 'lastBlock' is the
    // front of the chain. Block numbers only get written to the fullBlockIndex if they are
    // final. This means we revisit blocks until they are final (unless we shouldQuit)
    for (blknum_t num = options.startBlock ; num < options.endBlock && !shouldQuit() ; num++) {

 #if 0
        CBlock block;
        block.blockNumber = num;
        CScraperContext sCtx(&options, &block);
        sCtx.scrape(block);
        // We need to be aware of re-orgs, so we only write to the fullBlock index if we're never
        // going to revisit this block again (we can't be 100% certain, but we choose a value).
        // In this way, each time we start a scan we start from the block after the most recent
        // finalized block.
        //
        // We optionally write the binary block data to disc (defaults to true). By making block
        // caching optional, we are able to delay writing the much larger binary block data until
        // we've identified blocks of interest if so desired. Note: If we are not writing blocks
        // the behaviour of the following code is to re-query the chain until the block goes final.
        // If we are writing blocks we query the chain for blocks that don't already exist or have
        // a different hash (i.e. the chain has re-orged).
        string_q blockFilename = getBinaryFilename(num);
        string_q bloomFilename = substitute(blockFilename, "/blocks/", "/blooms/");
        bool goodHash = false;
        if (fileExists(blockFilename)) {
            // If the file exists, we may be able to skip re-querying the chain
            sCtx.bloomOkay = true;
            sCtx.blockOkay = true;
            readBlockFromBinary(block, blockFilename);
            readBloomFromBinary(sCtx.bloomList, bloomFilename);

            // If this block was final, we wouldn't be re-reading it. Just double checking
            ASSERT(!block.finalized);

            // Find out if it's finalized
            block.finalized = isBlockFinal(block.timestamp, options.latestBlockTs, (60 * 4));

            // Check the hash to see if it's changed. If it hasn't we can write the block and note
            // it in the full block index
            goodHash = (block.hash == getRawBlockHash(num));
            if (goodHash) {
                if (block.finalized) {
                    // ...unless it is...
                    sCtx.status = "final-a";
                    lockSection(true);
                    sCtx.blockOkay = (!options.writeBlocks || writeBlockToBinary(block, blockFilename));
                    if (block.transactions.size())
                        updateFullBlockIndex(fullBlockCache, num);
                    lockSection(false);
                } else {
                    // Same hash, but still not final
                    sCtx.status = "not final";
                }
            } else {
                // This is a block re-org, we need to re-scan...
                sCtx.status = "reorg-re";
            }
        }

        // If we have a good hash and the block is final, we will have already written its data.
        // If the block is not final, or we don't have to re-write it if we still have a good hash
        // If we have a bad hash (either because we've never seen this block before or its hash has
        // changed since the last time we scanned it), we need to re-scan and re-write.
        if (!goodHash) {
            bool hasTxs = sCtx.scrape(block);

            lockSection(true);

            if (hasTxs) {
                sCtx.status += "scanned";
                block.finalized = isBlockFinal(block.timestamp, options.latestBlockTs, (60 * 4));
                sCtx.blockOkay = (options.writeBlocks ? writeBlockToBinary(block, blockFilename) : true);
                sCtx.bloomOkay = writeBloomArray(sCtx.bloomList, bloomFilename);
                if (block.finalized) {
                    sCtx.status = "final-b";
                    updateFullBlockIndex(fullBlockCache, num);
                }
            } else {
                // If block has no txs, we do not write to the fullBlock index. Nor do we write the
                // block data. We add '-e' to the bloom filename so we know to skip over it when (and if
                // we are re-building the fullBlockIndex. We write the bloom for every block.
                sCtx.status += "skipped";
                bloomFilename = substitute(bloomFilename, ".bin", "-e.bin");
                sCtx.bloomOkay = writeBloomArray(sCtx.bloomList, bloomFilename);
            }

            // If we're keeping the address index, we update that for every block
            sCtx.updateAddrIndex();

            lockSection(false);
        }

        // we write blooms in either case
        ASSERT(( block.transactions.size() && ( sCtx.blockOkay && sCtx.bloomOkay)) ||
               (!block.transactions.size() && (!sCtx.blockOkay && sCtx.bloomOkay)));
#endif

//        // report on this block
//        cout << sCtx.report(options.endBlock) << endl;
    }

//    if (fullBlockCache.isOpen())
//        fullBlockCache.Release();

    return true;
}
