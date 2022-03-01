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

//--------------------------------------------------------------------------
bool COptions::scrape_blocks(void) {
    CMetaData meta = getMetaData();
    prev_block = 0;
    tmp_fn = indexFolder_staging + "000000000-temp.txt";
    tmp_stream.open(tmp_fn, ios::out | ios::trunc);
    blaze_start = max(meta.ripe, max(meta.staging, meta.finalized)) + 1;
    blaze_ripe = (meta.client < unripe_dist ? 0 : meta.client - unripe_dist);
    if ((blaze_start + block_cnt) > meta.client) {
        ASSERT(blaze_start <= meta.client);  // see above
        block_cnt = (meta.client - blaze_start);
    }
    distFromHead = (meta.client > blaze_start ? meta.client - blaze_start : 0);

    if (blaze_start > meta.client) {
        LOG_INFO("The index (", blaze_start, ") is ahead of the chain (", meta.client, ").");
        return false;
    }

    if (shouldQuit()) {
        LOG_WARN("The user hit control+C...");
        return false;
    }

    ostringstream blazeCmd;
    blazeCmd << "chifra scrape indexer --blaze ";
    blazeCmd << "--start_block " << blaze_start << " ";
    blazeCmd << "--ripe_block " << blaze_ripe << " ";
    blazeCmd << "--block_cnt " << block_cnt << " ";
    blazeCmd << "--block_chan_cnt " << block_chan_cnt << " ";
    blazeCmd << "--addr_chan_cnt " << addr_chan_cnt << " ";
    blazeCmd << "--chain " << getChain() << " ";
    blazeCmd << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "");

    if (system(blazeCmd.str().c_str()) != 0) {
        cleanFolder(indexFolder_ripe);
        LOG_WARN("Blaze quit without finishing. Reprocessing...");
        defaultQuitHandler(1);
        return false;
    }

    if (isRunning("acctExport")) {
        LOG_WARN("'chifra export' is running. 'chifra scrape' cannot run at this time...");
        return false;
    }

    if (!tmp_stream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        return false;
    }

    if (!forEveryFileInFolder(indexFolder_ripe, copyRipeToStage, this)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        tmp_stream.close();
        ::remove(tmp_fn.c_str());
        return false;
    }
    tmp_stream.close();

    if (!stage_chunks()) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(tmp_fn.c_str());
    }

    if (shouldQuit())
        return false;

    // TODO: BOGUS - we need to turn timestamps on again
    // freshenTimestamps(blaze_start + cons.blazeCnt);

    return consolidate_chunks();
}

//----------------------------------------------------------------
bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines, CONSTAPPLYFUNC pinFunc, void* pinFuncData) {
    // ASSUMES THE ARRAY IS SORTED!

    ASSERT(!fileExists(outFn));
    string_q tmpFile2 = substitute(outFn, ".bin", ".tmp");

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CIndexedAppearanceArray blockTable;

    hashbytes_t hash = hash_2_Bytes(versionHash);

    CBloomArray blooms;

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(tmpFile2, modeWriteCreate, LOCK_NOWAIT)) {
        LOG_ERR("Could not lock index file: ", tmpFile2);
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
    copyFile(tmpFile2, outFn);              // move the index file
    ::remove(tmpFile2.c_str());             // remove the tmp file
    unlockSection();

    return (pinFunc ? ((*pinFunc)(outFn, pinFuncData)) : true);
}

