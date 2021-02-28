/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------
class CConsolidator : public CBlockProgress {
  public:
    ofstream tmp_file;
    string_q tmp_fn;
    blknum_t prevBlock;
    blknum_t blazeStart;
    blknum_t blazeRipeBlock;
    blknum_t blazeNBlocks;
    explicit CConsolidator(const CBlockProgress& progress);

  private:
    CConsolidator(void) {
    }
};

// Note: We want to re-evaluate our progress each time we loop, so don't move this to parseOptions
//--------------------------------------------------------------------------
bool COptions::scrape_blocks(void) {
    ENTER("scrape_blocks");

    LOG4(" ");
    LOG4(" ");
    LOG4(string_q(120, '-'));
    LOG4(string_q(120, '-'));

    // These defines give us control over the scrape when we're testing. A bit cludgy, but effective.
#define CLIENT (isBlockScrapeTest() ? (80 + (runs)) : (progress.client))
#define N_BLOCKS (isBlockScrapeTest() ? 16 : progress.blazeNBlocks)
#define MAX_ROWS (isBlockScrapeTest() ? 13 : 2000000)
#define SNAP_TO_GRID (isBlockScrapeTest() ? 12 : 100000)
#define TEST_RUNS 3
    // 80, 16, 13

    static blknum_t runs = 0;  // this counter is used for texting purposes only
    if (isBlockScrapeTest() && runs++ > TEST_RUNS)
        defaultQuitHandler(0);

    // First, we need to know how far along in the scrape we are. We get the progress (i.e. highest
    // block) of the client and the each of index caches.
    CConsolidator progress(getBlockProgress());

    if (true) {  // !isBlockScrapeTest()
        // From there, we can determine where to start the scraper (one more than the largest cache)
        progress.blazeStart = max(progress.ripe, max(progress.staging, progress.finalized)) + 1;

        progress.client = CLIENT;
        progress.blazeNBlocks = n_blocks;

        // Make a few adjustments here in the non-docker case to speed things up a bit
        if (!isDockerMode()) {
            if (progress.blazeStart < 450000) {
                progress.blazeNBlocks = max((blknum_t)4000, progress.blazeNBlocks);

            } else if (ddosRange(progress.blazeStart)) {
                // ...or slow things down...
                progress.blazeNBlocks =
                    getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 200);
            }
        }
        progress.blazeNBlocks = N_BLOCKS;

        // If a block is more than 28 blocks from the head we consider it 'ripe.' Once a block goes
        // ripe, we no longer ask the node about it. We try to move it to staging. Staging means the
        // block is ready to be consolidated (finalized). 28 blocks is an arbitrarily chosen value,
        // but is a bit more than six minutes under normal operation ((14 * 28) / 60 == 6.5). If the
        // index is near the head of the chain and the difficulty level is high (the time bomb is
        // exploding), the time will extend, but the final nature of the operation is the same.
        progress.blazeRipeBlock = (progress.client < 28 ? 0 : progress.client - 28);

        // One final adjustment to nBlocks so we don't run past the tip of the chain
        if ((progress.blazeStart + progress.blazeNBlocks) > progress.client) {
            ASSERT(blazeStart <= progress.client);  // see above
            progress.blazeNBlocks = (progress.client - progress.blazeStart);
        }
    }

    LOG4("bs.unripe:         ", progress.unripe);
    LOG4("bs.ripe:           ", progress.ripe);
    LOG4("bs.staging:        ", progress.staging);
    LOG4("bs.finalized:      ", progress.finalized);
    LOG4("bs.client:         ", progress.client);
    LOG4("bs.blazeStart:     ", progress.blazeStart);
    LOG4("bs.blazeNBlocks:   ", progress.blazeNBlocks);
    LOG4("bs.n_block_procs:  ", n_block_procs);
    LOG4("bs.n_addr_procs:   ", n_addr_procs);
    LOG4("bs.blazeRipeBlock: ", progress.blazeRipeBlock);
    LOG4("bs.MAX_ROWS:       ", MAX_ROWS);

    // In some cases, for example, the index is ahead of the tip because we're re-syncing...do nothing...
    if (progress.blazeStart > progress.client) {
        ostringstream os;
        os << "The index (" << progress.blazeStart << ") is at the tip of the chain (" << progress.client << ").";
        LOG_INFO(os.str());
        // Returning false just means this round didn't complete. Loop continues.
        EXIT_NOMSG(false);
    }

    if (shouldQuit()) {
        LOG_WARN("The user hit control+C...");
        EXIT_NOMSG(false);
    }

    // Tell the user what's going on...
    LOG_INFO(cGreen, "blaze scrape (", (progress.client - progress.blazeStart), " blocks from head)", cOff);

    // We're ready to scrape, so build the blaze command line...
    ostringstream os;
    os << "env TB_INDEXPATH=\"" << getIndexPath("") << "\" ";
    os << "blaze scrape ";
    os << "--startBlock " << progress.blazeStart << " ";
    os << "--nBlocks " << progress.blazeNBlocks << " ";
    os << "--nBlockProcs " << n_block_procs << " ";
    os << "--nAddrProcs " << n_addr_procs << " ";
    os << "--ripeBlock " << progress.blazeRipeBlock << " ";
    os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "");
    LOG_CALL(os.str());
    if (system(os.str().c_str()) != 0) {
        // Blaze returns non-zero if it fails. In this case, we need to remove files in the 'ripe'
        // folder because they're inconsistent (blaze's runs in parallel, so the block sequence
        // is not complete). We blindly clean all ripe files, which is a bit of overill, but it's
        // easy and it works. Next time we run, blaze will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        LOG_WARN("Blaze quit without finishing. Reprocessing...");
        EXIT_NOMSG(false);
    }

    // Blaze succeeded, but the user may have started `acctExport` during the time blaze was running.
    // We don't want acctExport to get incorrect results, so we bail out knowing that the ripe
    // folder is in a consistant state, and the next scrape will pick up where it left off.
    if (isRunning("acctExport")) {
        LOG_WARN("acctExport is running. blockScrape will re-run in a moment...");
        EXIT_NOMSG(false);
    }

    // Blaze has sucessfullly created individual files, one for each block between 'blazeStart' and
    // 'blazeStart + n_blocks'. Each file is a fixed-width list of addresses that appear in that block.
    // The unripe folder holds blocks that are less than 28 blocks old. We do nothing further with them,
    // but the query tool (acctExport) may use them if the user so instructs.

    // From this point until the end of this invocation, the processing must be able to stop abruptly without
    // resulting in corrupted data (Control+C for example). This means we must process a single file at a
    // time in sequential order. 'Processing' means moving files from ripe to staging and then (if possible)
    // from the staging into a finalized chunk. If we stop processing at any point, we want to leave the
    // data in a state where the next invocation can either clean up or pick up where it left off.

    // Next, we processes one file in the ripe folder at a time (using the consolidator) by appending
    // that file's data to a temporary staging file. We flush the data after each append and then remove
    // the ripe file. If this process is interrupted, we clean up both the ripe folder and the temporary
    // staging file. This may be over kill, but it's safer. In effect, we start over at the most recently
    // successfully staged block the next time through. The value `prevBlock` points to the last completed
    // staging block. That is, the last block in the file is ../staged/{prevBlock}.txt.
    //
    // TODO(tjayrush): Is this still true? I don't think it is.
    // We're also processing time stamps (which are permitted to be ahead of the scraper) at the same time.

    // The consolidator creates index chunks if possible
    if (!progress.tmp_file.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        EXIT_NOMSG(false);
    }

    // Spin through 'ripe' files in order and process each one as we go. Note: it's okay to allow the
    // timestamp file to get ahead of the staged blocks. We only write when the block number is a new
    // one not already in the file.
    if (!forEveryFileInFolder(indexFolder_ripe, visitCopyRipeToStage, &progress)) {
        // Something went wrong with copying one of the ripe blocks into staging. (i.e. the user hit
        // Control+C or we encountered a non-sequential list of files). We clean up and start over the
        // next time through.
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        progress.tmp_file.close();
        ::remove(progress.tmp_fn.c_str());
        EXIT_NOMSG(false);
    }
    progress.tmp_file.close();

    // The stage (which is a single text file of fixed-width records of un-finalized blocks) now
    // contains all non-consolidated records. The ripe folder is empty. All files are closed.

    // Next, we try to create one or more chunks. Creating a chunk means consolidating them (writing
    // them to a binary relational table), and re-write any unfinalized records back onto the stage.
    // Again, if anything goes wrong we need clean up and leave the data in a recoverable state.
    if (!finalize_chunks(&progress)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(progress.tmp_fn.c_str());
    }

    // We completed one scrape and can now go to sleep
    EXIT_NOMSG(true);
}

