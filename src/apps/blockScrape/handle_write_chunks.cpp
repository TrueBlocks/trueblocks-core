/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "consolidator.h"

//---------------------------------------------------------------------------------------------------
bool CConsolidator::write_chunks(blknum_t chunkSize, bool atLeastOnce) {
    ENTER("write_chunks");

    blknum_t nRecords = fileSize(newStage) / 59;

    // We have enough records to consolidate. Process chunks (of size 'chunkSize') until done.
    // This may take more than one pass. Check for user input control+C at each pass.
    size_t pass = 0;
    while ((atLeastOnce || nRecords > chunkSize) && !shouldQuit()) {
        lockSection();

        LOG_INFO("");
        LOG_INFO(bBlue, "Consolidation pass ", pass++, cOff);
        CStringArray lines;
        lines.reserve(nRecords + 100);
        asciiFileToLines(newStage, lines);

        LOG_INFO(cWhite, "  Starting search at record ", (chunkSize - 1), " of ", lines.size(), cOff);
        LOG4(cGreen, "\t", (chunkSize - 1), ": ", lines[chunkSize - 1], cOff);
        if (chunkSize < lines.size())
            LOG4(cGreen, "\t", (chunkSize), ": ", lines[chunkSize], cOff);
        LOG4("chunkSize: ", chunkSize, " lines.size(): ", lines.size());
        LOG4((lines.size() == 0 ? "newStage file has zero lines" : ""));

        // We're looking for the location where the last complete block ends and the number
        // of records overtops chunkSize (note that chunkSize may be less than MAX_ROWS due
        // to snap_to_grid)
        string_q prvBlock;
        size_t loc = NOPOS;
        for (uint64_t record = (chunkSize - 1); record < lines.size() && loc == NOPOS; record++) {
            CStringArray pParts;
            explode(pParts, lines[record], '\t');
            if (verbose > 2 && (record == lines.size() - 2)) {
                LOG_INFO(bBlue, "\t", record, ": ", pParts[0], " -- ", pParts[1], " -- ", pParts[2], cOff);
            }
            if (prvBlock != pParts[1]) {
                if (!prvBlock.empty())
                    loc = record - 1;
                prvBlock = pParts[1];
            }
        }

        // Now we know where the break happens. We want to put all records up to and including
        // the break in the new chunk and put all the records after the break to the end of
        // the array back into newStage
        if (loc == NOPOS) {
            loc = lines.size() ? lines.size() - 1 : 0;
            LOG8("  Last full block is last line in file: ", nRecords, " loc: ", loc);
        }

        LOG_INFO(cWhite, "  Break line ", loc, " of ", lines.size(), ". [0 to ", loc, " of ", (loc - 0 + 1), "]", cOff);
        LOG4(cGreen, "\t", 0, ": ", lines[0], cOff);
        LOG4(cGreen, "\t", 1, ": ", lines[1], cOff);
        LOG4(bBlue, "\t", loc, ": ", lines[loc], cOff);
        if (loc + 1 < lines.size())
            LOG4(bTeal, "\t", min(lines.size(), loc + 1), ": ", lines[loc + 1], cOff);

        CStringArray remainingLines;
        remainingLines.reserve(chunkSize + 100);
        if (lines.size() > 0) {
            CStringArray consolidatedLines;
            consolidatedLines.reserve(lines.size());
            for (uint64_t record = 0; record <= loc; record++) {
                if (countOf(lines[record], '\t') != 2) {
                    LOG_WARN("Found a record with less than two tabs.");
                    LOG_WARN("preceeding line:\t[", ((record > 0) ? lines[record - 1] : "N/A"), "]");
                    LOG_WARN("offending line:\t[", lines[record], "]");
                    LOG_WARN("following line:\t[", ((record < loc) ? lines[record + 1] : "N/A"), "]");
                    EXIT_NOMSG(false);
                }
                consolidatedLines.push_back(lines[record]);
            }

            if (consolidatedLines.size() > 0) {
                CStringArray p1;
                explode(p1, consolidatedLines[0], '\t');
                CStringArray p2;
                explode(p2, consolidatedLines[consolidatedLines.size() - 1], '\t');

                sort(consolidatedLines.begin(), consolidatedLines.end());
                string_q chunkId = p1[1] + "-" + p2[1];
                string_q chunkPath = indexFolder_finalized + chunkId + ".bin";

                writeIndexAsBinary(chunkPath, consolidatedLines, (pin ? visitToPin : nullptr), &pinList);

                LOG8("  Found a chunk at [", chunkId, "] (inclusive)");
                LOG_INFO(cWhite, "  Wrote ", consolidatedLines.size(), " records to ", chunkPath, cOff);

                loc++;
                LOG4(cWhite, "  Rewriting records ", loc, " to ", lines.size(), " of ", lines.size(), " back to stage",
                     cOff);
                if (loc < lines.size())
                    LOG4(cGreen, "\t", loc, ": ", lines[loc], cOff);
                if (loc + 1 < lines.size())
                    LOG4(cGreen, "\t", (loc + 1), ": ", lines[loc + 1], cOff);
                if (loc - 1 < lines.size())
                    LOG4(bBlue, "\t", (loc - 1), ": ", lines[loc - 1], cOff);
                if (loc < lines.size())
                    LOG4(bTeal, "\t", (loc), ": ", lines[loc], cOff);

                for (uint64_t record = loc; record < lines.size(); record++)
                    remainingLines.push_back(lines[record]);

                ::remove(newStage.c_str());

            } else {
                LOG4("consolidatedLines.size() == 0. Not able to write a chunk.");
            }

        } else {
            LOG4("lines.size() == 0. Not able to write a chunk.");
        }

        if (remainingLines.size()) {
            linesToAsciiFile(newStage, remainingLines);
            LOG_INFO(cWhite, "  Wrote ", remainingLines.size(), " records to ", newStage, cOff);
        } else {
            LOG_INFO(cWhite, "  No records remain. ", substitute(newStage, indexFolder_staging, "$STAGING/"),
                     " not written.", cOff);
        }

        nRecords = fileSize(newStage) / 59;
        unlockSection();
        if (atLeastOnce)
            atLeastOnce = nRecords > 0;
        chunkSize = min(MAX_ROWS, nRecords);
    }

    LOG_FN8(newStage);
    EXIT_NOMSG(true);
}
