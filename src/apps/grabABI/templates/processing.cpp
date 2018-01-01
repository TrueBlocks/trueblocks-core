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
bool displayFromCache(const SFString& cacheFileName, uint64_t& blockNum, void *dataPtr) {

    CVisitor *visitor = reinterpret_cast<CVisitor*>(dataPtr);
    CBlock block;

    // If there is no cache, there's nothing to display
    if (!fileExists(cacheFileName))
        return true; // return true if we want to continue on to updateCache

    uint64_t orig = blockNum, lastBlock = 0;

    if (visitor->cache.Lock(cacheFileName, binaryReadOnly, LOCK_NOWAIT)) {

        while (!visitor->cache.Eof()) {

            uint64_t transID;
            uint32_t whichWatch;
            visitor->cache >> whichWatch;
            if (visitor->cache.Eof()) {
                visitor->closeIncomeStatement(block);
                visitor->cache.Release();
                blockNum++; // the next block to process
                return true;
            }
            visitor->cache >> blockNum;
            if (visitor->cache.Eof()) {
                visitor->closeIncomeStatement(block);
                visitor->cache.Release();
                blockNum++; // the next block to process
                return true;
            }
            visitor->cache >> transID;
            if (blockNum >= orig) {

                if (blockNum > lastBlock) {  // only re-read if it's a new block

                    if (!visitor->closeIncomeStatement(block)) {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        blockNum++; // the next block to process
                        return false; // return false since user hit 'quit' on debugger
                    }

                    // If we switched blocks, read the next block
                    if (!readOneBlock_fromBinary(block, getBinaryFilename(blockNum))) {
                        cerr << "Read of block " << blockNum << " failed. Quitting cache read\r\n";
                        visitor->cache.Release();
                        blockNum++; // the next block to process
                        return false;
                    }
                    visitor->blockStats.prevBlock = block;

                    if (!visitor->openIncomeStatement(block))  {
                        cerr << "Quitting debugger.\r\n";
                        visitor->cache.Release();
                        blockNum++; // the next block to process
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
                        visitor->displayTrans(whichWatch,trans);
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
        blockNum++; // the next block to process
    }
    return true;
}

uint64_t nFound = 0;
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
                blknum_t thisBucket1 = (bloomNum / 997 ) * 997;
                if (thisBucket1 != lastBucket1) {
//                    cerr << "earlyExit: " << thisBucket1 << "|"
//                        << visitor->bloomStats.bloomsChecked << "|"
//                        << visitor->bloomStats.bloomHits << "|"
//                        << visitor->bloomStats.falsePositives << "\r";
//                    cerr.flush();
                    SFString endMsg = " (" + asStringU(bloomNum) + ")";
                    blknum_t x = (visitor->blockStats.firstBlock >= bloomNum ? 0 : bloomNum - visitor->blockStats.firstBlock);
                    progressBar(x, visitor->blockStats.nBlocks, visitor->opts.monitorName+"|"+endMsg);
                    lastBucket1 = thisBucket1;
                }
                return true; // continue

            } else if (bloomNum >= visitor->blockStats.firstBlock + visitor->blockStats.nBlocks) {
                stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");
                return false; // don't continue
            }

            SFBloomArray blooms;
            visitor->bloomStats.bloomsChecked++;
            SFArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, binaryReadOnly, LOCK_NOWAIT)) {
//                cerr << "Reading bloom: " << path << "\n";
                archive >> blooms;
//                for (uint32_t j = 0 ; j < blooms.getCount() ; j++) {
//                    cerr << bitsTwiddled(blooms[j]) << "\n";
//                }
                archive.Close();
            }

            SFBloom whichBloom;SFAddress whichAddr;
//            cout << "Checking bloom " << path << "\r\n";
            bool hit = false;
            for (uint32_t j = 0 ; j < blooms.getCount() && !hit ; j++) {
                for (uint32_t i = 0 ; i < visitor->watches.getCount()-1 && !hit; i++) { // don't check too many
//                    cerr << cYellow << "    Checking " << visitor->watches[i].address << " against bloom " << path << "\n" << cOff;
                    if (isBloomHit(makeBloom(visitor->watches[i].address), blooms[j])) {
//                        cerr << bTeal << "    Hit\n" << cOff;
                        whichBloom = blooms[j];
                        whichAddr = visitor->watches[i].address;
                        hit = true;
                    }
                }
            }

            if (hit) {
//                cout << "Bloom hit at " << bloomNum << "\r\n";
                nFound = 0;
                if (fileExists(getBinaryFilename(bloomNum))) {
//                    cout << "Checking block " << bloomNum << "\r\n";
                    CBlock block;
                    readOneBlock_fromBinary(block, getBinaryFilename(bloomNum));
                    if (!updateCache(block, visitor)) {
                        visitor->bloomStats.bloomHits++;
                        visitor->bloomStats.falsePositives += (nFound == 0);
                        stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");
                        return false;  // don't continue, user hit 'q'
                    }
                }
//                cout << "Bloom hit at block " << bloomNum
//                        << " at address " << whichAddr
//                        << " with " << bitsTwiddled(whichBloom)
//                        << " bits found " << nFound << " transactions\r";
                cout.flush();
                visitor->bloomStats.bloomHits++;
                visitor->bloomStats.falsePositives += (nFound == 0);
            }
            stringToAsciiFile("./cache/lastBlock.txt", asStringU(bloomNum) + "\r\n");

            static blknum_t lastBucket2 = 0;
            blknum_t thisBucket2 = (bloomNum / 97 ) * 97;
            if (thisBucket2 != lastBucket2) {
//                cout << "buckets: " << thisBucket2 << "|"
//                    << visitor->bloomStats.bloomsChecked << "|"
//                    << visitor->bloomStats.bloomHits << "|"
//                    << visitor->bloomStats.falsePositives << "\r";
//                cout.flush();
                SFString endMsg = " (" + asStringU(bloomNum) + ")";
                blknum_t x = (visitor->blockStats.firstBlock >= bloomNum ? 0 : bloomNum - visitor->blockStats.firstBlock);
                progressBar(x, visitor->blockStats.nBlocks, visitor->opts.monitorName+"|"+endMsg);
                lastBucket2 = thisBucket2;
            }
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
                visitor->displayTrans(whichWatch,trans);
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

//    SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asStringU(block.blockNumber) + ")";
    SFString endMsg = " (" + asStringU(block.blockNumber) + ")";
    blknum_t x = (visitor->blockStats.firstBlock >= block.blockNumber ? 0 : block.blockNumber - visitor->blockStats.firstBlock);
    progressBar(x, visitor->blockStats.nBlocks, visitor->opts.monitorName+"|"+endMsg);
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

    SFString watchStr = toml.getConfigStr("watches", key, "");
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
            watch.id = cnt++;
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

//-----------------------------------------------------------------------
CAccountWatch *CVisitor::findWatch(SFAddress addr) {
    for (uint32_t i = 0 ; i < watches.getCount() ; i++)
        if (watches[i].address == addr)
            return &watches[i];
    return NULL;
}

//-----------------------------------------------------------------------
class CTemp {
public:
    uint64_t blockNum, transID;
    uint32_t whichWatch;
};
typedef SFArrayBase<CTemp> SFTempArray;

//-----------------------------------------------------------------------
uint32_t CVisitor::checkForImport(void) {
    if (!fileExists("./caxche/import.txt")) {
//        cerr << "No import found.\n";
        return 0;
    }

    SFTempArray imp;
    uint32_t cnt = 0;
    blknum_t currentLast = toLongU(asciiFileToString("./cache/lastBlock.txt"));
    blknum_t lastBlock   = 0;
    SFString contents = asciiFileToString("./cache/import.txt");
    while (!contents.empty()) {
        SFString line    = nextTokenClear(contents,'\n');
        SFString val = nextTokenClear(line,'\t');
        SFAddress address = toAddress (val);
        CAccountWatch *watch = findWatch(address);
        if (watch) {
            CTemp tmp;
            tmp.whichWatch = watch->id;
            val = nextTokenClear(line,'\t');
            tmp.blockNum = toUnsigned(val);
            val = nextTokenClear(line,'\t');
            tmp.transID = toUnsigned(val);
            imp[imp.getCount()] = tmp;
            lastBlock = max(lastBlock, tmp.blockNum);
            cnt++;
        }
    }

    for (uint32_t i = 0 ; i < imp.getCount() ; i++) {
        cout << imp[i].whichWatch << " : " << imp[i].blockNum << " : " << imp[i].transID << "\n";
    }

    if (lastBlock > currentLast)
        stringToAsciiFile("./cache/lastBlock.txt", asStringU(lastBlock) + "\r\n");

    return cnt;
}