//--------------------------------------------------------------------------
bool COptions::finalize_chunks(CConsolidator* cons) {
    // 'blaze' is finished scraping (and extracting addresses from) new blocks from the chain.
    // Those appearance records are stored in a file called "XXX"; The file 'oldStage' contains
    // staged but not yet consolidated records. 'tempStage' contains newly scraped ripe blocks.
    // 'newStage' is empty. This temporary file is where we will put the records from these two files.
    // We need to process in a way that will allow for interruption.

    LOG8("");

    // This file, in the staging folder, is the result of that previous blaze scrape. It is sorted.
    // We append this file into a temporary file, then append all new blocks into that same file,
    // then move that temporary file to the newStage file, and finally remove this file. In this way,
    // oldStage is the 'gold' data right up until the time 'newStage' replaces it.
    string_q oldStage = getLastFileInFolder(indexFolder_staging, false);
    LOG8("oldStage:  ", oldStage);

    // This is the file as it will be once we're done moving any new blocks to the stage. Remember
    // that the stage has a single, sorted text file whereas 'blaze' creates one file per block
    // because blaze is parallel. If the process completes, this file will be the only file remaining
    // in the staging folder. If the process does not complete, the previous staging file (oldStage)
    // will still be valid.
    string_q newStage = indexFolder_staging + padNum9(cons->prevBlock) + ".txt";
    LOG8("newStage:  ", newStage);

    // If newStage and oldStage are the same, blaze did not produce any new blocks...
    if (oldStage == newStage) {
        // TODO(tjayrush): the number 59 here is obviously not a good thing...
        blknum_t curSize = fileSize(oldStage) / 59;
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_PROGRESS("No new blocks ", curSize, MAX_ROWS, ". Need " + uint_2_Str(MAX_ROWS - curSize) + " more.");
        return true;
    }

    // We always want the stage to contain a file with perfectly valid data. We do this by working in
    // a temporary file. Once we're ready, we move the temp file to newStage and only then delete oldStage.
    string_q tmpFile = getIndexPath("temp.txt");
    LOG8("tmpFile:   ", tmpFile);

    LOG8("cons->tmp_fn: ", cons->tmp_fn);
    if (oldStage != cons->tmp_fn) {
        LOG8("oldStage != cons->tmp_fn:");
        LOG8(oldStage, " ", fileSize(oldStage));
        LOG8(cons->tmp_fn, " ", fileSize(cons->tmp_fn));
        if (!appendFile(tmpFile /* to */, oldStage /* from */)) {
            // oldStage is still valid. Caller will clean up the rest
            LOG_ERR("Could not append oldStage to temp.fil.");
            return false;
        }
        LOG8("Appended oldStage to tmpFile ", tmpFile, " ", fileSize(tmpFile));
    }

    // ...next we append the new ripe records if we can...
    if (!appendFile(tmpFile /* to */, cons->tmp_fn /* from */)) {
        // oldStage is still valid. Caller will clean up the rest
        ::remove(tmpFile.c_str());
        LOG_ERR("Could not append cons->tmp_fn to temp.fil.");
        return false;
    }
    LOG8("Appended con->tmp_fn to tmpFile ", tmpFile, " ", fileSize(tmpFile));
    LOG8("We're done appending and now replaceing...");

    // ...finally, we move the temp file to the new stage without allowing user to hit control+c
    lockSection(true);
    ::rename(tmpFile.c_str(), newStage.c_str());
    if (oldStage != cons->tmp_fn && oldStage != newStage)
        ::remove(oldStage.c_str());
    ::remove(cons->tmp_fn.c_str());
    lockSection(false);

    // Did user hit control+c?
    if (shouldQuit())
        return true;

    // We are now in a valid state with all records in the properly named newStage
    blknum_t curSize = fileSize(newStage) / 59;

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
            LOG_INFO(cGreen, "\t", (where + 1), ": ", (lines.size() > (where + 1) ? lines[where + 1] : "-end-of-file-"),
                     cOff);
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
CConsolidator::CConsolidator(const CBlockProgress& progress) {
    prevBlock = (progress.staging == NOPOS ? (progress.finalized == NOPOS ? 0 : progress.finalized) : progress.staging);
    tmp_fn = indexFolder_staging + "000000000-temp.txt";
    tmp_file.open(tmp_fn, ios::out | ios::trunc);
    unripe = progress.unripe;
    ripe = progress.ripe;
    staging = progress.staging;
    finalized = progress.finalized;
    client = progress.client;
}

//---------------------------------------------------------------------------------------------------
bool visitToPin(const string_q& chunkId, void* data) {
    LOG_INFO("   --> Pinning ", chunkId);
    ASSERT(data);
    // CPinnedItem pinRecord = *(CPinnedItem*)data;
    // pinChunk(chunkId, pinRecord);
    return !shouldQuit();
}
