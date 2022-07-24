#include "acctlib.h"
#include "bloom.h"

// TODO: BOGUS - TESTING SCRAPING
bool DebuggingOn = fileExists("./testing");

//---------------------------------------------------------------------------------------------------
void Report(const string_q& msg, blknum_t startBlock, uint64_t nAppsPerChunk, uint64_t blockCount, blknum_t nRecsThen,
            blknum_t nRecsNow, bool hide) {
    cerr << "Report: " << string_q(60, '-') << endl << msg << endl;
    cerr << "startBlock:    " << startBlock << endl;
    cerr << "nAppsPerChunk: " << nAppsPerChunk << endl;
    cerr << "blockCount:    " << blockCount << endl;
    cerr << "nRecsThen:     " << nRecsThen << endl;
    cerr << "nRecsNow:      " << nRecsNow << endl;

    if (hide) {
        return;
    }

    if (nRecsNow == nRecsThen) {
        // LOG_INFO("No new blocks...", string_q(80, ' '), "\r");
    } else {
        blknum_t need = nAppsPerChunk >= nRecsNow ? nAppsPerChunk - nRecsNow : 0;
        blknum_t seen = nRecsNow - nRecsThen;
        double pct = double(nRecsNow) / double(nAppsPerChunk);
        double pBlk = double(seen) / double(blockCount);
        string_q result = "C++ --> Block {0}: have {1} addrs of {2} ({3}). Need {4} more. Found {5} records ({6}).";
        replace(result, "{0}", "{" + uint_2_Str(startBlock + blockCount - 1) + "}");
        replace(result, "{1}", "{" + uint_2_Str(nRecsNow) + "}");
        replace(result, "{2}", "{" + uint_2_Str(nAppsPerChunk) + "}");
        replace(result, "{3}", "{" + double_2_Str(pct * 100.00, 1) + "%}");
        replace(result, "{4}", "{" + uint_2_Str(need) + "}");
        replace(result, "{5}", "{" + uint_2_Str(seen) + "}");
        replace(result, "{6}", "{" + double_2_Str(pBlk, 2) + " apps/blk}");
        replaceAll(result, "{", cGreen);
        replaceAll(result, "}", cOff);
        LOG_INFO(result);
    }

    return;
}

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

    string_q tmpStagingFn{""};
    string_q newStage{""};
    blknum_t prev_block{0};
    blknum_t nRecsThen{0};
    blknum_t nRecsNow{0};
    ofstream tmpStagingStream;

    COptions(void) {
        start_block = str_2_Uint(getEnvStr("TB_BLAZE_STARTBLOCK"));
        block_cnt = str_2_Uint(getEnvStr("TB_BLAZE_BLOCKCNT"));
        apps_per_chunk = str_2_Uint(getEnvStr("TB_SETTINGS_APPSPERCHUNK"));
        unripe_dist = str_2_Uint(getEnvStr("TB_SETTINGS_UNRIPEDIST"));
        snap_to_grid = str_2_Uint(getEnvStr("TB_SETTINGS_SNAPTOGRID"));
        first_snap = str_2_Uint(getEnvStr("TB_SETTINGS_FIRSTSNAP"));
        allow_missing = getEnvStr("TB_SETTINGS_ALLOWMISSING") == "true";
        if (DebuggingOn) {
            LOG_INFO("bs-start_block: ", start_block);
            LOG_INFO("bs-block_cnt: ", block_cnt);
            LOG_INFO("bs-apps_per_chunk: ", apps_per_chunk);
            LOG_INFO("bs-unripe_dist: ", unripe_dist);
            LOG_INFO("bs-snap_to_grid: ", snap_to_grid);
            LOG_INFO("bs-first_snap: ", first_snap);
            LOG_INFO("bs-allow_missing: ", allow_missing);
        }
        tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
        tmpStagingStream.open(tmpStagingFn, ios::out | ios::trunc);

        string_q stageFn = getLastFileInFolder(indexFolder_staging, false);
        LOG_INFO("In constructor: ", stageFn);
        prev_block = path_2_Bn(stageFn);
        LOG_INFO("In constructor: ", prev_block);
        nRecsThen = fileSize(stageFn) / asciiAppearanceSize;
        LOG_INFO("In constructor: ", nRecsThen);
    }
    void Cleanup(void) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        tmpStagingStream.close();
        ::remove(tmpStagingFn.c_str());
    }
    bool write_chunks(blknum_t chunkSize, bool snapped);
};

