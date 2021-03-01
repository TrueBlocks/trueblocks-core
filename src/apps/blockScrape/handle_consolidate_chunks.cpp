/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "consolidator.h"

//--------------------------------------------------------------------------
bool CConsolidator::consolidate_chunks(void) {
    ENTER("consolidate");

    LOG_INDEX8(tmpFile, " staging completed");
    LOG_INDEX8(tmp_fn, " staging completed");
    LOG_INDEX8(oldStage, " staging completed");
    LOG_INDEX8(newStage, " staging completed not yet consolidated");

    // We are now in a valid state. All records that have not yet been consolidated
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
        CPinnedItem pinRecord;
        writeIndexAsBinary(chunkPath, consolidatedLines, (pin ? visitToPin : nullptr), &pinRecord);

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

//---------------------------------------------------------------------------------------------------
bool visitToPin(const string_q& chunkId, void* data) {
    LOG_INFO("  Pinning");
    LOG_FN8(chunkId);
    ASSERT(data);
    // CPinnedItem pinRecord = *(CPinnedItem*)data;
    // pinChunk(chunkId, pinRecord);
    return !shouldQuit();
}
