#include "acctlib.h"
#include "bloom.h"

// TODO: BOGUS - TESTING SCRAPING
bool OnOff = fileExists("./testing");

//-----------------------------------------------------------------------------
class COptions {
  public:
    uint64_t block_cnt{0};
    uint64_t apps_per_chunk{0};
    uint64_t unripe_dist{0};
    uint64_t snap_to_grid{0};
    uint64_t first_snap{0};
    bool allow_missing{false};
    blknum_t start_block{0};

    string_q newStage{""};
    blknum_t prev_block{0};
    blknum_t nRecsThen{0};
    blknum_t nRecsNow{0};
    ofstream tmpStagingStream;

    bool stage_chunks(const string_q& tmpFn);
    bool write_chunks(blknum_t chunkSize, bool snapped);
    bool copyRipeToStage(const string_q& path, bool& snapped);
    bool report(uint64_t nRecsThen, uint64_t nRecsNow) const;

    COptions(void) {
    }
};

//--------------------------------------------------------------------------
bool COptions::copyRipeToStage(const string_q& path, bool& snapped) {
    blknum_t bn = path_2_Bn(path);
    bool allow = allow_missing;
    bool sequential = (prev_block + 1) == bn;
    bool less_than = (prev_block < bn);
    if (prev_block != 0 && ((!allow && !sequential) || (allow && !less_than))) {
        LOG_WARN("prev_block: ", prev_block);
        LOG_WARN("allow: ", allow, allow_missing);
        LOG_WARN("sequential: ", sequential, " ", prev_block + 1, " ", bn, " ", (prev_block + 1 == bn));
        LOG_WARN("less_than: ", less_than, " ", prev_block, " ", bn, " ", prev_block < bn);
        LOG_WARN("Current file (", path, ") does not sequentially follow previous file ", prev_block, ".");
        return false;
    }

    ifstream inputStream(path, ios::in);
    if (!inputStream.is_open()) {
        LOG_WARN("Could not open input stream ", path);
        return false;
    }

    lockSection();
    tmpStagingStream << inputStream.rdbuf();
    tmpStagingStream.flush();
    inputStream.close();
    ::remove(path.c_str());
    prev_block = bn;
    unlockSection();

    bool isSnapToGrid = (bn > first_snap && !(bn % snap_to_grid));
    if (isSnapToGrid) {
        string_q tmpStagingFn = (indexFolder_staging + "000000000-temp.txt");
        if (!stage_chunks(tmpStagingFn)) {
            return false;
        }

        if (!isTestMode()) {
            // TODO: BOGUS - USE THE NEW TIMESTAMP PROCESSOR?
            LOG_INFO("pre-freshenTimestamps");
            // freshenTimestampsAppend(start_block, block_cnt);
            freshenTimestamps(start_block + block_cnt);
            LOG_INFO("post-freshenTimestamps");
        }
        nRecsNow = fileSize(newStage) / asciiAppearanceSize;
        report(nRecsThen, nRecsNow);
        blknum_t chunkSize = min(nRecsNow, apps_per_chunk);
        write_chunks(chunkSize, true /* snapped */);
        snapped = true;
        return false;
    }
    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool COptions::stage_chunks(const string_q& tmpFn) {
    string_q prevStage = getLastFileInFolder(indexFolder_staging, false);
    newStage = indexFolder_staging + padNum9(prev_block) + ".txt";
    if (prevStage == newStage) {
        return !shouldQuit();
    }

    string_q tmpFile = indexFolder + "temp.txt";
    if (prevStage != tmpFn) {
        if (!appendFile(tmpFile /* to */, prevStage /* from */)) {
            cleanFolder(indexFolder_unripe);
            cleanFolder(indexFolder_ripe);
            ::remove(tmpFile.c_str());
            ::remove(tmpFn.c_str());
            return false;
        }
    }

    LOG_INFO("pre-append");
    if (!appendFile(tmpFile /* to */, tmpFn /* from */)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(tmpFile.c_str());
        ::remove(tmpFn.c_str());
        return false;
    }
    LOG_INFO("post-append");

    lockSection();
    ::rename(tmpFile.c_str(), newStage.c_str());
    ::remove(tmpFn.c_str());
    if (fileExists(prevStage) && prevStage != newStage)
        ::remove(prevStage.c_str());
    unlockSection();
    LOG_INFO("post-lock");
    return !shouldQuit();
}

//---------------------------------------------------------------------------------------------------
bool COptions::report(uint64_t nRecsThen2, uint64_t nRecsNow2) const {
    blknum_t found2 = nRecsNow2 - nRecsThen2;
    if (!found2) {
        LOG_INFO("No new blocks...", string_q(80, ' '), "\r");
        return true;
    }

    blknum_t need = apps_per_chunk >= nRecsNow2 ? apps_per_chunk - nRecsNow2 : 0;
    double pct = double(nRecsNow2) / double(apps_per_chunk);
    double pBlk = double(found2) / double(block_cnt);

    string_q result = "Block {0}: have {1} addrs of {2} ({3}). Need {4} more. Found {5} records ({6}).";
    replace(result, "{0}", "{" + uint_2_Str(start_block + block_cnt - 1) + "}");
    replace(result, "{1}", "{" + uint_2_Str(nRecsNow2) + "}");
    replace(result, "{2}", "{" + uint_2_Str(apps_per_chunk) + "}");
    replace(result, "{3}", "{" + double_2_Str(pct * 100.00, 1) + "%}");
    replace(result, "{4}", "{" + uint_2_Str(need) + "}");
    replace(result, "{5}", "{" + uint_2_Str(found2) + "}");
    replace(result, "{6}", "{" + double_2_Str(pBlk, 2) + " apps/blk}");
    replaceAll(result, "{", cGreen);
    replaceAll(result, "}", cOff);
    LOG_INFO(result);

    return true;
}

//---------------------------------------------------------------------------
class CBloomFilterWrite {
  public:
    vector<bloom_t> array;
    bool addToSet(const address_t& addr);
};

#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define BLOOM_K 5

//----------------------------------------------------------------------
bool CBloomFilterWrite::addToSet(const address_t& addr) {
    if (array.size() == 0) {
        array.push_back(bloom_t());  // so we have something to add to
    }

    CUintArray bitsLit;
    getLitBits(addr, bitsLit);
    for (auto bit : bitsLit) {
        array[array.size() - 1].lightBit(bit);
    }
    array[array.size() - 1].nInserted++;

    if (array[array.size() - 1].nInserted > MAX_ADDRS_IN_BLOOM)
        array.push_back(bloom_t());

    return true;
}

//----------------------------------------------------------------
bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines) {
    ASSERT(!fileExists(outFn));

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CIndexedAppearanceArray appTable;

    // TODO: BOGUS - MANIFEST WRITING HASH INTO INDEX
    hashbytes_t hash = hash_2_Bytes(padLeft(keccak256(manifestVersion), 64, '0'));

    CArchive archive(WRITING_ARCHIVE);
    string_q tmpFile2 = substitute(outFn, ".bin", ".tmp");
    if (!archive.Lock(tmpFile2, modeWriteCreate, LOCK_NOWAIT)) {
        LOG_ERR("Could not lock index file: ", tmpFile2);
        return false;
    }

    archive.Seek(0, SEEK_SET);  // write the header even though it's not fully detailed to preserve the space
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)appTable.size());  // not accurate yet

    CBloomFilterWrite bloomFilter;
    for (size_t l = 0; l < lines.size(); l++) {
        string_q line = lines[l];
        ASSERT(countOf(line, '\t') == 2);
        CStringArray parts;
        explode(parts, line, '\t');
        CIndexedAppearance rec(parts[1], parts[2]);
        appTable.push_back(rec);
        if (!prev.empty() && parts[0] != prev) {
            bloomFilter.addToSet(prev);
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
    bloomFilter.addToSet(prev);
    addrbytes_t bytes = addr_2_Bytes(prev);
    archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
    archive.Write(offset);
    archive.Write(cnt);
    nAddrs++;

    for (auto record : appTable) {
        archive.Write(record.blk);
        archive.Write(record.txid);
    }

    archive.Seek(0, SEEK_SET);  // re-write the header now that we have full data
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)appTable.size());
    archive.Release();

    lockSection();

    CArchive output(WRITING_ARCHIVE);
    string_q bloomFile = substitute(substitute(outFn, "/finalized/", "/blooms/"), ".bin", ".bloom");
    if (output.Lock(bloomFile, modeWriteCreate, LOCK_NOWAIT)) {
        output.Write((uint32_t)bloomFilter.array.size());
        for (auto bloom : bloomFilter.array) {
            output.Write(bloom.nInserted);
            output.Write(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
        }
        output.Release();

        copyFile(tmpFile2, outFn);
        ::remove(tmpFile2.c_str());

        string_q range = substitute(substitute(outFn, indexFolder_finalized, ""), indexFolder_blooms, "");
        range = substitute(substitute(range, ".bin", ""), ".bloom", "");
        appendToAsciiFile(cacheFolder_tmp + "chunks_created.txt", range + "\n");
    }

    unlockSection();
    return !shouldQuit();
}

