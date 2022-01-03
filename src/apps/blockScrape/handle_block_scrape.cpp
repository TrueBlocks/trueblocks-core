/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"
#include "consolidator.h"

// Note: We want to re-evaluate our progress each time we loop, so don't move this to parseOptions
//--------------------------------------------------------------------------
bool COptions::scrape_blocks(void) {
    ENTER("scrape_blocks");

    static blknum_t runs = 0;  // this counter is used for texting purposes only
    if (isLiveTest() && runs++ > TEST_RUNS)
        defaultQuitHandler(0);

    // First, we need to know how far along in the scrape we are. We get the progress (i.e. highest
    // block) of the client and the each of index caches. From there, we can determine where to
    // start the scraper (one more than the largest cache). Note that the first chunk for block
    // zero is special and already created
    CConsolidator cons;

    // This peice of data will be needed later when we create a new chunk, so we save it off here.
    cons.pin = pin;

    // We need to know where to start...
    cons.blazeStart = max(cons.ripe, max(cons.staging, cons.finalized)) + 1;

    // We need to know how many blocks to scrape. The user has told us...
    cons.blazeCnt = block_cnt;

    // ...but we may make some adjustments to speed things up. When not running in docker mode,
    // we can do more blocks. In docker mode, we stick with the defaults otherwise, docker
    // may kill us for using too many resources.
    if (!isDockerMode() && chain == "mainnet") {
        if (cons.blazeStart < 450000) {
            // We can speed things up on the early chain...
            cons.blazeCnt = max(blknum_t(4000), cons.blazeCnt);

        } else if (ddosRange(cons.blazeStart)) {
            // ...or slow things down during 2016s dDos...
            cons.blazeCnt = getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 500);
        }
    }

    // If a block is more than 28 blocks from the head we consider it 'ripe.' We want to tell
    // the blaze scraper when 'ripe' is, so it can ignore those blocks. Once a block goes ripe,
    // we no longer scrape it. We move it to staging instead. Staging means the block is ready to
    // be consolidated (or finalized) and put into a chunk.
    //
    // Note that 28 blocks is an arbitrarily chosen value, but is a bit more than six minutes under
    // normal operation ((14 * 28) / 60 == 6.5). If the index is near the head of the chain and the
    // difficulty level is high (the time bomb is exploding), the time will extend, but the
    // nature of the operation is the same.
    cons.blazeRipe = (cons.client < 28 ? 0 : cons.client - 28);

    // One final adjustment to blazeCnt so we don't run past the tip of the chain...
    if ((cons.blazeStart + cons.blazeCnt) > cons.client) {
        ASSERT(blazeStart <= cons.client);  // see above
        cons.blazeCnt = (cons.client - cons.blazeStart);
    }

    // How far are we from the head? This is useful for telling how long to sleep the next time.
    cons.distFromHead = (cons.client > cons.blazeStart ? cons.client - cons.blazeStart : 0);
    if (sleep < 13 && cons.distFromHead <= cons.blazeCnt)
        sleep = 13;  // we're basically caught up, so we can sleep until the next expected block

    cerr << endl;
    ostringstream os;
    os << string_q(5, '-');
    os << " Scraping " << cons.blazeStart << " to " << min(cons.client, (cons.blazeStart + cons.blazeCnt));
    os << " of " << cons.client;
    os << " (" << (cons.distFromHead) << " from head) " << string_q(30, '-');
    LOG_INFO(os.str());

    // Let the user know what's going on
    if (verbose >= 8)
        cerr << cons;

    // If the index is ahead of the tip of the chain (for example, the node is re-syncing)...do nothing...
    // Returning false only means this round didn't complete, the loop will continue.
    if (cons.blazeStart > cons.client) {
        LOG_INFO("The index (", cons.blazeStart, ") is ahead of the chain (", cons.client, ").");
        EXIT_NOMSG(false);
    }

    // If the user hit control+C somewhere along the way, let's get out of here...
    if (shouldQuit()) {
        LOG_WARN("The user hit control+C...");
        EXIT_NOMSG(false);
    }

    // We're ready to scrape, so build the blaze command line...
    os.clear();
    os.str("");
    os << getPathToCommands("blaze") + " scrape ";
    os << "--startBlock " << cons.blazeStart << " ";
    os << "--block_cnt " << cons.blazeCnt << " ";
    os << "--ripeBlock " << cons.blazeRipe << " ";
    os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "");
    LOG_TEST_CALL(os.str());

    ostringstream cmd;
    cmd << "env TB_INDEXPATH=\"" << getPathToIndex("")
        << "\" ";  // note--cobra/viper will pick this up even though you won't find it
    cmd << os.str() << " ";
    cmd << "--block_chan_cnt " << block_chan_cnt << " ";
    cmd << "--addr_chan_cnt " << addr_chan_cnt << " ";
    if (system(cmd.str().c_str()) != 0) {
        // Blaze returns non-zero if it fails. In this case, we need to remove files in the 'ripe'
        // folder because they're inconsistent (blaze's runs in parallel, so the block sequence
        // is not complete). We blindly clean all ripe files, which is a bit of overill, but it's
        // easy and it works. Next time we run, blaze will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        LOG_WARN("Blaze quit without finishing. Reprocessing...");
        defaultQuitHandler(1);  // this does not quit, but only notifies the caller that the user quit blaze early
        EXIT_NOMSG(false);
    }
    LOG_INFO("- <PROG> : Scraping ", cons.blazeCnt, " of ", cons.blazeCnt, " at block ", cons.client, "\n");

    if (!verbose) {
        cerr << '\r' << string_q(120, ' ') << '\r';
        cerr.flush();
    }

    // Blaze succeeded, but the user may have started `acctExport` during the time blaze started and finished.
    // Because we don't want acctExport to produce incorrect results, so we bail out knowing that the ripe
    // folder is in a consistant state, and the next scrape will pick up where it left off.
    if (isRunning("acctExport")) {
        LOG_WARN("'chifra export' is running. 'chifra scrape' cannot run at this time...");
        EXIT_NOMSG(false);
    }

    // Blaze has sucessfullly created an individual file for each block between 'blazeStart' and
    // 'blazeStart + block_cnt'. Each file is a fixed-width list of addresses that appear in that block.
    // The unripe folder holds blocks that are less than 28 blocks old. We do nothing further with them,
    // although acctExport may use them if it wishes to.

    // From this point until the end of this invocation (scrape), the processing must be able to stop abruptly
    // without resulting in corrupted data (Control+C for example). This means we must process a single file at
    // a time in sequential order. 'Processing' means moving files from ripe to staging and then (if possible)
    // from the staging into a finalized chunk. If we stop processing at any point, we want to leave the
    // data in a state where the next invocation can either clean up or pick up where it left off.

    // Next, we processes one file in the ripe folder at a time (using the consolidator) by appending
    // that file's data to a temporary, growing staging file. We flush the data to disc after each append
    // and then remove the ripe file. If this process is interrupted, we clean up both the ripe folder and
    // the temporary staging file. This may be over kill, but it's safer. In effect, we start over at the
    // most recently successfully staged block the next time through. The value `prevBlock` points to the
    // last completed staging block. That is, the last block in the file is ../staged/{prevBlock}.txt.
    //
    // We're also processing time stamps at the same time.

    // This file is where the consolidator stores the temporary new stage. We don't write to that newStage
    // directly. We collect in a temporary file and only write at the end. In this way, we don't corrupt
    // the stage.
    if (!cons.tmp_stream.is_open()) {
        // If we can't open the temporary stage, let the user know and try again later
        LOG_WARN("Could not open temporary staging file.");
        EXIT_NOMSG(false);
    }

    // Blaze has finished processign blazeCnt blocks. We spin through the 'ripe' folder and process each
    // one sequentially. At some points during this processing (when we hit a grid boundary) we consolidate
    // a chunk that is short of the MAX_ROWS boundary. That is, we will snap to grid. We do this in order to
    // make recovering from incorrect chunking, if it is ever identified, easier. (Otherwise, because the
    // index chunk files are named by block, every file would have to be regnerated if we found an error.)
    if (!forEveryFileInFolder(indexFolder_ripe, copyRipeToStage, &cons)) {
        // One of two thing can have happened here. (1) the user hit control+c or we encountered
        // a non-sequential block (i.e. blaze did something wrong.) The second thing that could have
        // happened is that we chunked up because we snapped to the grid. In either case, we clean up
        // and start another scrape wherever we left off.
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        cons.tmp_stream.close();
        ::remove(cons.tmp_fn.c_str());
        EXIT_NOMSG(false);
    }
    cons.tmp_stream.close();

    // The stage (which is a single text file of fixed-width records of un-chunked blocks) now
    // contains all non-consolidated records. That is, the ripe folder is empty. All files are closed.

    // Next, we try to create some chunks. Creating a chunk means consolidating them (writing
    // them to a binary relational table), and re-write any un-chunked records back onto the stage.
    // Again, if anything goes wrong we need clean up and leave the data in a recoverable state.
    if (!cons.stage_chunks()) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }

    // Did user hit control+c? Just checking...
    if (shouldQuit())
        EXIT_NOMSG(false);

    // Consolidate...
    bool ret = cons.consolidate_chunks();

    // We're done with a single scrape, we can go to sleep...
    EXIT_NOMSG(ret);
}
