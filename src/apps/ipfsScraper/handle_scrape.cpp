/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------
bool COptions::handle_scrape(void) {
    // Remove anything that may be residule from the last run. (For example, if the user hit control+c)
    ::remove((indexFolder_staging + "000000000-temp.txt").c_str());
    cleanFolder(indexFolder_unripe);

    // Find the last visited block. (It's the later of ripe, staging, or finalized.)
    blknum_t unused1, ripe, staging, finalized, client;
    getLatestBlocks(unused1, ripe, staging, finalized, client);
    // FIX_THIS_CODE
    client = 10480200;
    blknum_t lastVisit = max(ripe, max(staging, finalized));
    blknum_t startBlock = lastVisit + 1;

    // In some cases (the user is re-syncing his/her node from scratch, but has a partial
    // index) and, therefore, the index may be ahead of tip of the chain. In this case, we
    // return without doing anything...
    if (startBlock > client) {
        LOG_INFO("The index (", startBlock, ") is caught up to the chain (", client, ").");
        return false;
    }

    // Docker will kill blaze if it uses too many resources...
    if (getEnvStr("DOCKER_MODE") != "true") {
        // ...so we only override on n_blocks to speed things up if not docker...
        if (startBlock < 450000) {
            n_blocks = max((blknum_t)4000, n_blocks);

        } else if (ddosRange(startBlock)) {
            // ...or slow things down...
            n_blocks = getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 200);
        }
    }
    // FIX_THIS_CODE
    n_blocks = 100;

    // If a block is more than 28 blocks from the head we consider it 'ripe.' Once a block
    // goes ripe, we no longer ask the node about it. We try to move it to staging. Staging
    // means it's ready to be consolidated (finalized). 28 blocks is an arbitrarily chosen value,
    // but is a bit more than six minutes under normal operation ((14 * 28) / 60 == 6.5). If the
    // index is near the head of the chain and the difficulty level is high (the time bomb is
    // exploding), the time will extend, but the final nature is the same.
    blknum_t ripeBlock = client - 28;
    if (client < 28)
        ripeBlock = 0;

    // Now we want to do a final adjustment to n_blocks to make sure it doesn't go past the
    // last synced block at the client
    if ((startBlock + n_blocks) > client) {
        ASSERT(startBlock <= client);  // see above
        n_blocks = (client - startBlock);
    }

    // FIX_THIS_CODE
    string_q cmmd = "/Users/jrush/Development/trueblocks-core/bin/blaze scrape";
    // We're ready to scrape, so build the blaze command line...
    ostringstream os;
    os << cmmd << " --startBlock " << startBlock << " --nBlocks " << n_blocks;
    if (n_block_procs != 20)
        os << " --nBlockProcs " << n_block_procs;
    if (n_addr_procs != 60)
        os << " --nAddrProcs " << n_addr_procs;

    // reporting...
    cerr << cGreen << "\t" << os.str() << cOff << " (" << (startBlock + n_blocks) << ")" << endl;

    os << " --ripeBlock " << ripeBlock;
    // ...and make the call to blaze.
    if (system(os.str().c_str()) != 0) {
        // if blaze returns non-zero, it did not complete sucessfully. We need to remove files
        // in the 'ripe' folder because they're inconsistent (blaze's 'go' processes do not run
        // in sequence). We blindly clean all ripe files, which is a bit of overill, but it's easy
        // and it works. Next time we run, it will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        cerr << cRed << "\tBlaze quit without finishing. Reprocessing..." << cOff << endl;
        return false;
    }

    // If blaze returned '0', it has sucessfully run through all the blocks between 'startBlock'
    // and 'startBlock + n_blocks'. The ripe folder contains individual files, one for each block,
    // containing lists of addresses that appear in that block. The unripe folder holds blocks that
    // are less than 28 blocks old. We do nothing further with them here, but the query tool (acctScrape)
    // may use them. acctScrape is responsible to report to the user that the unripe data may be
    // unsafe to use. Here, we quit if acctScrape is running. We can quit safely without cleaning
    // up. The next run will pick up where we left off.
    if (isRunning("acctScrape")) {
        LOG_WARN("acctScrape is running. blockScrape will re-run in a moment...");
        return false;
    }

    // cout << indexFolder_staging << ": " << folderExists(indexFolder_staging) << endl;
    // cout << indexFolder_ripe << ": " << folderExists(indexFolder_ripe) << endl;
    // cout << indexFolder_unripe << ": " << folderExists(indexFolder_unripe) << endl;

    // From this point until the end of the invocation, the processing must be able to stop abruptly
    // without resulting in corrupted data (control+c for example). This means we must process a single
    // file at a time in sequential order. Processing means moving files from a ripe file into the staging
    // file and then (if applicable) from the staging file into a finalized chunk. If we stop processing
    // at any point, we want to leave the data in a state where the next invocation of this program
    // can either clean up or pick up where it left off.

    // Next, we processes one file in the ripe folder at a time by appending its data to a temporary
    // staging file. We flush the data after each append and then remove the ripe file. If this process
    // is interrupted, we clean up both the ripe folder and the temporary staging file. This may be
    // over kill, but it's safer. In effect, we start over at the most recently successfully staged block
    // the next time through. The value `prevBlock` points to the last completed staging block. The last
    // block in the file is ../staged/{prevBlock}.txt. We're also processing time stamps (which can get
    // ahead of the scraper) at the same time.

    // Pick up the latest of zero, finalized, or staging...
    blknum_t p = (staging == NOPOS ? (finalized == NOPOS ? 0 : finalized) : staging);

    // Carries state we need to process the blocks
    CConsolidator cons(p);
    if (!cons.tmp_file.is_open()) {
        cerr << cRed << "\tCould not open temporary staging file." << cOff << endl;
        return false;
    }

    // Spin through 'ripe' files and process as we go. Note: it's okay to allow the timestamp file to get
    // ahead of the staged blocks. We only write when the block number is a new one not already in the file.
    if (!forEveryFileInFolder(indexFolder_ripe, copyRipeToStage, &cons)) {
        // Something went wrong with copying the ripe blocks to staging. (i.e. the user hit control+c or we
        // encountered a non-sequential list of files). We clean up and start over the next time through.
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        cons.tmp_file.close();
        ::remove(cons.tmp_fn.c_str());
        return false;
    }
    cons.tmp_file.close();

    // The stage now contains all non-consolidated records. Ripe should be empty. All files are closed.

    // Next, we try to pick off chunks of 500,000 records (maxIndexRows) if we can, consolidate them (write
    // them to a binary relational table), and re-write any unfinalized records back onto the stage. Again, if
    // anything goes wrong we need clean up and leave the data in a recoverable state.
    cerr << bTeal << "\t  attempting to consolidate blocks" << cOff << endl;
    if (!finalize_chunks(&cons)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }

    return true;
}

