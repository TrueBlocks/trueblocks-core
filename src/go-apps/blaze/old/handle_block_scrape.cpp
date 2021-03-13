/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "consolidator.h"

// Note: We want to re-evaluate our progress each time we loop, so don't move this to parseOptions
//--------------------------------------------------------------------------
bool COptions::scrape_blocks(void) {
    ENTER("scrape_blocks");

    LOG8(" ");
    LOG8(string_q(120, '-'));
    LOG8(string_q(120, '-'));

    static blknum_t runs = 0;  // this counter is used for texting purposes only
    if (is LiveTest() && runs++ > TEST_RUNS)
        defaultQuitHandler(0);

    // First, we need to know how far along in the scrape we are. We get the progress (i.e. highest
    // block) of the client and the each of index caches.
    CConsolidator cons(getBlockProgress());

    if (true) {  // !is LiveTest()
        // From there, we can determine where to start the scraper (one more than the largest cache)
        cons.scrapeStart = max(cons.ripe, max(cons.staging, cons.finalized)) + 1;

        cons.client = CLIENT;
        cons.scrapeCnt = n_blocks;

        // Make a few adjustments here in the non-docker case to speed things up a bit
        if (!isDockerMode()) {
            if (cons.scrapeStart < 450000) {
                cons.scrapeCnt = max((blknum_t)4000, cons.scrapeCnt);

            } else if (ddosRange(cons.scrapeStart)) {
                // ...or slow things down...
                cons.scrapeCnt = getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 200);
            }
        }
        cons.scrapeCnt = N_BLOCKS;

        // If a block is more than 28 blocks from the head we consider it 'ripe.' Once a block goes
        // ripe, we no longer ask the node about it. We try to move it to staging. Staging means the
        // block is ready to be consolidated (finalized). 28 blocks is an arbitrarily chosen value,
        // but is a bit more than six minutes under normal operation ((14 * 28) / 60 == 6.5). If the
        // index is near the head of the chain and the difficulty level is high (the time bomb is
        // exploding), the time will extend, but the final nature of the operation is the same.
        cons.scrapeRipe = (cons.client < 28 ? 0 : cons.client - 28);

        // One final adjustment to nBlocks so we don't run past the tip of the chain
        if ((cons.scrapeStart + cons.scrapeCnt) > cons.client) {
            ASSERT(scrapeStart <= cons.client);  // see above
            cons.scrapeCnt = (cons.client - cons.scrapeStart);
        }
    }

    LOG8("bs.unripe:         ", cons.unripe);
    LOG8("bs.ripe:           ", cons.ripe);
    LOG8("bs.staging:        ", cons.staging);
    LOG8("bs.finalized:      ", cons.finalized);
    LOG8("bs.client:         ", cons.client);
    LOG8("bs.scrapeStart:      ", cons.scrapeStart);
    LOG8("bs.scrapeCnt:        ", cons.scrapeCnt);
    LOG8("bs.n_block_procs:  ", n_block_procs);
    LOG8("bs.n_addr_procs:   ", n_addr_procs);
    LOG8("bs.scrapeRipe:       ", cons.scrapeRipe);
    LOG8("bs.MAX_ROWS:       ", MAX_ROWS);

    // How far are we from the head?
    cons.distFromHead = (cons.client > cons.scrapeStart ? cons.client - cons.scrapeStart : 0);

    // If we're caught up, we can sleep longer - say 13 seconds
    if (sleep < 13 && cons.distFromHead <= cons.scrapeCnt)
        sleep = 13;

    // In some cases, for example, the index is ahead of the tip because we're re-syncing...do nothing...
    if (cons.scrapeStart > cons.client) {
        ostringstream os;
        os << "The index (" << cons.scrapeStart << ") is at the tip of the chain (" << cons.client << ").";
        LOG_INFO(os.str());
        // Returning false just means this round didn't complete. Loop continues.
        EXIT_NOMSG(false);
    }

    if (shouldQuit()) {
        LOG_WARN("The user hit control+C...");
        EXIT_NOMSG(false);
    }

    // Tell the user what's going on...
    LOG_INFO(cGreen, "bbbb scrape (", (cons.distFromHead), " blocks from head)", cOff);

    // We're ready to scrape, so build the bbbb command line...
    ostringstream os;
    os << "env TB_INDEXPATH=\"" << getIndexPath("") << "\" ";
    os << "bbbb scrape ";
    os << "--startBlock " << cons.scrapeStart << " ";
    os << "--nBlocks " << cons.scrapeCnt << " ";
    os << "--nBlockProcs " << n_block_procs << " ";
    os << "--nAddrProcs " << n_addr_procs << " ";
    os << "--ripeBlock " << cons.scrapeRipe << " ";
    os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "");
    LOG_CALL(substitute(os.str(), getIndexPath(""), "$INDEX/"));
    if (system(os.str().c_str()) != 0) {
        // bbbb returns non-zero if it fails. In this case, we need to remove files in the 'ripe'
        // folder because they're inconsistent (bbbb's runs in parallel, so the block sequence
        // is not complete). We blindly clean all ripe files, which is a bit of overill, but it's
        // easy and it works. Next time we run, bbbb will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        LOG_WARN("bbbb quit without finishing. Reprocessing...");
        defaultQuitHandler(1);  // assume the user hit control+c
        EXIT_NOMSG(false);
    }
    cerr << endl;

    // bbbb succeeded, but the user may have started `acctExport` during the time bbbb was running.
    // We don't want acctExport to get incorrect results, so we bail out knowing that the ripe
    // folder is in a consistant st ate, and the next scrape will pick up where it left off.
    if (isRunning("acctExport")) {
        LOG_WARN("acctExport is running. blockScrape will re-run in a moment...");
        EXIT_NOMSG(false);
    }

    // bbbb has sucessfullly created individual files, one for each block between 'scrapeStart' and
    // 'scrapeStart + n_blocks'. Each file is a fixed-width list of addresses that appear in that block.
    // The unripe folder holds blocks that are less than 28 blocks old. We do nothing further with them,
    // but the query tool (acctExport) may use them if the user so instructs.

    // From this point until the end of this invocation, the processing must be able to stop abruptly without
    // resulting in corrupted data (Control+C for example). This means we must process a single file at a
    // time in sequential order. 'Processing' means moving files from ripe to staging and then (if possible)
    // from the staging into a finalized chunk. If we stop processing at any point, we want to leave the
    // data in a s tate where the next invocation can either clean up or pick up where it left off.

    // Next, we processes one file in the ripe folder at a time (using the consolidator) by appending
    // that file's data to a temporary staging file. We flush the data after each append and then remove
    // the ripe file. If this process is interrupted, we clean up both the ripe folder and the temporary
    // staging file. This may be over kill, but it's safer. In effect, we start over at the most recently
    // successfully staged block the next time through. The value `prevBlock` points to the last completed
    // staging block. That is, the last block in the file is ../staged/{prevBlock}.txt.
    //
    // We're also processing time stamps (which are permitted to be ahead of the scraper) at the same time.

    // The consolidator creates index chunks if possible
    if (!cons.tmp_stream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        EXIT_NOMSG(false);
    }

    // Spin through 'ripe' files in order and process each one as we go. Note: it's okay to allow the
    // timestamp file to get ahead of the staged blocks. We only write when the block number is a new
    // one not already in the file.
    if (!forEveryFileInFolder(indexFolder_ripe, appendRipeToStage, &cons)) {
        // Something went wrong with copying one of the ripe blocks into staging. (i.e. the user hit
        // Control+C or we encountered a non-sequential list of files). We clean up and start over the
        // next time through.
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        cons.tmp_stream.close();
        ::remove(cons.tmp_fn.c_str());
        EXIT_NOMSG(false);
    }
    cons.tmp_stream.close();

    // The stage (which is a single text file of fixed-width records of un-finalized blocks) now
    // contains all non-consolidated records. The ripe folder is empty. All files are closed.

    // Next, we try to create one or more chunks. Creating a chunk means consolidating them (writing
    // them to a binary relational table), and re-write any unfinalized records back onto the stage.
    // Again, if anything goes wrong we need clean up and leave the data in a recoverable s tate.
    cons.pin = pin;
    if (!cons.finalize_chunks()) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }
}