extern bool stage_chunks(COptions* options);
extern bool processSnap(COptions* options, const string_q& path, bool& snapped);
extern bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines);
extern void freshenTimestamps2(blknum_t firstBlock, blknum_t nBlocks);
extern bool appendToStage(COptions* options, const string_q& file);

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    colorsOff();
    LOG_INFO("scrape_blocks");

    COptions options;
    COptions* opts = &options;
    string_q tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
    if (!options.tmpStagingStream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        return EXIT_FAILURE;
    }

    CStringArray files;
    if (!listFilesInFolder(files, indexFolder_ripe, false)) {
        LOG_ERR("Could not list files in ripe folder.");
        options.Cleanup();
        return EXIT_FAILURE;
    }

    Report("Prior --> ", opts->start_block, opts->apps_per_chunk, opts->block_cnt, opts->nRecsThen, opts->nRecsNow,
           true);
    for (auto file : files) {
        if (shouldQuit()) {
            options.Cleanup();
            return EXIT_FAILURE;
        }

        if (!appendToStage(&options, file)) {
            LOG_WARN("Could not open input stream ", file);
            options.Cleanup();
            return EXIT_FAILURE;
        }

        bool snapped = false;
        if (!processSnap(&options, file, snapped)) {
            LOG_INFO("snapped: ", snapped);
            options.Cleanup();
            return snapped ? EXIT_SUCCESS : EXIT_FAILURE;
        }
    }

    Report("Gafter --> ", opts->start_block, opts->apps_per_chunk, opts->block_cnt, opts->nRecsThen, opts->nRecsNow,
           true);

    options.tmpStagingStream.close();
    if (!stage_chunks(&options)) {
        LOG_ERR("stage_chunks returned false");
        options.Cleanup();
        return EXIT_FAILURE;
    }

    freshenTimestamps2(options.start_block, options.block_cnt);
    options.nRecsNow = fileSize(options.newStage) / asciiAppearanceSize;
    uint64_t szBeforeWrite = fileSize(options.newStage);

    int ret = EXIT_SUCCESS;
    if (options.nRecsNow <= options.apps_per_chunk) {
        // do nothing
    } else {
        ret = options.write_chunks(options.apps_per_chunk, false /* snapped */) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    cerr << "C++" << endl;
    cerr << "fileName:         " << options.newStage << endl;
    cerr << "szBeforeWrite:    " << szBeforeWrite << endl;
    cerr << "recordSize:       " << asciiAppearanceSize << endl;
    cerr << "options.nRecsNow: " << options.nRecsNow << endl;
    cerr << "szAfterWrite:     " << fileSize(options.newStage) << endl;
    Report("Finished --> ", opts->start_block, opts->apps_per_chunk, opts->block_cnt, opts->nRecsThen, opts->nRecsNow,
           false);

    return ret;
}

bool appendToStage(COptions* opts, const string_q& file) {
    ifstream inputStream(file, ios::in);
    if (!inputStream.is_open()) {
        return false;
    }
    lockSection();
    opts->tmpStagingStream << inputStream.rdbuf();
    opts->tmpStagingStream.flush();
    inputStream.close();
    ::remove(file.c_str());
    unlockSection();
    return true;
}

