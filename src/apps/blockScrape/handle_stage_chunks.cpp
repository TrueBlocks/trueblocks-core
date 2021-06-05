/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "consolidator.h"

//--------------------------------------------------------------------------
bool CConsolidator::stage_chunks(void) {
    LOG8("");
    ENTER("stage_chunks");

    // 'blaze' is finished scraping (and extracting addresses from) new blocks from the chain.
    // Those appearance records are stored in a file called "XXX"; The file 'oldStage' contains
    // staged but not yet consolidated records. 'tempStage' contains newly scraped ripe blocks.
    // 'newStage' is empty. This temporary file is where we will put the records from these two files.
    // We need to process in a way that will allow for interruption.

    // This file, in the staging folder, is the result of that previous blaze scrape. It is sorted.
    // We append this file into a temporary file, then append all new blocks into that same file,
    // then move that temporary file to the newStage file, and finally remove this file. In this way,
    // oldStage is the 'gold' data right up until the time 'newStage' replaces it.
    oldStage = getLastFileInFolder(indexFolder_staging, false);
    LOG_FN8(oldStage);

    // This is the file as it will be once we're done moving any new blocks to the stage. Remember
    // that the stage has a single, sorted text file whereas 'blaze' creates one file per block
    // because blaze is parallel. If the process completes, this file will be the only file remaining
    // in the staging folder. If the process does not complete, the previous staging file (oldStage)
    // will still be valid.
    newStage = indexFolder_staging + padNum9(prevBlock) + ".txt";
    LOG_FN8(newStage);

    // If newStage and oldStage are the same, blaze did not produce any new blocks...
    if (oldStage == newStage) {
        // TODO(tjayrush): the number 59 here is obviously not a good thing...
        blknum_t curSize = fileSize(oldStage) / 59;
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_PROGRESS("No new blocks ", curSize, MAX_ROWS, ". Need " + uint_2_Str(MAX_ROWS - curSize) + " more.");
        EXIT_NOMSG(true);
    }

    // We always want the stage to contain a file with perfectly valid data. We do this by working in
    // a temporary file. Once we're ready, we move the temp file to newStage and only then delete oldStage.
    // The next time we run, if we didn't create a newStage file, it will start at oldStage
    tmpFile = getIndexPath("temp.txt");
    LOG_FN8(tmpFile);

    LOG_FN8(tmp_fn);
    if (oldStage != tmp_fn) {
        LOG8("oldStage != tmp_fn");
        LOG_FN8(oldStage);
        LOG_FN8(tmp_fn);
        if (!appendFile(tmpFile /* to */, oldStage /* from */)) {
            // oldStage is still valid. Caller will clean up the rest
            LOG_ERR("Could not append oldStage to temp.fil.");
            EXIT_NOMSG(false);
        }
        LOG8("Appended oldStage to tmpFile");
        LOG_FN8(oldStage);
        LOG_FN8(tmpFile);
    }

    // ...next we append the new ripe records if we can...
    if (!appendFile(tmpFile /* to */, tmp_fn /* from */)) {
        // oldStage is still valid. Caller will clean up the rest
        ::remove(tmpFile.c_str());
        LOG_ERR("Could not append tmp_fn to temp.fil.");
        EXIT_NOMSG(false);
    }
    LOG8("Appended con->tmp_fn to tmpFile");
    LOG_FN8(tmpFile);

    // ...finally, we move the temp file to the new stage without allowing user to hit control+c
    LOG8("We're done appending. Next, move records from tmpFile to newStage...");
    lockSection();
    ::rename(tmpFile.c_str(), newStage.c_str());
    if (oldStage != tmp_fn && oldStage != newStage)
        ::remove(oldStage.c_str());
    ::remove(tmp_fn.c_str());
    unlockSection();

    EXIT_NOMSG(true);
}