//---------------------------------------------------------------------------------------------------
bool visitToPin(const string_q& chunkId, void* data) {
    CPinnedChunkArray& pinList = *(CPinnedChunkArray*)data;  // NO_LINT
    CPinnedChunk pinRecord;
    pinlib_pinChunk(pinList, chunkId, pinRecord);
    string_q ci = substitute(pinRecord.fileName, indexFolder_finalized, "");
    ci = substitute(ci, indexFolder_blooms, "");
    ci = substitute(ci, ".bin", "");
    ostringstream os;
    os << ci << "\t" << pinRecord.bloomHash << "\t" << pinRecord.indexHash << endl;
    string_q manifestFile = chainConfigsTxt_manifest;
    os << asciiFileToString(manifestFile);
    stringToAsciiFile(manifestFile, os.str());
    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool copyRipeToStage(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", copyRipeToStage, data);

    } else {
        blknum_t e_unused;
        timestamp_t ts;
        blknum_t bn = path_2_Bn(path, e_unused, ts);

        // If we're not one behind, we have a problem
        COptions* opts = reinterpret_cast<COptions*>(data);

        bool allow = opts->allow_missing;
        bool sequential = (opts->prev_block + 1) == bn;
        bool less_than = (opts->prev_block < bn);
        if ((!allow && !sequential) || (allow && !less_than)) {
            // Something went wrong. We quit here but we will try again next time
            LOG_WARN("Current file (", path, ") does not sequentially follow previous file ", opts->prev_block, ".");
            return false;
        }

        ifstream inputStream(path, ios::in);
        if (!inputStream.is_open()) {
            // Something went wrong, try again next time
            LOG_WARN("Could not open input stream ", path);
            return false;
        }

        lockSection();
        opts->tmp_stream << inputStream.rdbuf();
        opts->tmp_stream.flush();
        inputStream.close();
        ::remove(path.c_str());
        opts->prev_block = bn;
        unlockSection();

        if (bn > opts->first_snap && !(bn % opts->snap_to_grid)) {
            LOG4("  Snapping to block number ", bn);

            // We've been copying each block's data into a temporary file. At this point, we've
            // encountered a block that is a snap-to block (for example, every 100,000 blocks).
            // We write a short chunk here. We do this in order to make correcting errors in the
            // index easier. As we do during normal processing, we clean up of the chunking does
            // not complete properly. 'stage_ chunks' creates the newStage.

            // First, we complete the staging at the current progress. Cleanup and quit if something
            // goes wrong. Quit the scan and start over later...
            if (!opts->stage_chunks()) {
                cleanFolder(indexFolder_unripe);
                cleanFolder(indexFolder_ripe);
                ::remove(opts->tmp_fn.c_str());
                return false;
            }

            // We continually want to check to see if the user has hit control+c so we stay responsive.
            if (shouldQuit())
                return true;

            // This may be less than a single chunk or larger. We want to do at most MAX_ROWS records
            // but we want to do a chunk no matter what (since we've snapped to grid).

            // How many records are there?
            blknum_t nRecords = fileSize(opts->newStage) / asciiAppearanceSize;

            // How big are we going to make this chunk?
            blknum_t chunkSize = min(nRecords, opts->apps_per_chunk);

            // Now we try to write the chunk. We will write at least one chunk no matter how many records there are
            opts->write_chunks(chunkSize, true /* atLeastOnce */);

            // We return false here to pretend this scrape did not complete. We do this because we've actually already
            // written at least one chunk and we know we are at a snap-to boundary. We want the processor to clean up
            // and start fresh next time at the grid location plus one. It will do this naturally if we pretend to
            // have failed processing by returning false;
            return false;
        }
    }

    // Return true if the user has not hit control+c
    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool COptions::consolidate_chunks(void) {
    blknum_t nRecords = fileSize(newStage) / asciiAppearanceSize;
    blknum_t chunkSize = apps_per_chunk;
    if (nRecords <= chunkSize) {
        return true;
    }
    return write_chunks(chunkSize, false /* atLeastOnce */);
}

//--------------------------------------------------------------------------
bool COptions::stage_chunks(void) {
    oldStage = getLastFileInFolder(indexFolder_staging, false);
    newStage = indexFolder_staging + padNum9(prev_block) + ".txt";
    if (oldStage == newStage) {
        return true;
    }

    tmpFile = indexFolder + "temp.txt";
    if (oldStage != tmp_fn) {
        if (!appendFile(tmpFile /* to */, oldStage /* from */)) {
            return false;
        }
    }

    if (!appendFile(tmpFile /* to */, tmp_fn /* from */)) {
        ::remove(tmpFile.c_str());
        return false;
    }

    lockSection();
    ::rename(tmpFile.c_str(), newStage.c_str());
    ::remove(tmp_fn.c_str());
    if (fileExists(oldStage) && oldStage != newStage)
        ::remove(oldStage.c_str());
    unlockSection();

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::write_chunks(blknum_t chunkSize, bool atLeastOnce) {
    blknum_t nRecords = fileSize(newStage) / asciiAppearanceSize;
    while ((atLeastOnce || nRecords > chunkSize) && !shouldQuit()) {
        lockSection();

        CStringArray lines;
        lines.reserve(nRecords + 100);
        asciiFileToLines(newStage, lines);

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
        chunkSize = min(apps_per_chunk, nRecords);
    }

    return true;
}
