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
    CConsolidator cons(this);
    cons.pin = pin;
    cons.blazeStart = max(cons.ripe, max(cons.staging, cons.finalized)) + 1;
    cons.blazeCnt = block_cnt;
    cons.blazeRipe = (cons.client < unripe_dist ? 0 : cons.client - unripe_dist);
    if ((cons.blazeStart + cons.blazeCnt) > cons.client) {
        ASSERT(blazeStart <= cons.client);  // see above
        cons.blazeCnt = (cons.client - cons.blazeStart);
    }
    distFromHead = (cons.client > cons.blazeStart ? cons.client - cons.blazeStart : 0);

    if (cons.blazeStart > cons.client) {
        LOG_INFO("The index (", cons.blazeStart, ") is ahead of the chain (", cons.client, ").");
        return false;
    }

    // If the user hit control+C somewhere along the way, let's get out of here...
    if (shouldQuit()) {
        LOG_WARN("The user hit control+C...");
        return false;
    }
    ostringstream blazeCmd;
    blazeCmd << "chifra scrape indexer --blaze ";
    blazeCmd << "--start_block " << cons.blazeStart << " ";
    blazeCmd << "--ripe_block " << cons.blazeRipe << " ";
    blazeCmd << "--block_cnt " << cons.blazeCnt << " ";
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

    if (!cons.tmp_stream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        return false;
    }

    if (!forEveryFileInFolder(indexFolder_ripe, copyRipeToStage, &cons)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        cons.tmp_stream.close();
        ::remove(cons.tmp_fn.c_str());
        return false;
    }
    cons.tmp_stream.close();

    if (!cons.stage_chunks()) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }

    if (shouldQuit())
        return false;

    // TODO: BOGUS - we need to turn timestamps on again
    // freshenTimestamps(cons.blazeStart + cons.blazeCnt);

    return cons.consolidate_chunks();
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