//---------------------------------------------------------------------------------------------------
bool COptions::write_chunks(blknum_t chunkSize, bool snapped) {
    LOG_INFO("write_chunks");
    blknum_t nRecords = fileSize(newStage) / asciiAppearanceSize;
    while ((snapped || nRecords > chunkSize) && !shouldQuit()) {
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
                LOG_INFO("Writing...", string_q(80, ' '), "\r");
                writeIndexAsBinary(chunkPath, consolidatedLines);
                ostringstream os;
                os << "Wrote " << consolidatedLines.size() << " records to " << cTeal << relativize(chunkPath);
                if (snapped && (lines.size() - 1 == loc)) {
                    os << cYellow << " (snapped to " << snap_to_grid << " blocks)";
                }
                os << cOff;
                LOG_INFO(os.str());

                loc++;

                for (uint64_t record = loc; record < lines.size(); record++)
                    remainingLines.push_back(lines[record]);

                ::remove(newStage.c_str());
            }
        }

        if (remainingLines.size()) {
            linesToAsciiFile(newStage, remainingLines);
        }
        unlockSection();

        nRecords = fileSize(newStage) / asciiAppearanceSize;
        if (snapped)
            snapped = nRecords > 0;
        chunkSize = min(apps_per_chunk, nRecords);
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool freshenTimestampsAppend(blknum_t firstBlock, blknum_t nBlocks) {
    // This routine is called after Blaze finishes. Blaze will have written a text file containing
    // blockNumber,timestamp pairs that it encountered during it's pass. The job of this
    // routine is to append timestamps to the timestamps database up to but not
    // including firstBlock + nBlocks (so 0 and 1, if firstBlock == 0 and nBlocks == 2)
    // Note that the text file may not contain every block. In fact, on many chains
    // it does not. This routine has to recitfy that. Plus -- if this routine does not
    // complete, it may have to start prior to firstBlock the next time, so we have to
    // check the invariant: sizeof(file) == bn[last].Bn * 2 and bn[i] == i.

    if (isTestMode())
        return true;

    if (!fileExists(indexFolderBin_ts)) {
        ostringstream cmd;
        cmd << "cd \"" << indexFolder << "\" ; ";
        cmd << "cp \"" << chainConfigsZip_ts << "\" . ; ";
        cmd << "gunzip ts.bin.gz";

        establishFolder(indexFolder);
        doCommand(cmd.str());
        if (!fileExists(indexFolderBin_ts)) {
            return false;
        }
    }

    if (fileExists(indexFolderBin_ts + ".lck")) {  // it's being updated elsewhere
        LOG_ERR("Timestamp file ", indexFolderBin_ts, " is locked. Cannot update. Re-running...");
        ::remove((indexFolderBin_ts + ".lck").c_str());
        return false;
    }

    // If we're already there, we're finished
    blknum_t lastBlock = firstBlock + nBlocks;
    size_t nRecords = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
    if (nRecords >= lastBlock) {
        return true;
    }

    // We always start at one less than the number of blocks already in the file
    // (i.e., if there's two we need to add block two next.)
    firstBlock = nRecords;

    // We need to fill blocks firstBlock to lastBlock (non-inclusive). Make note
    // of which ones were processed by Blaze
    string_q blazeTsFilename = cacheFolder_tmp + "tempTsFile.txt";
    CStringArray lines;
    asciiFileToLines(blazeTsFilename, lines);
    // The strings are left-padded with zeros, so we can sort them as strings
    sort(lines.begin(), lines.end());

    typedef map<blknum_t, timestamp_t> bn_ts_map_t;
    bn_ts_map_t theMap;
    for (auto line : lines) {
        if (shouldQuit())
            break;
        CStringArray parts;
        explode(parts, line, '-');
        if (parts.size() == 2) {
            theMap[str_2_Uint(parts[0])] = str_2_Int(parts[1]);
        } else {
            LOG_ERR("Line without dash (-) in temporarty timestamp file. Cannot continue.");
            break;
        }
    }

    for (blknum_t bn = firstBlock; bn < (firstBlock + nBlocks) && !shouldQuit(); bn++) {
        if (theMap[bn] == 0) {
            CBlock block;
            getBlockHeader(block, bn);
            theMap[bn] = block.timestamp;
        }
    }

    lockSection();
    CArchive file(WRITING_ARCHIVE);
    if (!file.Lock(indexFolderBin_ts, modeWriteAppend, LOCK_NOWAIT)) {
        LOG_ERR("Failed to open ", indexFolderBin_ts);
        unlockSection();
        return false;
    }

    for (auto item : theMap) {
        if (shouldQuit())
            break;
        file << ((uint32_t)item.first) << ((uint32_t)item.second);
        file.flush();
        if (!OnOff) {
            ostringstream post;
            post << " (" << (lastBlock - item.first);
            post << " " << item.second << " - " << ts_2_Date(item.second).Format(FMT_EXPORT) << ")";
            post << "             \r";
            LOG_INFO(UPDATE, item.first, lastBlock, post.str());
        }
    }
    file.Release();
    unlockSection();

    return true;
}

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    colorsOff();
    COptions options;
    LOG_INFO("scrape_blocks");
    options.start_block = str_2_Uint(getEnvStr("TB_BLAZE_STARTBLOCK"));
    options.block_cnt = str_2_Uint(getEnvStr("TB_BLAZE_BLOCKCNT"));
    options.apps_per_chunk = str_2_Uint(getEnvStr("TB_SETTINGS_APPSPERCHUNK"));
    options.unripe_dist = str_2_Uint(getEnvStr("TB_SETTINGS_UNRIPEDIST"));
    options.snap_to_grid = str_2_Uint(getEnvStr("TB_SETTINGS_SNAPTOGRID"));
    options.first_snap = str_2_Uint(getEnvStr("TB_SETTINGS_FIRSTSNAP"));
    options.allow_missing = getEnvStr("TB_SETTINGS_ALLOWMISSING") == "true";

    if (OnOff) {
        LOG_INFO("bs-start_block: ", options.start_block);
        LOG_INFO("bs-block_cnt: ", options.block_cnt);
        LOG_INFO("bs-apps_per_chunk: ", options.apps_per_chunk);
        LOG_INFO("bs-unripe_dist: ", options.unripe_dist);
        LOG_INFO("bs-snap_to_grid: ", options.snap_to_grid);
        LOG_INFO("bs-first_snap: ", options.first_snap);
        LOG_INFO("bs-allow_missing: ", options.allow_missing);
    }

    string_q tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
    options.tmpStagingStream.open(tmpStagingFn, ios::out | ios::trunc);
    if (!options.tmpStagingStream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        return EXIT_FAILURE;
    }

    string_q stageFn = getLastFileInFolder(indexFolder_staging, false);
    options.prev_block = path_2_Bn(stageFn);
    options.nRecsThen = fileSize(stageFn) / asciiAppearanceSize;

    CStringArray files;
    if (!listFilesInFolder(files, indexFolder_ripe, false)) {
        LOG_ERR("Could not list files in ripe folder.");
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        options.tmpStagingStream.close();
        ::remove(tmpStagingFn.c_str());
        return EXIT_FAILURE;
    }

    bool snapped = false;
    for (auto file : files) {
        if (!options.copyRipeToStage(file, snapped)) {
            cleanFolder(indexFolder_unripe);
            cleanFolder(indexFolder_ripe);
            options.tmpStagingStream.close();
            ::remove(tmpStagingFn.c_str());
            if (!snapped) {
                LOG_WARN("copyRipeToStage failed...");
            }
            LOG_INFO("snapped: ", snapped);
            return snapped;
        }
    }
    LOG_INFO("snapped: ", snapped);
    options.tmpStagingStream.close();

    if (!options.stage_chunks(tmpStagingFn))
        return EXIT_FAILURE;

    // TODO: BOGUS - USE THE NEW TIMESTAMP PROCESSOR?
    LOG_INFO("pre-freshenTimestamps");
    // freshenTimestampsAppend(start_block, block_cnt);
    freshenTimestamps(options.start_block + options.block_cnt);
    LOG_INFO("post-freshenTimestamps");

    options.nRecsNow = fileSize(options.newStage) / asciiAppearanceSize;
    options.report(options.nRecsThen, options.nRecsNow);
    if (options.nRecsNow <= options.apps_per_chunk)
        return EXIT_SUCCESS;

    return options.write_chunks(options.apps_per_chunk, false /* snapped */) ? EXIT_SUCCESS : EXIT_FAILURE;
}
