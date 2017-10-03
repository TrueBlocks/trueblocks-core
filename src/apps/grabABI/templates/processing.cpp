/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "parselib.h"
#include "debug.h"
#include "ncurses.h"

//-----------------------------------------------------------------------
bool CVisitor::isTransactionOfInterest(CTransaction *trans, uint32_t& whichWatch) {

    for (uint32_t i = 0; i < watches.getCount() ; i++) {
        if (trans->blockNumber >= watches[i].firstBlock && trans->blockNumber <= watches[i].lastBlock) {
            if (watches[i].isTransactionOfInterest(trans, nSigs, sigs)) {
                whichWatch = i;
                return true;
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool displayFromCache(const SFString& cacheFileName, SFUint32& blockNum, void *dataPtr) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(dataPtr);
    CBlock block;

    // If there is no cache, there's nothing to display
    if (!fileExists(cacheFileName))
        return true; // return true if we want to continue on to updateCache

    SFUint32 orig = blockNum, lastBlock = 0;

    if (visitor->cache.Lock(cacheFileName, binaryReadOnly, LOCK_NOWAIT)) {

        while (!visitor->cache.Eof()) {

            SFUint32 transID;
            uint32_t whichWatch;
            visitor->cache >> whichWatch;
            if (visitor->cache.Eof()) {
                visitor->closeIncomeStatement(block);
                visitor->cache.Release();
                return true;
            }
            visitor->cache >> blockNum;
            if (visitor->cache.Eof()) {
                visitor->closeIncomeStatement(block);
                visitor->cache.Release();
                return true;
            }
            visitor->cache >> transID;
            if (blockNum >= orig) {

                if (blockNum > lastBlock) {  // only re-read if it's a new block

                    if (!visitor->closeIncomeStatement(block)) {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        return false; // return false since user hit 'quit' on debugger
                    }

                    // If we switched blocks, read the next block
                    if (!readOneBlock_fromBinary(block, getBinaryFilename1(blockNum))) {
                        cerr << "Block read failed. Quitting cache read\r\n";
                        visitor->cache.Release();
                        return false;
                    }
                    visitor->blockStats.prevBlock = block;

                    if (!visitor->openIncomeStatement(block))  {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        return false; // return false since user hit 'quit' on debugger
                    }

                    lastBlock = blockNum;
                    if (verbose) {
                        cerr << "fromCache: " << block.blockNumber << " " << dateFromTimeStamp(block.timestamp) << "\r";
                        cerr.flush();
                    }
                }

                // this will always be true, but we just protect ourselves here
                if (transID < block.transactions.getCount()) {
                    CTransaction *trans = &block.transactions[(uint32_t)transID];
                    trans->pBlock = &block;
                    if (visitor->watches[whichWatch].status != "disabled") {
                        if (visitor->opts.trace_on || visitor->opts.logs_on)
                            cout << bGreen << SFString('-',180) << "\r\n";
                        if (visitor->opts.accounting_on || visitor->opts.trace_on)
                            getTraces(trans->traces, trans->hash);
                        visitor->accountForExtTransaction(block, trans);
                        visitor->displayTrans(trans);
                        visitor->transStats.nDisplayed++;
                        if (visitor->opts.debugger_on && !visitor->esc_hit) {
                            nodelay(stdscr, true);
                            int ch = getch();
                            visitor->esc_hit = (ch == 27 || ch == 'q');
                            if (ch == 27) // esc comes with an extra key
                                getch();
                            nodelay(stdscr, false);
                        }
                    }
                }
            }
        }

        // ignore return since we're done anway
        visitor->closeIncomeStatement(block);
        visitor->cache.Release();
    }
    return true;
}

blknum_t lastBloomHit = 0;
SFUint32 nFound = 0;
//-----------------------------------------------------------------------
bool updateCacheUsingBlooms(const SFString& path, void *dataPtr) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(dataPtr);
    if (visitor->user_hit_q)
        return false;

    if (path.endsWith("/")) {
        forAllFiles(path + "*", updateCacheUsingBlooms, dataPtr);

    } else {

        if (path.endsWith(".bin")) {
            SFString p = path.Substitute(".bin","");
            p.Reverse(); p = nextTokenClear(p, '/'); p.Reverse();
            blknum_t bloomNum = toUnsigned(p);
            if (bloomNum <= visitor->blockStats.firstBlock) {
                static blknum_t lastBucket1 = 0;
                blknum_t thisBucket1 = (bloomNum / 10000 ) * 10000;
                if (thisBucket1 != lastBucket1) {
                    cerr << "earlyExit: " << thisBucket1 << "|"
                        << visitor->bloomStats.bloomsChecked << "|"
                        << visitor->bloomStats.bloomHits << "|"
                        << visitor->bloomStats.falsePositives << "\r";
                    cerr.flush();
                    lastBucket1 = thisBucket1;
                }
                lastBloomHit = bloomNum;
                return true; // continue

            } else if (bloomNum >= visitor->blockStats.firstBlock + visitor->blockStats.nBlocks) {
                stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");
                return false; // don't continue

            } else {
                if (lastBloomHit ==0)
                    lastBloomHit = visitor->blockStats.firstBlock;
            }

            SFBloom bloom;
            visitor->bloomStats.bloomsChecked++;
            SFArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, binaryReadOnly, LOCK_NOWAIT)) {
                archive >> bloom;
                archive.Close();
            }

//            cout << "Checking bloom " << path << "\r\n";
            bool hit = false;
            for (uint32_t i = 0 ; i < visitor->watches.getCount()-1 && !hit; i++) { // don't check too many
                if (isBloomHit(makeBloom(visitor->watches[i].address), bloom)) {
                    hit = true;
                }
            }

            if (hit) {
//                cout << "Bloom hit from " << lastBloomHit << " to " << bloomNum << "\r\n";
                nFound = 0;
                for (blknum_t k = lastBloomHit ; k < bloomNum ; k++) {
                    if (fileExists(getBinaryFilename1(k))) {
//                        cout << "Checking block " << k << "\r\n";
                        CBlock block;
                        readOneBlock_fromBinary(block, getBinaryFilename1(k));
                        if (!updateCache(block, visitor)) {
                            visitor->bloomStats.bloomHits++;
                            visitor->bloomStats.falsePositives += (nFound == 0);
                            stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");
                            return false;  // don't continue, user hit 'q'
                        }
                    }
                }
//                cout << "Bloom hit at block " << bloomNum
//                        << " at address " << padNum7T(hit)
//                        << " with " << bitsTwiddled(bloom)
//                        << " bits found " << nFound << " transactions\r";
                cout.flush();
                visitor->bloomStats.bloomHits++;
                visitor->bloomStats.falsePositives += (nFound == 0);
            }
            stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");

            static blknum_t lastBucket2 = 0;
            blknum_t thisBucket2 = (bloomNum / 1000 ) * 1000;
            if (thisBucket2 != lastBucket2) {
                cout << "buckets: " << thisBucket2 << "|"
                    << visitor->bloomStats.bloomsChecked << "|"
                    << visitor->bloomStats.bloomHits << "|"
                    << visitor->bloomStats.falsePositives << "\r";
                cout.flush();
                lastBucket2 = thisBucket2;
            }
            lastBloomHit = bloomNum;
        }
    }
    return true;
}

