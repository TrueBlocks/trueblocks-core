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

//---------------------------------------------------------------------------------------------------
bool CConsolidator::write_chunks(blknum_t chunkSize, bool atLeastOnce) {
    blknum_t nRecords = fileSize(newStage) / asciiAppearanceSize;

    // We have enough records to consolidate. Process chunks (of size 'chunkSize') until done.
    // This may take more than one pass. Check for user input control+C at each pass.
    while ((atLeastOnce || nRecords > chunkSize) && !shouldQuit()) {
        lockSection();

        CStringArray lines;
        lines.reserve(nRecords + 100);
        asciiFileToLines(newStage, lines);

        // We're looking for the location where the last complete block ends and the number
        // of records overtops chunkSize (note that chunkSize may be less than apps_per_chunk due
        // to snap_to_grid)
        string_q prvBlock;
        size_t loc = NOPOS;
        for (uint64_t record = (chunkSize - 1); record < lines.size() && loc == NOPOS; record++) {
            CStringArray pParts;
            explode(pParts, lines[record], '\t');
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
        }

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
                    return false;
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

                loc++;

                for (uint64_t record = loc; record < lines.size(); record++)
                    remainingLines.push_back(lines[record]);

                ::remove(newStage.c_str());
            }
        }

        if (remainingLines.size()) {
            linesToAsciiFile(newStage, remainingLines);
        }

        nRecords = fileSize(newStage) / asciiAppearanceSize;
        unlockSection();
        if (atLeastOnce)
            atLeastOnce = nRecords > 0;
        chunkSize = min(opts->apps_per_chunk, nRecords);
    }

    return true;
}

//----------------------------------------------------------------
bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines, CONSTAPPLYFUNC pinFunc, void* pinFuncData) {
    // ASSUMES THE ARRAY IS SORTED!

    ASSERT(!fileExists(outFn));
    string_q tmpFile = substitute(outFn, ".bin", ".tmp");

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CIndexedAppearanceArray blockTable;

    hashbytes_t hash = hash_2_Bytes(versionHash);

    CBloomArray blooms;

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(tmpFile, modeWriteCreate, LOCK_NOWAIT)) {
        LOG_ERR("Could not lock index file: ", tmpFile);
        return false;
    }

    archive.Seek(0, SEEK_SET);  // write the header even though it's not fully detailed to preserve the space
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)blockTable.size());  // not accurate yet
    for (size_t l = 0; l < lines.size(); l++) {
        string_q line = lines[l];
        ASSERT(countOf(line, '\t') == 2);
        CStringArray parts;
        explode(parts, line, '\t');
        CIndexedAppearance rec(parts[1], parts[2]);
        blockTable.push_back(rec);
        if (!prev.empty() && parts[0] != prev) {
            addToSet(blooms, prev);
            addrbytes_t bytes = addr_2_Bytes(prev);
            archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
            archive.Write(offset);
            archive.Write(cnt);
            offset += cnt;
            cnt = 0;
            nAddrs++;
        }
        cnt++;
        prev = parts[0];
    }

    // The above algo always misses the last address, so we add it here
    addToSet(blooms, prev);
    addrbytes_t bytes = addr_2_Bytes(prev);
    archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
    archive.Write(offset);
    archive.Write(cnt);
    nAddrs++;

    for (auto record : blockTable) {
        archive.Write(record.blk);
        archive.Write(record.txid);
    }

    archive.Seek(0, SEEK_SET);  // re-write the header now that we have full data
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)blockTable.size());
    archive.Release();

    // We've built the data in a temporary file. We do this in case we're interrupted during the building of the
    // data so it's not corrupted. In this way, we only move the data to its final resting place once. It's safer.
    string_q bloomFile = substitute(substitute(outFn, "/finalized/", "/blooms/"), ".bin", ".bloom");
    lockSection();                          // disallow control+c
    writeBloomToBinary(bloomFile, blooms);  // write the bloom file
    copyFile(tmpFile, outFn);               // move the index file
    ::remove(tmpFile.c_str());              // remove the tmp file
    unlockSection();

    return (pinFunc ? ((*pinFunc)(outFn, pinFuncData)) : true);
}