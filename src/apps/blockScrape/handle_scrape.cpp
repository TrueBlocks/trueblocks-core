/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

#ifdef OLD_CODE
#else
extern void finalizeIndexChunk2(COptions *options, CStringArray& stage);
extern blknum_t getLastBlockInFolder(const string_q& folder);
extern blknum_t firstUnripe(void);
#define indexFolder_raw (getCachePath("addr_index/raw/"))
#define indexFolder_unripe (getCachePath("addr_index/unripe/"))
#undef MARKER
#define MARKER 2
#define SS(a) { cerr << bBlue << padLeft(padRight((a),22,'.'),28) << cOff << "\r"; }
#define ES()  { cerr << "\t\t\t\t" << TIC() << cOff << bBlue << " ...done                      " << endl; }
#endif
//--------------------------------------------------------------------------
bool handle_scrape(COptions &options) {

#ifndef OLD_CODE
    options.maxIndexRows = 500000;

    // We do not want to be running if the user is scraping an account
    bool mustQuit = isRunning("acctScrape", false);
    if (mustQuit) {
        return false;
    }

    // Cleanup any left over files from a previous run. The 'raw' folder never has useful files. If the
    // process is exited early, the next time it starts, it will start over with the last staged file
    cleanFolder(indexFolder_raw);
    cleanFolder(indexFolder_unripe);

    // Find the last visited block (if not in staging folder, then check finalized folder) or zero if none
    blknum_t lastVisit = getLastBlockInFolder(indexFolder_staging);
    if (lastVisit == 0)
        lastVisit = getLastBlockInFolder(indexFolder_sorted); //getLastBlockInFolder(indexFolder_finalized);

    blknum_t client = getLastBlock_client();
    blknum_t unripe = firstUnripe();
    cerr << (client - unripe) << endl;
    blknum_t startBlock = lastVisit + 1;
    if (startBlock >= unripe) {
        cerr << "No ripe blocks to process" << endl;
        return false;
    }

    if (startBlock < firstTransactionBlock)
        options.nBlocks = 5000;
//    else if (ddosRange(startBlock))
//        options.nBlocks = 200;

    //2287592
    //2288192
    //2356992

    ostringstream os;
    os << "blaze scrape --startBlock " << startBlock << " --nBlocks " << options.nBlocks;
    if (options.nBlockProcesses != NOPOS)
        os << " --nBlockProcesses " << options.nBlockProcesses;
    if (options.nAddrProcesses != NOPOS)
        os << " --nAddrProcesses " << options.nAddrProcesses;
    cerr << cGreen << "\t" << os.str() << cOff << " (" << (startBlock + options.nBlocks) << ")" << endl;
    SS("Scraping");
    if (system(os.str().c_str()) != 0) {
        cerr << cRed << "\t\t\t\tBlaze quit unexpectedly. Quitting..." << cOff << endl;
        return false;
    }
    ES();

    SS("Grouping");
    string_q newLines;
    blknum_t endBlock = min(unripe, startBlock + options.nBlocks);  // we only move ripe blocks to the indexes
    for (blknum_t bn = startBlock ; bn < endBlock && !shouldQuit() && !mustQuit ; bn++) {
        lockSection(true); // no control+C during this phase
        string_q rawName = indexFolder_raw + padNum9(bn) + ".txt";
        size_t nLines = fileSize(rawName) / 59;
        if (nLines) {
            newLines.reserve((newLines.size() + nLines) * 59);
            newLines += asciiFileToString(rawName);
            cerr << "\t\t\t\t" << bn << " - " << (endBlock-bn) << "     \r";
            //mustQuit = isRunning("acctScrape", false);
        } else {
            // blaze failed for some reason (may have been control+C). Quit accumulating here
            // and start again next time.
            CBlock block;
            getBlock(block, bn);
            if (block.miner == "0x0") {
                // some early blocks had no miner by mistake, skip over them
                // put a false record here to skip over this block and quit
                ostringstream os1;
                os1 << "0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef\t" << padNum9(bn) << "\t" << padNum5(99997) << endl;
                newLines += os1.str();
                cerr << "\n" << cRed << "No miner: " << os1.str() << cOff;
            } else {
                mustQuit = true;
            }
        }
        if (fileExists(rawName))
            ::remove(rawName.c_str());
        lockSection(false); // no control+C during this phase
    }
    ES();

    // The stage now contains all non-sorted records (actually, they are sorted naturally by
    // block number). Next, we pick off a chunk if we can, finalize it, and re-write any
    // unfinalized records back to the stage (still sorted by block number)
    CStringArray newRecords;
    SS("Exploding");
    explode(newRecords, newLines, '\n');
    ES();
    finalizeIndexChunk2(&options, newRecords);
#else
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

    return true;
}