//--------------------------------------------------------------------------
bool CConsolidator::consolidate_chunks(void) {
    ENTER("consolidate");

    LOG_INDEX8(tmpFile, " staging completed");
    LOG_INDEX8(tmp_fn, " staging completed");
    LOG_INDEX8(oldStage, " staging completed");
    LOG_INDEX8(newStage, " staging completed not yet consolidated");

    // We are now in a valid s tate. All records that have not yet been consolidated
    // are in newStage. Count how many lines we have...
    blknum_t nRecords = fileSize(newStage) / 59;

    // ...if we don't have enough, return and get more...
    if (nRecords <= MAX_ROWS) {
        blknum_t distToHead = (MAX_ROWS - nRecords);
        LOG_INFO("");
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_INFO(cYellow, "  Have ", nRecords, " records of ", MAX_ROWS, ". Need ", distToHead, " more.", cOff);
        LOG_INDEX8(newStage, " consolidation not ready");
        EXIT_NOMSG(true);
    }

    // We have enough records to consolidate. Process chunks (of size MAX_ROWS) until done.
    // This may take more than one pass. Check for user input control+C at each pass.
    size_t pass = 0;
    while (nRecords > MAX_ROWS && !shouldQuit()) {
        lockSection();

        LOG_INFO("");
        LOG_INFO(bBlue, "Consolidation pass ", pass++, cOff);
        CStringArray lines;
        lines.reserve(nRecords + 100);
        asciiFileToLines(newStage, lines);

        LOG_INFO(cWhite, "  Starting search at record ", (MAX_ROWS - 1), " of ", lines.size(), cOff);
        LOG4(cGreen, "\t", (MAX_ROWS - 1), ": ", lines[MAX_ROWS - 1], cOff);
        LOG4(cGreen, "\t", (MAX_ROWS), ": ", lines[MAX_ROWS], cOff);

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

        LOG_INFO(cWhite, "  Break at line ", where, ". Extracting [0 to ", where, "] of ", lines.size(), cOff);
        LOG4(cGreen, "\t", 0, ": ", lines[0], cOff);
        LOG4(cGreen, "\t", 1, ": ", lines[1], cOff);
        LOG4(bBlue, "\t", where, ": ", lines[where], cOff);
        LOG4(bTeal, "\t", (where + 1), ": ", lines[where + 1], cOff);

        CStringArray consolidatedLines;
        consolidatedLines.reserve(lines.size());
        for (uint64_t record = 0; record <= where; record++) {
            if (countOf(lines[record], '\t') != 2) {
                LOG_WARN("Found a record with less than two tabs.");
                LOG_WARN("preceeding line:\t[", ((record > 0) ? lines[record - 1] : "N/A"), "]");
                LOG_WARN("offending line:\t[", lines[record], "]");
                LOG_WARN("following line:\t[", ((record < where) ? lines[record + 1] : "N/A"), "]");
                EXIT_NOMSG(false);
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

        writeIndexAsBinary(chunkPath, consolidatedLines, (pin ? visitToPin : nullptr), &pinList);

        LOG_INFO(cWhite, "  Wrote ", consolidatedLines.size(), " records to ",
                 substitute(chunkPath, indexFolder_finalized, "$FINAL/"), cOff);

        where += 1;
        CStringArray remainingLines;
        remainingLines.reserve(MAX_ROWS + 100);

        LOG4(cWhite, "  Extracting records ", where, " to ", lines.size(), " of ", lines.size(), cOff);
        LOG4(cGreen, "\t", where, ": ", lines[where], cOff);
        LOG4(cGreen, "\t", (where + 1), ": ", (lines.size() > (where + 1) ? lines[where + 1] : "-end-of-file-"), cOff);
        LOG4(bBlue, "\t", (where - 1), ": ", lines[where - 1], cOff);
        LOG4(bTeal, "\t", (where), ": ", lines[where], cOff);

        for (uint64_t record = where; record < lines.size(); record++)
            remainingLines.push_back(lines[record]);

        ::remove(newStage.c_str());
        linesToAsciiFile(newStage, remainingLines);
        LOG_INFO(cWhite, "  Wrote ", remainingLines.size(), " records to ",
                 substitute(newStage, indexFolder_staging, "$STAGING/"), cOff);

        nRecords = fileSize(newStage) / 59;
        unlockSection();
    }

    LOG_INDEX8(newStage, " after consolidation");
    EXIT_NOMSG(true);
}

bool CConsolidator::finalize_chunks(void) {
    LOG8("");
    ENTER("finalize_chunks");

    // 'bbbb' is finished scraping (and extracting addresses from) new blocks from the chain.
    // Those appearance records are stored in a file called "XXX"; The file 'oldStage' contains
    // staged but not yet consolidated records. 'tempStage' contains newly scraped ripe blocks.
    // 'newStage' is empty. This temporary file is where we will put the records from these two files.
    // We need to process in a way that will allow for interruption.

    // This file, in the staging folder, is the result of that previous bbbb scrape. It is sorted.
    // We append this file into a temporary file, then append all new blocks into that same file,
    // then move that temporary file to the newStage file, and finally remove this file. In this way,
    // oldStage is the 'gold' data right up until the time 'newStage' replaces it.
    oldStage = getLastFileInFolder(indexFolder_staging, false);
    LOG_FN8(oldStage);

    // This is the file as it will be once we're done moving any new blocks to the stage. Remember
    // that the stage has a single, sorted text file whereas 'bbbb' creates one file per block
    // because bbbb is parallel. If the process completes, this file will be the only file remaining
    // in the staging folder. If the process does not complete, the previous staging file (oldStage)
    // will still be valid.
    newStage = indexFolder_staging + padNum9(prevBlock) + ".txt";
    LOG_FN8(newStage);

    // If newStage and oldStage are the same, bbbb did not produce any new blocks...
    if (oldStage == newStage) {
        // TODO(tjayrush): the number 59 here is obviously not a good thing...
        blknum_t curSize = fileSize(oldStage) / 59;
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_PROGRESS("No new blocks ", curSize, MAX_ROWS, ". Need " + uint_2_Str(MAX_ROWS - curSize) + " more.");
        EXIT_NOMSG(true);
    }

    // We always want the stage to contain a file with perfectly valid data. We do this by working in
    // a temporary file. Once we're ready, we move the temp file to newStage and only then delete oldStage.
    tmpFile = getIndexPath("temp.txt");
    LOG_FN8(tmpFile);

    LOG_FN8(tmp_fn);
    if (oldStage != tmp_fn) {
        LOG8("oldStage != tmp_fn");
        LOG_FN8(oldStage);
        LOG_FN8(tmp_fn);
        if (!appendFileToFile(tmpFile /* to */, oldStage /* from */)) {
            // oldStage is still valid. Caller will clean up the rest
            LOG_ERR("Could not append oldStage to temp.fil.");
            EXIT_NOMSG(false);
        }
        LOG8("Appended oldStage to tmpFile");
        LOG_INDEX8(oldStage, "");
        LOG_INDEX8(tmpFile, " after appending oldStage");
    }

    // ...next we append the new ripe records if we can...
    if (!appendFileToFile(tmpFile /* to */, tmp_fn /* from */)) {
        // oldStage is still valid. Caller will clean up the rest
        ::remove(tmpFile.c_str());
        LOG_ERR("Could not append tmp_fn to temp.fil.");
        EXIT_NOMSG(false);
    }
    LOG8("Appended con->tmp_fn to tmpFile");
    LOG_INDEX8(tmpFile, " after appending tmp_fn");

    // ...finally, we move the temp file to the new stage without allowing user to hit control+c
    LOG8("We're done appending. Next, move records from tmpFile to newStage...");
    lockSection();
    ::rename(tmpFile.c_str(), newStage.c_str());
    if (oldStage != tmp_fn && oldStage != newStage)
        ::remove(oldStage.c_str());
    ::remove(tmp_fn.c_str());
    unlockSection();

    // Did user hit control+c?
    if (shouldQuit())
        EXIT_NOMSG(true);

    EXIT_NOMSG(consolidate_chunks());
}

// TODO: We should try to scrape timestamps with bbbb while we're doing this scan
// TODO: Cleanup on quit as per clean-upAndQuit
// TODO: Get test cases to work
// TODO: Knowing the current s tate of the scrape (running, sleeping, paused, resumed)
// TODO: alal change-State(void) and get-CurrentState
// TODO: Be responsive to user pausing and re-enabling the scrapers
// TODO: Update timestamps on each pass (any maybe other things like difficulty)
// TODO: Make sure the server runs only once and starts the scraper only once
// TODO: Can I process things as they come (instead of 'all' blocks then all 'monitors')
// TODO: Can't we do, process a block shove it into pipe, monitor scraper ignores if not
// TODO: turned on or not a monitored address
// TODO: Either we have to disallow the user to hit Control+C or make sure the processing is stable at every
// TODO: moment and/or can recover
// TODO: What parts (if any or all) of pinLib need to be re-written? writeIndexAsBinary needs to be re-written
// TODO: Pinning happened (optionally) after consolidation by calling a function sent in to the index writer
// TODO: We have to make sure pinning works when we're done - that's the whole point
// TODO: Should be able to scrape monitors only, blocks only, or both, or neither
// TODO: (neither updates only timestamps, difficulty columns)
// TODO: Should be able to start, stop, pause, resume scrapers (individually)
// TODO: Publishing means writing to the smart contract (Reconsider the idea of publishing when and where)
// TODO: pinning and publishing need API keys
// TODO: User should be able to specify how long to sleep between scrapes (should he - reconsider)
// TODO: nBlocks, nBlockProcs, nAddrProcs, sleep, pin, publish, tool(monitors/blocks/both/neither)
// TODO: blaze creates individual files per block in random order (called ripe)
// TODO:    -- after all blaze blocks are scraped, we append them (in order) to the stage file
// TODO:    -- if interrupted, ripe is ignored and we start over at last staging
// TODO:    -- if completed, we append all ripe files in order to staging
// TODO:    -- always start at last sucessful staged block
// TODO:    -- don't let it sleep less than .5 seconds so Control+C works
// TODO: We should try to scrape timestamps with bbbb while we're doing this scan
// TODO: If the block scraper 'touched' each monitor that needs updating every
// TODO: time something changed, we could skip over anything that hasn't changed
// TODO: when we do an update
// TODO: We need to make sure we have an archive node, and balances, and traces or optionally
// TODO: allow the end user to tell us it's okay by reading blockScrape.toml file for 'needsArchive'
// TODO: 'needsBalances', 'needsEvents', etc.
// TODO: bool needsTracing = config->getConfigBool("requires", "tracing", true);
// TODO: bool needsParity = config->getConfigBool("requires", "parity", true);
// TODO: bool needsBalances = config->getConfigBool("requires", "balances", false);
// TODO: You must process the zero block (prefunds in order by address + bn + txid)
// TODO: n_blocks = config("blockScrape")->Int("settings", "n_blocks", (isDockerMode() ? 100 : 2000));
// TODO: n_block_procs = config("blockScrape")->Int("settings", "n_block_procs", (isDockerMode() ? 5 : 10));
// TODO: n_addr_procs = config("blockScrape")->Int("settings", "n_addr_procs", (isDockerMode() ? 10 : 20));
// TODO: https://medium.com/@matryer/golang-advent-calendar-day-eleven-persisting-go-objects-to-disk-7caf1ee3d11d
