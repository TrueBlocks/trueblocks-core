/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

// OLD_CODE - SET THIS BACK
#if 0
#define MAX_ROWS 174
#define CLIENT 600
#define N_BLOCKS 100
#define SNAP_TO_GRID 75
#else
#define MAX_ROWS 2000000
#define CLIENT (progress.client + 0)
#define N_BLOCKS (n_blocks + 0)
#define SNAP_TO_GRID 100000
#endif

#define LOG_MARKER(l)                                                                                                  \
    LOG_INFO("\n");                                                                                                    \
    LOG_INFO(string_q((l), '-'));

//--------------------------------------------------------------------------
bool COptions::scrape_blocks(void) {
    ENTER("scrape_blocks");

    LOG_MARKER(120);

    // We want to re-evaluate our progress each time we loop.

    // First, get the progress (i.e. highest block) of the client and the each of index caches
    CBlockProgress progress = getBlockProgress();
    progress.client = CLIENT;

    // From there, we can determine where to start the scraper (one more than the largest cache)
    blknum_t startBlock = max(progress.ripe, max(progress.staging, progress.final)) + 1;

    // In some cases, for example, the index is ahead of the tip because we're re-syncing...do nothing...
    if (startBlock > progress.client) {
        ostringstream os;
        os << "The index (" << startBlock << ") is caught up to the tip of the chain (" << progress.client << ").";
        LOG_INFO(os.str());
        EXIT_NOMSG(false);
    }

    // Make a few adjustments here in the non-docker case to speed things up a bit
    if (!isDockerMode()) {
        if (startBlock < 450000) {
            n_blocks = max((blknum_t)4000, n_blocks);

        } else if (ddosRange(startBlock)) {
            // ...or slow things down...
            n_blocks = getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 200);
        }
    }
    n_blocks = N_BLOCKS;

    // If a block is more than 28 blocks from the head we consider it 'ripe.' Once a block
    // goes ripe, we no longer ask the node about it. We try to move it to staging. Staging
    // means it's ready to be consolidated (finalized). 28 blocks is an arbitrarily chosen value,
    // but is a bit more than six minutes under normal operation ((14 * 28) / 60 == 6.5). If the
    // index is near the head of the chain and the difficulty level is high (the time bomb is
    // exploding), the time will extend, but the final nature is the same.
    blknum_t ripeBlock = (progress.client < 28 ? 0 : progress.client - 28);

    // Now we want to do a final adjustment to n_blocks to make sure it doesn't go past the
    // last synced block at the client
    if ((startBlock + n_blocks) > progress.client) {
        ASSERT(startBlock <= progress.client);  // see above
        n_blocks = (progress.client - startBlock);
    }

    if (shouldQuit()) {
        LOG_WARN("User quit...");
        EXIT_NOMSG(false);
    }

    // Tell the user what's going on...
    LOG_INFO(cGreen, "blaze scrape (", (progress.client - startBlock), " blocks from head)", cOff);

    // We're ready to scrape, so build the blaze command line...
    ostringstream os;
    os << "blaze scrape ";
    os << "--startBlock " << startBlock << " ";
    os << "--nBlocks " << n_blocks << " ";
    os << "--nBlockProcs " << n_block_procs << " ";
    os << "--nAddrProcs " << n_addr_procs << " ";
    os << " --ripeBlock " << ripeBlock << " ";
    LOG_CALL(os.str());
    if (system(os.str().c_str()) != 0) {
        // blaze will return non-zero if it fails. In this case, we need to remove files in the 'ripe'
        // folder because they're inconsistent (blaze's 'go' processes do not run in sequence). We
        // blindly clean all ripe files, which is a bit of overill, but it's easy and it works.
        // Next time we run, it will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        LOG_WARN("Blaze quit without finishing. Reprocessing...");
        EXIT_NOMSG(false);
    }

    // Blaze succeeded, but the user may have started `acctExport` since it was called. We don't want
    // to get incorrect results, so we return here knowing it will get cleaned up next time.
    if (isRunning("acctExport")) {
        LOG_WARN("acctExport is running. blockScrape will re-run in a moment...");
        EXIT_NOMSG(false);
    }

    // Blaze sucessfullly created individual files, one for each block between 'start' and 'start + n_blocks'.
    // Each contains a list of addresses that appear in that block. The unripe folder holds blocks that
    // are less than 28 blocks old. We do nothing further with them, but the query tool (acctExport)
    // may use them if it wants to.

    // From this point until the end of this invocation, the processing must be able to stop abruptly
    // without resulting in corrupted data (control+c for example). This means we must process a single
    // file at a time in sequential order. Processing means moving files from ripe to staging and then
    // (if applicable) from the staging file into a finalized chunk. If we stop processing at any
    // point, we want to leave the data in a state where the next invocation of this program can either
    // clean up or pick up where it left off.

    // Next, we processes one file in the ripe folder at a time by appending its data to a temporary
    // staging file. We flush the data after each append and then remove the ripe file. If this process
    // is interrupted, we clean up both the ripe folder and the temporary staging file. This may be
    // over kill, but it's safer. In effect, we start over at the most recently successfully staged block
    // the next time through. The value `prevBlock` points to the last completed staging block. The last
    // block in the file is ../staged/{prevBlock}.txt. We're also processing time stamps (which can get
    // ahead of the scraper) at the same time.

    // Pick up the latest of zero, finalized, or staging...
    blknum_t p = (progress.staging == NOPOS ? (progress.final == NOPOS ? 0 : progress.final) : progress.staging);

    // Carries state we need to process the blocks
    CConsolidator cons(p);
    if (!cons.tmp_file.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        EXIT_NOMSG(false);
    }

    // Spin through 'ripe' files and process as we go. Note: it's okay to allow the timestamp file to get
    // ahead of the staged blocks. We only write when the block number is a new one not already in the file.
    if (!forEveryFileInFolder(indexFolder_ripe, visitCopyRipeToStage, &cons)) {
        // Something went wrong with copying the ripe blocks to staging. (i.e. the user hit control+c or we
        // encountered a non-sequential list of files). We clean up and start over the next time through.
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        cons.tmp_file.close();
        ::remove(cons.tmp_fn.c_str());
        EXIT_NOMSG(false);
    }
    cons.tmp_file.close();

    // The stage now contains all non-consolidated records. Ripe should be empty. All files are closed.

    // Next, we try to pick off chunks of MAX_ROWS  records if we can, consolidate them (write
    // them to a binary relational table), and re-write any unfinalized records back onto the stage. Again, if
    // anything goes wrong we need clean up and leave the data in a recoverable state.
    if (!finalize_chunks(&cons)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }
    EXIT_NOMSG(true);
}