#ifndef OLD_CODE
//-------------------------------------------------------------------------------------------------------------
inline bool waitForCreate(const string_q& filename) {
    size_t mx = 1000;
    size_t cnt = 0;
    while (cnt < mx && !fileExists(filename))
        cnt++;

    return fileExists(filename);
}
//--------------------------------------------------------------------------
void finalizeIndexChunk2(COptions *options, CStringArray& stage) {

    lockSection(true);
    string_q stagingOld = getLastFileInFolder(indexFolder_staging, false);
    size_t prevSize = fileSize(stagingOld) / 59;

    if (stage.size() == 0) {
        cerr << "\t" << cTeal << "Nothing to stage. Quitting..." << cOff << endl;
        exit(0);
    }
    string_q lastLine = stage[stage.size()-1];

    CStringArray parts;
    explode(parts, lastLine, '\t');
    if (parts.size() != 3 || !isNumeral(parts[1])) {
        cerr << "\t" << cRed << "Invalid last line. Quitting..." << lastLine << cOff << endl;
        exit(0);
        return;
    }

    SS("Moving");
    string_q stagingNew = indexFolder_staging + parts[1] + ".txt";
    if (fileExists(stagingOld)) {
        ::rename(stagingOld.c_str(),stagingNew.c_str());
    }
    ES()

    SS("Growing");
    linesToAsciiFile(stagingNew, stage, true);
    ES()

    size_t curSize = fileSize(stagingNew) / 59;
    cerr << "\t" << cYellow << stagingOld << "\t" << prevSize << cOff << endl;
    cerr << "\t" << cYellow << stagingNew << "\t" << curSize << cOff << " (" << (curSize - prevSize) << ")" << endl;

    if (curSize < prevSize) {
        // Something went wrong. Remove the new file and start over
        cerr << cRed << "\tSomething went wrong. Replacing old file.";
        ::rename(stagingNew.c_str(), stagingOld.c_str());
        return;
    }

    lockSection(false);
    if (curSize > options->maxIndexRows) {

        // We need to continue to process until curSize is less than maxIndexRows. This may mean more than once
        size_t pass = 0;
        while (curSize > options->maxIndexRows && !shouldQuit()) {
            lockSection(true);
            SS("Consolodate (pass " + uint_2_Str(pass++) + ")"); cerr << endl;
            CStringArray lines;
            lines.reserve(curSize + 100);
            asciiFileToLines(stagingNew, lines);
            cerr << "\tstagingNew: " << stagingNew << ": " << lines.size() << endl;

            string_q prev;
            cerr << "\tSearching from " << (options->maxIndexRows-1) << " to " << lines.size() << endl;
            cerr << cGreen << "\t\t" << (options->maxIndexRows-1) << ": " << lines[options->maxIndexRows-1] << cOff << endl;
            cerr << cGreen << "\t\t" << (options->maxIndexRows) << ": " << lines[options->maxIndexRows] << cOff << endl;
            size_t where = 0;
            for (uint64_t record = (options->maxIndexRows-1) ; record < lines.size() && where == 0; record++) {
                CStringArray pParts;
                explode(pParts, lines[record], '\t');
                cerr << bBlue << "\t\t" << record << ": " << pParts[0] << " -- " << pParts[1] << " -- " << pParts[2] << cOff << "\r";
                if (record == lines.size() - 2)
                    cerr << endl;
                if (prev != pParts[1]) {
                    if (!prev.empty())
                        where = record - 1;
                    prev = pParts[1];
                }
            }
            if (where == 0) {
                cerr << "The weird edge case";
//                getchar();
                where = lines.size() - 1;
            }
//            if (where == NOPOS)
//                where = lines.size() - 1;
            cerr << endl;
            cerr << "\tFound break at " << where << ": " << bBlue << lines[where] << cOff << endl;

            CStringArray consolidatedLines;
            consolidatedLines.reserve(lines.size());
            cerr << "\tExtract records 0 to " << where << " (inclusive) of " << lines.size() << endl;
            cerr << cGreen << "\t\t" << 0 << ": " << lines[0] << cOff << endl;
            cerr << cGreen << "\t\t" << 1 << ": " << lines[1] << cOff << endl;
            for (uint64_t record = 0 ; record <= where ; record++) {
                if (verbose > 2) {
                    cerr << bBlue << "\t\t" << record << ": " << lines[record] << cOff << "\r";
                    if (record == where - 1)
                        cerr << endl;
                }
                consolidatedLines.push_back(lines[record]);
            }
            if (verbose > 2) {
                cerr << endl;
                cerr << bTeal << "\t\t" << (where+1) << ": " << lines[where+1] << cOff << endl;
            } else {
                cerr << bBlue << "\t\t" << (where) << ": " << lines[where] << cOff << endl;
                cerr << bTeal << "\t\t" << (where+1) << ": " << lines[where+1] << cOff << endl;
            }
            CStringArray p1;
            explode(p1, consolidatedLines[0], '\t');
            CStringArray p2;
            explode(p2, consolidatedLines[consolidatedLines.size()-1], '\t');
            string_q ll = indexFolder_sorted + p1[1] + "-" + p2[1] + ".txt";
            linesToAsciiFile(ll, consolidatedLines, true);
            cerr << "\tWrote " << bYellow << consolidatedLines.size() << cOff << " records to " << ll << endl;

            where += 1;
            CStringArray remainingLines;
            remainingLines.reserve(options->maxIndexRows+100);
            cerr << "\tExtract records " << where << " to " << lines.size() << " of " << lines.size() << endl;
            cerr << cGreen << "\t\t" << where << ": " << lines[where] << cOff << endl;
            cerr << cGreen << "\t\t" << where+1 << ": " << lines[where+1] << cOff << endl;
            for (uint64_t record = where ; record < lines.size() ; record++) {
                if (verbose > 2) {
                    cerr << bBlue << "\t\t" << record << ": " << lines[record] << cOff << "\r";
                    if (record == lines.size() - 2)
                        cerr << endl;
                }
                remainingLines.push_back(lines[record]);
            }
            if (verbose > 2)
                cerr << endl;
            else {
                cerr << bBlue << "\t\t" << (where-1) << ": " << lines[where-1] << cOff << endl;
                cerr << bTeal << "\t\t" << (where) << ": " << lines[where] << cOff << endl;
            }
            ::remove(stagingNew.c_str());
            linesToAsciiFile(stagingNew, remainingLines, true);
            cerr << "\tWrote " << bMagenta << remainingLines.size() << cOff << " records to " << stagingNew << endl;

            curSize = fileSize(stagingNew) / 59;
            lockSection(false);
            ES();
        }

    } else {
        cerr << bRed << "\tDid not consolidate: " << curSize << " of " << options->maxIndexRows << cOff << endl;
    }
}

//--------------------------------------------------------------------------
blknum_t getLastBlockInFolder(const string_q& folder) {
    string_q fileName = getLastFileInFolder(folder, false);
    fileName = substitute(fileName, folder, "");
    return str_2_Uint(fileName);
}

//--------------------------------------------------------------------------
blknum_t firstUnripe(void) {
    CBlock block;
    getBlock(block, "latest"); // the latest block at the client

    cerr << block.Format("[{BLOCKNUMBER}]\t[{TIMESTAMP}]\t[{AGE}]\t[{DATE}]") << endl;
    blknum_t eightAgo = block.blockNumber - blknum_t( ( 60 / 14. ) * 8 + 1); // eight mintes ago (client time!) rounded up
    blknum_t last = block.blockNumber - blknum_t( ( 60 / 14. ) * 2 + 1); // two mintes ago (client time!) rounded up
    block.timestamp = block.timestamp - ( 60 * 5 );  // we're looking for the block at least five minutes ago
    findTimestamp_binarySearch(block, eightAgo, last);
    cerr << block.Format("[{BLOCKNUMBER}]\t[{TIMESTAMP}]\t[{AGE}]\t[{DATE}]") << endl;
    return block.blockNumber;
}
#endif
