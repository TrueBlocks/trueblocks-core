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

    LOG4(string_q(80, '-'));

    static blknum_t runs = 0;  // this counter is used for texting purposes only
    if (isLiveTest() && runs++ > TEST_RUNS)
        defaultQuitHandler(0);

    // First, we need to know how far along in the scrape we are. We get the progress (i.e. highest
    // block) of the client and the each of index caches.
    CConsolidator cons(getBlockProgress());

    // From there, we can determine where to start the scraper (one more than the largest cache)
    cons.blazeStart = max(cons.ripe, max(cons.staging, cons.finalized)) + 1;

    cons.client = CLIENT;
    cons.blazeCnt = n_blocks;

    // Make a few adjustments here in the non-docker case to speed things up a bit
    if (!isDockerMode()) {
        if (cons.blazeStart < 450000) {
            cons.blazeCnt = max((blknum_t)4000, cons.blazeCnt);

        } else if (ddosRange(cons.blazeStart)) {
            // ...or slow things down...
            cons.blazeCnt = getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 200);
        }
    }
    cons.blazeCnt = N_BLOCKS;

    // If a block is more than 28 blocks from the head we consider it 'ripe.' Once a block goes
    // ripe, we no longer ask the node about it. We try to move it to staging. Staging means the
    // block is ready to be consolidated (finalized). 28 blocks is an arbitrarily chosen value,
    // but is a bit more than six minutes under normal operation ((14 * 28) / 60 == 6.5). If the
    // index is near the head of the chain and the difficulty level is high (the time bomb is
    // exploding), the time will extend, but the final nature of the operation is the same.
    cons.blazeRipe = (cons.client < 28 ? 0 : cons.client - 28);

    // One final adjustment to nBlocks so we don't run past the tip of the chain
    if ((cons.blazeStart + cons.blazeCnt) > cons.client) {
        ASSERT(blazeStart <= cons.client);  // see above
        cons.blazeCnt = (cons.client - cons.blazeStart);
    }

    LOG8("bs.unripe:         ", cons.unripe);
    LOG8("bs.ripe:           ", cons.ripe);
    LOG4("bs.staging:        ", cons.staging);
    LOG4("bs.finalized:      ", cons.finalized);
    LOG4("bs.client:         ", cons.client);
    LOG4("bs.blazeStart:     ", cons.blazeStart);
    LOG4("bs.blazeCnt:       ", cons.blazeCnt);
    LOG4("bs.n_block_procs:  ", n_block_procs);
    LOG4("bs.n_addr_procs:   ", n_addr_procs);
    LOG4("bs.blazeRipe:      ", cons.blazeRipe);
    LOG4("bs.MAX_ROWS:       ", MAX_ROWS);

    // How far are we from the head?
    cons.distFromHead = (cons.client > cons.blazeStart ? cons.client - cons.blazeStart : 0);

    // If we're caught up, we can sleep longer - say 13 seconds
    if (sleep < 13 && cons.distFromHead <= cons.blazeCnt)
        sleep = 13;

    // In some cases, for example, the index is ahead of the tip because we're re-syncing...do nothing...
    if (cons.blazeStart > cons.client) {
        ostringstream os;
        os << "The index (" << cons.blazeStart << ") is at the tip of the chain (" << cons.client << ").";
        LOG_INFO(os.str());
        // Returning false just means this round didn't complete. Loop continues.
        EXIT_NOMSG(false);
    }

    if (shouldQuit()) {
        LOG_WARN("The user hit control+C...");
        EXIT_NOMSG(false);
    }

    // Tell the user what's going on...
    LOG_INFO(cGreen, "blaze scrape ", cons.blazeStart, "-", (cons.blazeStart + cons.blazeCnt), "-",
             cons.blazeCnt, " (", cons.distFromHead, " blocks from head)", cOff);

    // We're ready to scrape, so build the blaze command line...
    ostringstream os;
    os << "env TB_INDEXPATH=\"" << getIndexPath("") << "\" ";
    os << "blaze scrape ";
    os << "--startBlock " << cons.blazeStart << " ";
    os << "--nBlocks " << cons.blazeCnt << " ";
    os << "--nBlockProcs " << n_block_procs << " ";
    os << "--nBlockProcs " << n_block_procs << " ";
    os << "--nAddrProcs " << n_addr_procs << " ";
    os << "--nAddrProcs " << n_addr_procs << " ";
    os << "--ripeBlock " << cons.blazeRipe << " ";
    // os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "");
    // LOG_CALL(substitute(os.str(), getIndexPath(""), "$INDEX/"));
    if (system(os.str().c_str()) != 0) {
        // Blaze returns non-zero if it fails. In this case, we need to remove files in the 'ripe'
        // folder because they're inconsistent (blaze's runs in parallel, so the block sequence
        // is not complete). We blindly clean all ripe files, which is a bit of overill, but it's
        // easy and it works. Next time we run, blaze will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        LOG_WARN("Blaze quit without finishing. Reprocessing...");
        defaultQuitHandler(1);  // assume the user hit control+c
        EXIT_NOMSG(false);
    }
    cerr << endl;

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
    // We're also processing time stamps (which are permitted to be ahead of the scraper) at the same time.

    // The consolidator creates index chunks if possible
    if (!cons.tmp_stream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        EXIT_NOMSG(false);
    }

    // We want this to apply if the user told us to
    cons.pin = pin;

    // Spin through 'ripe' files in order and process each one as we go. Note: it's okay to allow the
    // timestamp file to get ahead of the staged blocks. We only write when the block number is a new
    // one not already in the file. Also note that at some points during this copy (when we hit a grid
    // boundary) we will consolidate short of the MAX_ROWS boundary. We do this so make recovering from
    // incorrect chunking easier.)
    LOG8("Prior to visitCopyRipeToStage");
    LOG8(cons.Format());

    if (!forEveryFileInFolder(indexFolder_ripe, visitCopyRipeToStage, &cons)) {
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
    LOG8("After visitCopyRipeToStage");
    LOG8(cons.Format());

    // The stage (which is a single text file of fixed-width records of un-finalized blocks) now
    // contains all non-consolidated records. The ripe folder is empty. All files are closed.

    // Next, we try to create one or more chunks. Creating a chunk means consolidating them (writing
    // them to a binary relational table), and re-write any unfinalized records back onto the stage.
    // Again, if anything goes wrong we need clean up and leave the data in a recoverable state.
    if (!cons.stage_chunks()) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }

    // Did user hit control+c?
    if (shouldQuit())
        EXIT_NOMSG(true);

    bool ret = cons.consolidate_chunks();

    // We completed one scrape and can now go to sleep
    EXIT_NOMSG(ret);
}