extern bool appendFile(const string_q& toFile, const string_q& fromFile);
// FIX_THIS_CODE
static const blknum_t maxIndexRows = 50;
//--------------------------------------------------------------------------
bool COptions::finalize_chunks(CConsolidator* cons) {
    // 'oldStage' contains staged but not yet consolidated records. 'tempStage' contains
    // newly scraped ripe blocks. 'newStage' is empty. This temporary file is where we will put
    // the records from these two files. We need to process in a way that will allow for interruption.

    // ...we want to work with a temporary file so if anything goes wrong, we can recover...
    string_q tmpFile = getIndexPath("temp.txt");
    string_q oldStage = getLastFileInFolder(indexFolder_staging, false);
    string_q tempStage = cons->tmp_fn;
    string_q newStage = indexFolder_staging + padNum9(cons->prevBlock) + ".txt";
// FIX_THIS_CODE
#define FF(a)                                                                                                          \
    { cerr << setw(12) << #a << setw(70) << a << ": " << cYellow << (fileSize(a) / 59) << cOff << endl; }
#define FF1(a) cerr << bBlue << (a) << cOff << endl << endl;
    // #define FF(a)
    // #define FF1(a)
    FF(tmpFile);
    FF(oldStage);
    FF(tempStage);
    FF(newStage);
    FF1("Prior to anything");

    if (oldStage == newStage) {
        blknum_t curSize = fileSize(newStage) / 59;
        cerr << bTeal;
        cerr << "\t  nothing new to process." << endl;
        cerr << "\t  will consolidate in " << (maxIndexRows - curSize);
        cerr << " rows (" << curSize << " of " << maxIndexRows << ")" << cOff << endl;
        return true;
    }

    if (oldStage != tempStage) {
        if (!appendFile(tmpFile, oldStage)) {
            // oldStage is still valid. Caller will clean up the rest
            cerr << cRed << "Could not append oldStage to temp.fil" << cOff << endl;
            return false;
        }
        FF(tmpFile);
        FF(oldStage);
        FF(tempStage);
        FF(newStage);
        FF1("Append oldStage to tmpFile");
    }

    // ...next we append the new ripe records if we can...
    if (!appendFile(tmpFile, tempStage)) {
        // oldStage is still valid. Caller will clean up the rest
        ::remove(tmpFile.c_str());
        cerr << cRed << "Could not append tempStage to temp.fil" << cOff << endl;
        return false;
    }
    FF(tmpFile);
    FF(oldStage);
    FF(tempStage);
    FF(newStage);
    FF1("Appending tempStage to tmpFile");

    // ...finally, we move the temp file to the new stage without allowing user to hit control+c
    lockSection(true);
    ::rename(tmpFile.c_str(), newStage.c_str());
    if (oldStage != tempStage && oldStage != newStage)
        ::remove(oldStage.c_str());
    ::remove(tempStage.c_str());
    lockSection(false);
    FF(tmpFile);
    FF(oldStage);
    FF(tempStage);
    FF(newStage);
    FF1("Renaming tmp to new, removing old and temp");

    // We are now in a valid state with all records in the properly named newStage
    blknum_t curSize = fileSize(newStage) / 59;

    // Did user hit control+c?
    if (shouldQuit())
        return true;

    // If we don't have enough records to consolidate, tell the user and return...
    if (curSize <= maxIndexRows) {
        cerr << bTeal << "\t  will consolidate in " << (maxIndexRows - curSize);
        cerr << " rows (" << curSize << " of " << maxIndexRows << ")" << cOff << endl;
        return true;
    }

    // Process until curSize is less than maxIndexRows. This may mean more than one pass
    size_t pass = 0;
    while (curSize > maxIndexRows && !shouldQuit()) {
        lockSection(true);
        cerr << bBlue << "\tconsolidate (pass " + uint_2_Str(pass++) + ")" << endl;
        CStringArray lines;
        lines.reserve(curSize + 100);
        asciiFileToLines(newStage, lines);
        cerr << "\tnewStage: " << newStage << ": " << lines.size() << endl;

        string_q prev;
        cerr << "\tsearching from " << (maxIndexRows - 1) << " to " << lines.size() << endl;
        cerr << cGreen << "\t\t" << (maxIndexRows - 1) << ": " << lines[maxIndexRows - 1] << cOff << endl;
        cerr << cGreen << "\t\t" << (maxIndexRows) << ": " << lines[maxIndexRows] << cOff << endl;
        size_t where = 0;
        for (uint64_t record = (maxIndexRows - 1); record < lines.size() && where == 0; record++) {
            CStringArray pParts;
            explode(pParts, lines[record], '\t');
            cerr << bBlue << "\t\t" << record << ": " << pParts[0] << " -- " << pParts[1] << " -- " << pParts[2] << cOff
                 << "\r";
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
            where = lines.size() - 1;
        }
        cerr << endl;
        cerr << "\tfound break at " << where << ": " << bBlue << lines[where] << cOff << endl;

        CStringArray consolidatedLines;
        consolidatedLines.reserve(lines.size());
        cerr << "\textract records 0 to " << where << " (inclusive) of " << lines.size() << endl;
        cerr << cGreen << "\t\t" << 0 << ": " << lines[0] << cOff << endl;
        cerr << cGreen << "\t\t" << 1 << ": " << lines[1] << cOff << endl;
        for (uint64_t record = 0; record <= where; record++) {
            if (verbose > 2) {
                cerr << bBlue << "\t\t" << record << ": " << lines[record] << cOff << "\r";
                cerr.flush();
                if (record == where - 1)
                    cerr << endl;
            }
            if (countOf(lines[record], '\t') != 2) {
                cerr << "Found a record with less than two tabs. Quitting..." << endl;
                if (record > 0)
                    cerr << bBlue << "empty line follows: " << lines[record - 1] << cOff << endl;
                cerr << bBlue << "empty line: [" << lines[record] << "]" << cOff << endl;
                if (record < where)
                    cerr << bBlue << "empty line preceeds: " << lines[record + 1] << cOff << endl;
                return false;
            }
            consolidatedLines.push_back(lines[record]);
        }

        if (verbose > 2) {
            cerr << endl;
            cerr << bTeal << "\t\t" << (where + 1) << ": " << lines[where + 1] << cOff << endl;
        } else {
            cerr << bBlue << "\t\t" << (where) << ": " << lines[where] << cOff << endl;
            cerr << bTeal << "\t\t" << (where + 1) << ": " << lines[where + 1] << cOff << endl;
        }
        CStringArray p1;
        explode(p1, consolidatedLines[0], '\t');
        CStringArray p2;
        explode(p2, consolidatedLines[consolidatedLines.size() - 1], '\t');

        sort(consolidatedLines.begin(), consolidatedLines.end());
        string_q binFile = indexFolder_finalized + p1[1] + "-" + p2[1] + ".bin";
        writeIndexAsBinary(binFile, consolidatedLines);
        cerr << "\twrote " << bYellow << consolidatedLines.size() << cOff << " records to " << binFile << endl;

        where += 1;
        CStringArray remainingLines;
        remainingLines.reserve(maxIndexRows + 100);
        cerr << "\textract records " << where << " to " << lines.size() << " of " << lines.size() << endl;
        cerr << cGreen << "\t\t" << where << ": " << lines[where] << cOff << endl;
        cerr << cGreen << "\t\t" << where + 1 << ": " << lines[where + 1] << cOff << endl;
        for (uint64_t record = where; record < lines.size(); record++) {
            if (verbose > 2) {
                cerr << bBlue << "\t\t" << record << ": " << lines[record] << cOff << "\r";
                cerr.flush();
                if (record == lines.size() - 2)
                    cerr << endl;
            }
            remainingLines.push_back(lines[record]);
        }
        if (verbose > 2) {
            cerr << endl;
        } else {
            cerr << bBlue << "\t\t" << (where - 1) << ": " << lines[where - 1] << cOff << endl;
            cerr << bTeal << "\t\t" << (where) << ": " << lines[where] << cOff << endl;
        }
        ::remove(newStage.c_str());
        linesToAsciiFile(newStage, remainingLines, true);
        cerr << "\twrote " << bMagenta << remainingLines.size() << cOff << " records to " << newStage << endl;

        curSize = fileSize(newStage) / 59;
        lockSection(false);
    }
    return true;
}

//--------------------------------------------------------------------------
bool copyRipeToStage(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", copyRipeToStage, data);

    } else {
        blknum_t e_unused;
        timestamp_t ts;
        blknum_t bn = bnFromPath(path, e_unused, ts);
        CConsolidator* con = reinterpret_cast<CConsolidator*>(data);
        // If we're not one behind, we have a problem
        if ((con->prevBlock + 1) != bn) {
            // For some reason, we're missing a file. Quit and try again next time
            cerr << cRed << "Current file (" << path << ") does not sequentially follow previous file "
                 << con->prevBlock << "." << cOff << endl;
            //            cerr << "Press enter to continue: ";
            //            getchar();
            return false;
        }
        ifstream inputStream(path, ios::in);
        if (!inputStream.is_open()) {
            // Something went wrong, try again next time
            cerr << cRed << "Could not open input stream " << path << cOff << endl;
            return false;
        }
        lockSection(true);
        con->tmp_file << inputStream.rdbuf();
        con->tmp_file.flush();
        inputStream.close();
        ::remove(path.c_str());
        con->prevBlock = bn;
        lockSection(false);
    }

    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool appendFile(const string_q& toFile, const string_q& fromFile) {
    ofstream output;
    output.open(toFile, ios::out | ios::app);
    if (!output.is_open())
        return false;

    ifstream input(fromFile, ios::in);
    if (!input.is_open()) {
        output.close();
        return false;
    }

    output << input.rdbuf();
    output.flush();
    input.close();

    return true;
}

//--------------------------------------------------------------------------
CConsolidator::CConsolidator(blknum_t p) {
    prevBlock = p;
    tmp_fn = indexFolder_staging + "000000000-temp.txt";
    tmp_file.open(tmp_fn, ios::out | ios::trunc);
}