//--------------------------------------------------------------------------
bool COptions::finalize_chunks(CConsolidator* cons) {
    // 'blaze' is finished scraping (and extracting addresses from) new blocks from the chain. Those appearance records
    // are stored in a file called "XXX";
    // 'oldStage' contains staged but not yet consolidated records. 'tempStage' contains
    // newly scraped ripe blocks. 'newStage' is empty. This temporary file is where we will put
    // the records from these two files. We need to process in a way that will allow for interruption.

    // ...we want to work with a temporary file so if anything goes wrong, we can recover...
    string_q tmpFile = getIndexPath("temp.txt");
    string_q oldStage = getLastFileInFolder(indexFolder_staging, false);
    string_q tempStage = cons->tmp_fn;
    string_q newStage = indexFolder_staging + padNum9(cons->prevBlock) + ".txt";
    LOG8("");
    LOG8("tmpFile:   ", substitute(tmpFile, getIndexPath(""), "$INDEX/"));
    LOG8("oldStage:  ", substitute(oldStage, getIndexPath(""), "$INDEX/"));
    LOG8("tempStage: ", substitute(tempStage, getIndexPath(""), "$INDEX/"));
    LOG8("newStage:  ", substitute(newStage, getIndexPath(""), "$INDEX/"));

    if (oldStage == newStage) {
        blknum_t curSize = fileSize(newStage) / 59;
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_INFO(cYellow, "  No new blocks. Have ", curSize, " records of ", MAX_ROWS, ". Need ", (MAX_ROWS - curSize),
                 " more.", cOff);
        LOG8("Going to sleep since we're basically caught up.");
        mode = "quit";
        changeState();
        return true;
    }

    if (oldStage != tempStage) {
        if (!appendFile(tmpFile, oldStage)) {
            // oldStage is still valid. Caller will clean up the rest
            LOG_ERR("Could not append oldStage to temp.fil.");
            return false;
        }
    }

    // ...next we append the new ripe records if we can...
    if (!appendFile(tmpFile, tempStage)) {
        // oldStage is still valid. Caller will clean up the rest
        ::remove(tmpFile.c_str());
        LOG_ERR("Could not append tempStage to temp.fil.");
        return false;
    }

    // ...finally, we move the temp file to the new stage without allowing user to hit control+c
    lockSection(true);
    ::rename(tmpFile.c_str(), newStage.c_str());
    if (oldStage != tempStage && oldStage != newStage)
        ::remove(oldStage.c_str());
    ::remove(tempStage.c_str());
    lockSection(false);

    // We are now in a valid state with all records in the properly named newStage
    blknum_t curSize = fileSize(newStage) / 59;

    // Did user hit control+c?
    if (shouldQuit())
        return true;

    // If we don't have enough records to consolidate, tell the user and return...
    if (curSize <= MAX_ROWS) {
        LOG_INFO(" ");
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_INFO(cYellow, "  Have ", curSize, " records of ", MAX_ROWS, ". Need ", (MAX_ROWS - curSize), " more.",
                 cOff);
        return true;
    }

    // Process until curSize is less than MAX_ROWS. This may mean more than one pass
    size_t pass = 0;
    while (curSize > MAX_ROWS && !shouldQuit()) {
        lockSection(true);

        CStringArray lines;
        lines.reserve(curSize + 100);
        asciiFileToLines(newStage, lines);

        LOG_INFO(" ");
        LOG_INFO(bBlue, "Consolidation pass ", pass++, cOff);
        LOG_INFO(cWhite, "  Starting search at record ", (MAX_ROWS - 1), " of ", lines.size(), cOff);
        if (verbose > 2) {
            LOG_INFO(cGreen, "\t", (MAX_ROWS - 1), ": ", lines[MAX_ROWS - 1], cOff);
            LOG_INFO(cGreen, "\t", (MAX_ROWS), ": ", lines[MAX_ROWS], cOff);
        }

        size_t where = 0;
        string_q prev;
        for (uint64_t record = (MAX_ROWS - 1); record < lines.size() && where == 0; record++) {
            CStringArray pParts;
            explode(pParts, lines[record], '\t');
            if (verbose > 2 && (record == lines.size() - 2)) {
                LOG_INFO(bBlue, "\t", record, ": ", pParts[0], " -- ", pParts[1], " -- ", pParts[2], cOff);
            }
            if (prev != pParts[1]) {
                if (!prev.empty())
                    where = record - 1;
                prev = pParts[1];
            }
        }
        if (where == 0) {
            LOG_INFO("The weird edge case");
            where = lines.size() - 1;
        }

        LOG_INFO(cWhite, "  Found a break at line ", where, " extracting records 0 to ", where, " (inclusive) of ",
                 lines.size(), cOff);
        if (verbose > 2) {
            LOG_INFO(cGreen, "\t", 0, ": ", lines[0], cOff);
            LOG_INFO(cGreen, "\t", 1, ": ", lines[1], cOff);
            LOG_INFO(bBlue, "\t", where, ": ", lines[where], cOff);
            LOG_INFO(bTeal, "\t", (where + 1), ": ", lines[where + 1], cOff);
        }

        CStringArray consolidatedLines;
        consolidatedLines.reserve(lines.size());
        for (uint64_t record = 0; record <= where; record++) {
            if (countOf(lines[record], '\t') != 2) {
                LOG_WARN("Found a record with less than two tabs.");
                LOG_WARN("preceeding line:\t[", ((record > 0) ? lines[record - 1] : "N/A"), "]");
                LOG_WARN("offending line:\t[", lines[record], "]");
                LOG_WARN("following line:\t[", ((record < where) ? lines[record + 1] : "N/A"), "]");
                return false;
            }
            consolidatedLines.push_back(lines[record]);
        }

        CStringArray p1;
        explode(p1, consolidatedLines[0], '\t');
        CStringArray p2;
        explode(p2, consolidatedLines[consolidatedLines.size() - 1], '\t');

        sort(consolidatedLines.begin(), consolidatedLines.end());
        string_q chunkId = p1[1] + "-" + p2[1];
        string_q chunkPath = indexFolder_finalized + chunkId + ".bin";
        CPinnedItem pinRecord;
        writeIndexAsBinary(chunkPath, consolidatedLines, (pin ? visitToPin : nullptr), &pinRecord);

        LOG_INFO(cWhite, "  Wrote ", consolidatedLines.size(), " records to ",
                 substitute(chunkPath, indexFolder_finalized, "$FINAL/"), cOff);

        where += 1;
        CStringArray remainingLines;
        remainingLines.reserve(MAX_ROWS + 100);

        if (verbose > 2) {
            LOG_INFO(cWhite, "  Extracting records ", where, " to ", lines.size(), " of ", lines.size(), cOff);
            LOG_INFO(cGreen, "\t", where, ": ", lines[where], cOff);
            LOG_INFO(cGreen, "\t", (where + 1), ": ", lines[where + 1], cOff);
            LOG_INFO(bBlue, "\t", (where - 1), ": ", lines[where - 1], cOff);
            LOG_INFO(bTeal, "\t", (where), ": ", lines[where], cOff);
        }

        for (uint64_t record = where; record < lines.size(); record++) {
            remainingLines.push_back(lines[record]);
        }

        ::remove(newStage.c_str());
        linesToAsciiFile(newStage, remainingLines);
        LOG_INFO(cWhite, "  Wrote ", remainingLines.size(), " records to ",
                 substitute(newStage, indexFolder_staging, "$STAGING/"), cOff);

        curSize = fileSize(newStage) / 59;
        lockSection(false);
    }
    return true;
}

//--------------------------------------------------------------------------
bool visitCopyRipeToStage(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitCopyRipeToStage, data);

    } else {
        blknum_t e_unused;
        timestamp_t ts;
        blknum_t bn = bnFromPath(path, e_unused, ts);

        // If we're not one behind, we have a problem
        CConsolidator* con = reinterpret_cast<CConsolidator*>(data);
        if ((con->prevBlock + 1) != bn) {
            // For some reason, we're missing a file. Quit and try again next time
            LOG_WARN("Current file (", path, ") does not sequentially follow previous file ", con->prevBlock, ".");
            return false;
        }

        ifstream inputStream(path, ios::in);
        if (!inputStream.is_open()) {
            // Something went wrong, try again next time
            LOG_WARN("Could not open input stream ", path);
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

//---------------------------------------------------------------------------------------------------
bool visitToPin(const string_q& chunkId, void* data) {
    LOG_INFO("I am going to pin ", chunkId);
    ASSERT(data);
    // CPinnedItem pinRecord = *(CPinnedItem*)data;
    // pinChunk(chunkId, pinRecord);
    return !shouldQuit();
}