//-----------------------------------------------------------------------
bool updateCache(CBlock& block, void *dataPtr) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(dataPtr);

    if (!visitor->openIncomeStatement(block))  {
        cerr << "Quitting debugger.\r\n";
        return false; // return false since user hit 'quit' on debugger
    }
    for (uint32_t i = 0 ; i < block.transactions.getCount() ; i++) {

        CTransaction *trans = &block.transactions[i];
        trans->pBlock = &block;

        // NEVER CHANGE THE TYPE OR SIZE OF THIS DATA!!!
        uint32_t whichWatch;
        // NEVER CHANGE THE TYPE OR SIZE OF THIS DATA!!!
        if (visitor->isTransactionOfInterest(trans, whichWatch)) {

            nFound++;

            // Display only if the user is interested in this account
            if (visitor->opts.accounting_on || visitor->opts.trace_on)
                getTraces(trans->traces, trans->hash);
            visitor->accountForExtTransaction(block, trans);

            if (visitor->watches[(uint32_t)whichWatch].status != "disabled") {
                visitor->displayTrans(trans);
                visitor->transStats.nDisplayed++;
                if (visitor->opts.debugger_on && !visitor->esc_hit) {
                    nodelay(stdscr, true);
                    int ch = getch();
                    visitor->esc_hit = (ch == 27 || ch == 'q');
                    if (ch == 27) // esc comes with an extra key
                        getch();
                    nodelay(stdscr, false);
                }
            }

            ASSERT(visitor->cache.isWriting());
            // Write the data even if we're not displaying it (flush to make sure it gets written)
            visitor->cache << whichWatch << trans->pBlock->blockNumber << trans->transactionIndex;
            visitor->cache.flush();
            visitor->transStats.nFreshened++;
        }
    }

    timestamp_t tsOut = (block.timestamp == 0 ? toTimestamp(Now()) : block.timestamp);
    SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asStringU(block.blockNumber) + ")";
    blknum_t x = (visitor->blockStats.firstBlock >= block.blockNumber ? 0 : block.blockNumber - visitor->blockStats.firstBlock);
    progressBar(x, visitor->blockStats.nBlocks, endMsg);
    visitor->blockStats.prevBlock = block;

    // Write this to the file so we know which block to start on next time the monitor is run
    stringToAsciiFile("./cache/lastBlock.txt", asStringU(block.blockNumber) + "\r\n");
    if (visitor->opts.debugger_on && !visitor->esc_hit) {
        nodelay(stdscr, true);
        int ch = getch();
        visitor->esc_hit = (ch == 27 || ch == 'q');
        if (ch == 27) // esc comes with an extra key
            getch();
        nodelay(stdscr, false);
    }
    return visitor->closeIncomeStatement(block);  // may invoke debugger, which may return false, which will stop update
}

