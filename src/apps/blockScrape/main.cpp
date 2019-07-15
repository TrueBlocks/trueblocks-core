/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define SS(a) { cerr << bBlue << padLeft(padRight((a),22,'.'),75) << cOff << "\r"; cerr.flush(); }

//--------------------------------------------------------------------------
extern bool waitForCreate(const string_q& filename);
extern void finalizeIndexChunk2(COptions *options, CStringArray& stage);

//--------------------------------------------------------------------------
bool handle_scrape(COptions &options) {

    // Do not run if someone is searching the index
    if (isRunning("acctScrape", false))
        return false;

    // Remove any blocks that were not 'ripe' the last time we ran
    cleanFolder(indexFolder_unripe);

    // Find the last visited block (it's either in staging or finalized)
    blknum_t unused1, ripe, staging, finalized, client;
    getLastBlocks(unused1, ripe, staging, finalized, client);
    blknum_t lastVisit = max(ripe, max(staging, finalized));
    blknum_t startBlock = lastVisit + 1;

    // In some cases (if the user is re-syncing his/her node from scratch for example) the index may be
    // ahead of tip of the chain. In this case, we return without processign anything.
    if (startBlock > client) {
        cerr << cTeal << "INFO: " << cOff;
        cerr << "The index (" << startBlock << ") is caught up to the tip of the chain (" << client << ").";
        cerr << endl;
        return false;
    }

    if (startBlock < firstTransactionBlock)
        options.nBlocks = 5000;
    else if (ddosRange(startBlock))
        options.nBlocks = 200;
    if (getEnvStr("DOCKER_MODE") == "true") {
        if (ddosRange(startBlock)) {
            options.nBlocks = 100;
            options.nBlockProcs = 5;
            options.nAddrProcs = 20;
        }
    }

#if 0
    //2287592
    //2288192
    //2356992
#endif

    // At some point, we need to stop re-visiting blocks. We call this point the 'ripe' block. Here,
    // the 'ripe' block is 28 blocks before the current head (a bit more than five minutes under normal
    // operation). Note that, if the index is caught up to the head and the difficulty is high (we're
    // in the time bomb), we will receed from the front of the chain than normal. (28 blocks, when blocks
    // take longer, is a longer amount of time.)
    blknum_t ripeBlock = client - 28;

    ostringstream os;
    os << "blaze scrape";
    os << " --startBlock " << startBlock;
    os << " --nBlocks " << options.nBlocks;
    os << " --nBlockProcs " << options.nBlockProcs;
    os << " --nAddrProcs " << options.nAddrProcs;

    cerr << endl << cTeal << "\t" << Now().Format(FMT_EXPORT) << endl;
    cerr << cGreen << "\t" << os.str() << cOff << " (" << (startBlock + options.nBlocks) << ")" << endl;

    // We don't need to show ripeBlock...
    os << " --ripeBlock " << ripeBlock;
    if (system(os.str().c_str()) != 0) {
        cerr << cRed << "\tBlaze quit without finishing. Reprocessing..." << cOff << endl;
        return false;
    }

#if 0
    SS("Grouping");
    string_q newLines;
    blknum_t endBlock = min(unripe, startBlock + options.nBlocks);  // we only move ripe blocks to the indexes
    bool mustQuit = isRunning("acctScrape", false);
    for (blknum_t bn = startBlock ; bn < endBlock && !shouldQuit() && !mustQuit ; bn++) {
        lockSection(true); // no control+C during this phase
        string_q rawName = indexFolder_unripe + padNum9(bn) + ".txt";
        size_t nLines = fileSize(rawName) / 59;
        if (nLines) {
            newLines.reserve((newLines.size() + nLines) * 59);
            newLines += asciiFileToString(rawName);
            cerr << "\t\t\t\t" << bn << " - " << (endBlock-bn) << "     \r"; cerr.flush();
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
#endif

    return true;
}

//-------------------------------------------------------------------------------------------------------------
bool waitForCreate(const string_q& filename) {
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

    SS("Growing");
    linesToAsciiFile(stagingNew, stage, true);

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
    blknum_t maxIndexRows = 500000;
    if (curSize > maxIndexRows) {

        // We need to continue to process until curSize is less than maxIndexRows. This may mean more than once
        size_t pass = 0;
        while (curSize > maxIndexRows && !shouldQuit()) {
            lockSection(true);
            SS("Consolodate (pass " + uint_2_Str(pass++) + ")"); cerr << endl;
            CStringArray lines;
            lines.reserve(curSize + 100);
            asciiFileToLines(stagingNew, lines);
            cerr << "\tstagingNew: " << stagingNew << ": " << lines.size() << endl;

            string_q prev;
            cerr << "\tSearching from " << (maxIndexRows-1) << " to " << lines.size() << endl;
            cerr << cGreen << "\t\t" << (maxIndexRows-1) << ": " << lines[maxIndexRows-1] << cOff << endl;
            cerr << cGreen << "\t\t" << (maxIndexRows) << ": " << lines[maxIndexRows] << cOff << endl;
            size_t where = 0;
            for (uint64_t record = (maxIndexRows-1) ; record < lines.size() && where == 0; record++) {
                CStringArray pParts;
                explode(pParts, lines[record], '\t');
                cerr << bBlue << "\t\t" << record << ": " << pParts[0] << " -- " << pParts[1] << " -- " << pParts[2] << cOff << "\r";
                cerr.flush();
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
                    cerr << bBlue << "\t\t" << record << ": " << lines[record] << cOff << "\r"; cerr.flush();
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


            //            string_q ll = indexFolder_sorted + p1[1] + "-" + p2[1] + ".txt";
            //            linesToAsciiFile(ll, consolidatedLines, true);
            //            cerr << "\tWrote " << bYellow << consolidatedLines.size() << cOff << " records to " << ll << endl;
            //            string_q asciiFile = substitute(ll, indexFolder_sorted, indexFolder_ascii);
            //            writeIndexAsAscii(asciiFile, consolidatedLines);
            //            cerr << "\tWrote " << bYellow << consolidatedLines.size() << cOff << " records to " << asciiFile << endl;
            string_q binFile = indexFolder_finalized + p1[1] + "-" + p2[1] + ".txt";
            //            string_q binFile = substitute(substitute(ll, indexFolder_sorted, indexFolder_finalized),".txt",".bin");
            writeIndexAsBinary(binFile, consolidatedLines);
            cerr << "\tWrote " << bYellow << consolidatedLines.size() << cOff << " records to " << binFile << endl;

            where += 1;
            CStringArray remainingLines;
            remainingLines.reserve(maxIndexRows+100);
            cerr << "\tExtract records " << where << " to " << lines.size() << " of " << lines.size() << endl;
            cerr << cGreen << "\t\t" << where << ": " << lines[where] << cOff << endl;
            cerr << cGreen << "\t\t" << where+1 << ": " << lines[where+1] << cOff << endl;
            for (uint64_t record = where ; record < lines.size() ; record++) {
                if (verbose > 2) {
                    cerr << bBlue << "\t\t" << record << ": " << lines[record] << cOff << "\r"; cerr.flush();
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
        }

    } else {
        cerr << bRed << "\tDid not consolidate: " << curSize << " of " << maxIndexRows << cOff << endl;
    }
}

//----------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init("local",  defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (options.parseArguments(command)) {
            //                       cerr << bGreen << "Scraping new blocks..." << "\n" << cOff;
            handle_scrape(options);
            //                       cerr << bGreen << "...done\n" << cOff;
        }
    }

    //    if (verbose)
    //        cerr << scraperStatus();

    etherlib_cleanup();

    return 0;
}