//--------------------------------------------------------------------------
bool processSnap(COptions* opts, const string_q& file, bool& snapped) {
    blknum_t bn = path_2_Bn(file);
    opts->prev_block = bn;
    bool isSnapToGrid = (bn > opts->first_snap && !(bn % opts->snap_to_grid));
    if (isSnapToGrid) {
        if (!stage_chunks(opts)) {
            LOG_ERR("stage_chunks returned false");
            return false;
        }
        freshenTimestamps2(opts->start_block, opts->block_cnt);
        opts->nRecsNow = fileSize(opts->newStage) / asciiAppearanceSize;
        uint64_t szBeforeWrite = fileSize(opts->newStage);

        blknum_t chunkSize = min(opts->nRecsNow, opts->apps_per_chunk);
        opts->write_chunks(chunkSize, true /* snapped */);

        cerr << "C++" << endl;
        cerr << "fileName:         " << opts->newStage << endl;
        cerr << "szBeforeWrite:    " << szBeforeWrite << endl;
        cerr << "recordSize:       " << asciiAppearanceSize << endl;
        cerr << "options.nRecsNow: " << opts->nRecsNow << endl;
        cerr << "szAfterWrite:     " << fileSize(opts->newStage) << endl;
        Report("In Snap --> ", opts->start_block, opts->apps_per_chunk, opts->block_cnt, opts->nRecsThen,
               opts->nRecsNow, false);
        snapped = true;
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------
bool appendFile(const string_q& toFile, const string_q& fromFile) {
    ofstream output;
    output.open(toFile, ios::out | ios::app);
    if (!output.is_open())
        return false;

    ifstream input(fromFile, ios::in);
    if (!input.is_open()) {
        output.close();
        return false;
    }

    output << input.rdbuf();
    output.flush();
    input.close();

    return true;
}

//--------------------------------------------------------------------------
bool stage_chunks(COptions* opts) {
    string_q prevStage = getLastFileInFolder(indexFolder_staging, false);
    LOG_INFO("prevStage: ", prevStage);
    opts->newStage = indexFolder_staging + padNum9(opts->prev_block) + ".txt";
    if (prevStage == opts->newStage) {
        return !shouldQuit();
    }

    string_q tmpFile = indexFolder + "temp.txt";
    if (prevStage != opts->tmpStagingFn) {
        if (!appendFile(tmpFile /* to */, prevStage /* from */)) {
            opts->Cleanup();
            ::remove(tmpFile.c_str());
            return false;
        }
    }

    if (!appendFile(tmpFile /* to */, opts->tmpStagingFn /* from */)) {
        opts->Cleanup();
        ::remove(tmpFile.c_str());
        return false;
    }

    lockSection();
    ::rename(tmpFile.c_str(), opts->newStage.c_str());
    ::remove(opts->tmpStagingFn.c_str());
    if (fileExists(prevStage) && prevStage != opts->newStage) {
        ::remove(prevStage.c_str());
    }
    unlockSection();

    return !shouldQuit();
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
                // LOG_INFO("Writing...", string_q(80, ' '), "\r");
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
// TODO: BOGUS - USE THE NEW TIMESTAMP PROCESSOR?
void freshenTimestamps2(blknum_t firstBlock, blknum_t nBlocks) {
    LOG_INFO("pre-freshenTimestamps");
    freshenTimestamps(firstBlock + nBlocks);
    LOG_INFO("post-freshenTimestamps");
}

// bool freshenTimestampsAppend(blknum_t firstBlock, blknum_t nBlocks) {
//     // This routine is called after Blaze finishes. Blaze will have written a text file containing
//     // blockNumber,timestamp pairs that it encountered during it's pass. The job of this
//     // routine is to append timestamps to the timestamps database up to but not
//     // including firstBlock + nBlocks (so 0 and 1, if firstBlock == 0 and nBlocks == 2)
//     // Note that the text file may not contain every block. In fact, on many chains
//     // it does not. This routine has to recitfy that. Plus -- if this routine does not
//     // complete, it may have to start prior to firstBlock the next time, so we have to
//     // check the invariant: sizeof(file) == bn[last].Bn * 2 and bn[i] == i.

//     if (isTestMode())
//         return true;

//     if (!fileExists(indexFolderBin_ts)) {
//         ostringstream cmd;
//         cmd << "cd \"" << indexFolder << "\" ; ";
//         cmd << "cp \"" << chainConfigsZip_ts << "\" . ; ";
//         cmd << "gunzip ts.bin.gz";

//         establishFolder(indexFolder);
//         doCommand(cmd.str());
//         if (!fileExists(indexFolderBin_ts)) {
//             return false;
//         }
//     }

//     if (fileExists(indexFolderBin_ts + ".lck")) {  // it's being updated elsewhere
//         LOG_ERR("Timestamp file ", indexFolderBin_ts, " is locked. Cannot update. Re-running...");
//         ::remove((indexFolderBin_ts + ".lck").c_str());
//         return false;
//     }

//     // If we're already there, we're finished
//     blknum_t lastBlock = firstBlock + nBlocks;
//     size_t nRecords = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
//     if (nRecords >= lastBlock) {
//         return true;
//     }

//     // We always start at one less than the number of blocks already in the file
//     // (i.e., if there's two we need to add block two next.)
//     firstBlock = nRecords;

//     // We need to fill blocks firstBlock to lastBlock (non-inclusive). Make note
//     // of which ones were processed by Blaze
//     string_q blazeTsFilename = cacheFolder_tmp + "tempTsFile.txt";
//     CStringArray lines;
//     asciiFileToLines(blazeTsFilename, lines);
//     // The strings are left-padded with zeros, so we can sort them as strings
//     sort(lines.begin(), lines.end());

//     typedef map<blknum_t, timestamp_t> bn_ts_map_t;
//     bn_ts_map_t theMap;
//     for (auto line : lines) {
//         if (shouldQuit())
//             break;
//         CStringArray parts;
//         explode(parts, line, '-');
//         if (parts.size() == 2) {
//             theMap[str_2_Uint(parts[0])] = str_2_Int(parts[1]);
//         } else {
//             LOG_ERR("Line without dash (-) in temporarty timestamp file. Cannot continue.");
//             break;
//         }
//     }

//     for (blknum_t bn = firstBlock; bn < (firstBlock + nBlocks) && !shouldQuit(); bn++) {
//         if (theMap[bn] == 0) {
//             CBlock block;
//             getBlockHeader(block, bn);
//             theMap[bn] = block.timestamp;
//         }
//     }

//     lockSection();
//     CArchive file(WRITING_ARCHIVE);
//     if (!file.Lock(indexFolderBin_ts, modeWriteAppend, LOCK_NOWAIT)) {
//         LOG_ERR("Failed to open ", indexFolderBin_ts);
//         unlockSection();
//         return false;
//     }

//     for (auto item : theMap) {
//         if (shouldQuit())
//             break;
//         file << ((uint32_t)item.first) << ((uint32_t)item.second);
//         file.flush();
//         if (!DebuggingOn) {
//             ostringstream post;
//             post << " (" << (lastBlock - item.first);
//             post << " " << item.second << " - " << ts_2_Date(item.second).Format(FMT_EXPORT) << ")";
//             post << "             \r";
//             LOG_INFO(UPDATE, item.first, lastBlock, post.str());
//         }
//     }
//     file.Release();
//     unlockSection();

//     return true;
// }