//-----------------------------------------------------------------------
void loadWatches(const CToml& toml, CAccountWatchArray& array, const SFString& key, blknum_t& minny, blknum_t& maxxy) {

    minny = UINT32_MAX;
    maxxy = 0;

    SFString watchStr = toml.getConfigArray("watches", key, "");
    if (key == "list" && watchStr.empty()) {
        cout << "Empty list of watches. Quitting.\r\n";
        exit(0);
    }

    uint32_t cnt = 0;
    char *p = cleanUpJson((char *)watchStr.c_str());
    while (p && *p) {
        CAccountWatch watch;
        uint32_t nFields = 0;
        p = watch.parseJson(p, nFields);

        if (nFields) {
            // cleanup and report on errors
            bool okay = true;
            SFString msg;
            watch.index = cnt++;
            watch.color = convertColor(watch.color);
            watch.address = fixAddress(toLower(watch.address));
            watch.nodeBal = getBalance(watch.address, watch.firstBlock-1, false);
            if (!isAddress(watch.address)) {
                okay = false;
                msg = "invalid address " + watch.address;
            }
            if (watch.name.empty()) {
                if (!msg.empty())
                    msg += ", ";
                msg += "no name " + watch.name;
                okay = false;
            }
            if (okay) {
                minny = min(minny, watch.firstBlock);
                maxxy = max(maxxy, watch.lastBlock);
                array[array.getCount()] = watch;
            } else {
                cerr << msg << "\n";
            }
        }
    }
    return;
}

//-----------------------------------------------------------------------
blknum_t CVisitor::loadWatches(const CToml& toml) {

    blknum_t unused1 = 0, unused2 = 0;
    ::loadWatches(toml, named, "named", unused1, unused2);
    ::loadWatches(toml, watches, "list", blockStats.minWatchBlock, blockStats.maxWatchBlock);
    watches[watches.getCount()] = CAccountWatch(watches.getCount(), "Others", "Other Accts", 0, UINT32_MAX, bRed);
    return true;
}
