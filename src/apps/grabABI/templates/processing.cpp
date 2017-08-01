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

    for (int i = 0; i < watches.getCount() ; i++) {
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
bool displayFromCache(const SFString& cacheFileName, SFUint32& blockNum, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(data);
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
                    visitor->lastBlockNum = block.blockNumber;
                    if (!readOneBlock_fromBinary(block, getBinaryFilename1(blockNum))) {
                        cerr << "Block read failed. Quitting cache read\r\n";
                        visitor->cache.Release();
                        return false;
                    }

                    if (!visitor->openIncomeStatement(block))  {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        return false; // return false since user hit 'quit' on debugger
                    }

                    lastBlock = blockNum;
                    if (verbose)
                        visitor->interumReport1(block.blockNumber, block.timestamp);
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
                        visitor->accountForTransaction(block, trans);
                        visitor->lastTrans = trans;
                        visitor->nDisplayed += visitor->displayTransaction(trans);
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
bool updateCacheUsingBlooms(const SFString& path, void *data) {

    if (path.endsWith("/")) {
        forAllFiles(path + "*", updateCacheUsingBlooms, data);

    } else {

        CVisitor *visitor = reinterpret_cast<CVisitor*>(data);
        if (path.endsWith(".bin")) {
            SFString p = path.Substitute(".bin","");
            p.Reverse(); p = nextTokenClear(p, '/'); p.Reverse();
            blknum_t bloomNum = toUnsigned(p);
            if (bloomNum <= visitor->startBlock) {
                static blknum_t lastBucket1 = 0;
                blknum_t thisBucket1 = (bloomNum / 10000 ) * 10000;
                if (thisBucket1 != lastBucket1) {
                    cerr << "earlyExit: " << thisBucket1 << "|"
                        << visitor->bloomStats.bloomsChecked << "|"
                        << visitor->bloomStats.bloomHits << "|"
                        << visitor->bloomStats.falsePositives << "|"
                        << (qbNow() - visitor->bloomStats.startTime) << "\r";
                    cerr.flush();
                    lastBucket1 = thisBucket1;
                }
                lastBloomHit = bloomNum;
                return true; // continue
            }
            if (bloomNum >= visitor->startBlock + visitor->nBlocksToVisit) {
                stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");
                return false; // don't continue
            }

            SFBloom bloom;
            visitor->bloomStats.bloomsChecked++;
            SFArchive archive(true, curVersion, true);
            if (archive.Lock(path, binaryReadOnly, LOCK_NOWAIT)) {
                archive >> bloom;
                archive.Close();
            }

//            cout << "Checking bloom " << path << "\r\n";
            bool hit = false;
            for (int i = 0 ; i < visitor->watches.getCount()-1 && !hit; i++) { // don't check too many
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
                        updateCache(block, visitor);
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
                    << visitor->bloomStats.falsePositives << "|"
                    << (qbNow() - visitor->bloomStats.startTime) << "\r";
                cout.flush();
                lastBucket2 = thisBucket2;
            }
            lastBloomHit = bloomNum;
        }
    }
    return true;
}

//-----------------------------------------------------------------------
bool updateCache(CBlock& block, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(data);

    if (!visitor->openIncomeStatement(block))  {
        cerr << "Quitting debugger.\r\n";
        return false; // return false since user hit 'quit' on debugger
    }
    for (int i = 0 ; i < block.transactions.getCount() ; i++) {

        CTransaction *trans = &block.transactions[i];
        trans->pBlock = &block;

        uint32_t whichWatch;
        if (visitor->isTransactionOfInterest(trans, whichWatch)) {

            nFound++;

            // Display only if the user is interested in this account
            if (visitor->opts.accounting_on || visitor->opts.trace_on)
                getTraces(trans->traces, trans->hash);
            visitor->accountForTransaction(block, trans);

            if (visitor->watches[whichWatch].status != "disabled") {
                visitor->lastTrans = trans;
                visitor->nDisplayed += visitor->displayTransaction(trans);
                if (visitor->opts.debugger_on && !visitor->esc_hit) {
                    nodelay(stdscr, true);
                    int ch = getch();
                    visitor->esc_hit = (ch == 27 || ch == 'q');
                    if (ch == 27) // esc comes with an extra key
                        getch();
                    nodelay(stdscr, false);
                }
            }

            ASSERT(!visitor->cache.m_isReading);
            // Write the data even if we're not displaying it (flush to make sure it gets written)
            visitor->cache << whichWatch << trans->pBlock->blockNumber << trans->transactionIndex;
            visitor->cache.flush();
            visitor->nFreshened++;
        }
    }

    if (true) { //verbose) {
        timestamp_t tsOut = (block.timestamp == 0 ? toTimeStamp(Now()) : block.timestamp);
        SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asString(block.blockNumber) + ")";
        visitor->interumReport(block.blockNumber, block.timestamp, endMsg);
    }

    // Write this to the file so we know which block to start on next time the monitor is run
    stringToAsciiFile("./cache/lastBlock.txt", asStringU(block.blockNumber) + "\r\n");
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
            watch.address = toLower(watch.address);
            watch.color = convertColor(watch.color);
            if (!watch.address.startsWith("0x"))
                watch.address = "0x" + watch.address;
            watch.nodeBal = getBalance(watch.address, watch.firstBlock-1, false);
            if (watch.address.length() != 42) {
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
    ::loadWatches(toml, watches, "list", minWatchBlock, maxWatchBlock);
    watches[watches.getCount()] = CAccountWatch(watches.getCount(), "Others", "Other Accts", 0, UINT32_MAX, bRed);
    return true;
}
