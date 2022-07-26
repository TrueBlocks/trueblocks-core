#include "acctlib.h"
#include "bloom.h"

// TODO: BOGUS - TESTING SCRAPING
bool DebuggingOn = fileExists("./testing");

//-----------------------------------------------------------------------------
class COptions {
  public:
    uint64_t block_cnt{0};
    uint64_t apps_per_chunk{0};
    uint64_t unripe_dist{0};
    uint64_t snap_to_grid{0};
    uint64_t first_snap{0};
    uint64_t start_block{0};
    uint64_t prev_block{0};
    uint64_t nRecsThen{0};
    uint64_t nRecsNow{0};
    bool allow_missing{false};

    string_q tmpStagingFn{""};
    string_q newStage{""};
    ofstream tmpStagingStream;

    COptions(void) {
        start_block = str_2_Uint(getEnvStr("TB_BLAZE_STARTBLOCK"));
        block_cnt = str_2_Uint(getEnvStr("TB_BLAZE_BLOCKCNT"));
        apps_per_chunk = str_2_Uint(getEnvStr("TB_SETTINGS_APPSPERCHUNK"));
        unripe_dist = str_2_Uint(getEnvStr("TB_SETTINGS_UNRIPEDIST"));
        snap_to_grid = str_2_Uint(getEnvStr("TB_SETTINGS_SNAPTOGRID"));
        first_snap = str_2_Uint(getEnvStr("TB_SETTINGS_FIRSTSNAP"));
        allow_missing = getEnvStr("TB_SETTINGS_ALLOWMISSING") == "true";
        tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
        tmpStagingStream.open(tmpStagingFn, ios::out | ios::trunc);

        string_q stageFn = getLastFileInFolder(indexFolder_staging, false);
        prev_block = path_2_Bn(stageFn);
        nRecsThen = fileSize(stageFn) / asciiAppearanceSize;

        if (DebuggingOn) {
            LOG_INFO("bs-start_block: ", start_block);
            LOG_INFO("bs-block_cnt: ", block_cnt);
            LOG_INFO("bs-apps_per_chunk: ", apps_per_chunk);
            LOG_INFO("bs-unripe_dist: ", unripe_dist);
            LOG_INFO("bs-snap_to_grid: ", snap_to_grid);
            LOG_INFO("bs-first_snap: ", first_snap);
            LOG_INFO("bs-allow_missing: ", allow_missing);
            LOG_INFO("In constructor: ", stageFn);
            LOG_INFO("In constructor: ", prev_block);
            LOG_INFO("In constructor: ", nRecsThen);
        }
    }

    void Cleanup(void) {
        if (DebuggingOn) {
            LOG_INFO("Cleanup");
            LOG_INFO("\tcleanFolder(indexFolder_unripe): ", indexFolder_unripe);
            LOG_INFO("\tcleanFolder(indexFolder_ripe):   ", indexFolder_ripe);
            LOG_INFO("\t::remove(tmpStagingFn.c_str()):  ", tmpStagingFn);
        }
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        tmpStagingStream.close();
        ::remove(tmpStagingFn.c_str());
    }

    bool write_chunks(blknum_t chunkSize, bool snapped);
};

extern bool stage_chunks(COptions* options);
extern bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines);
extern bool appendToStage(COptions* options, const string_q& file);

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    colorsOff();
    LOG_INFO("scrape_blocks");

    COptions options;
    COptions* opts = &options;
    string_q tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
    if (!opts->tmpStagingStream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        return EXIT_FAILURE;
    }

    CStringArray files;
    if (!listFilesInFolder(files, indexFolder_ripe, false)) {
        LOG_ERR("Could not list files in ripe folder.");
        opts->Cleanup();
        return EXIT_FAILURE;
    }

    for (auto file : files) {
        if (shouldQuit()) {
            opts->Cleanup();
            return EXIT_FAILURE;
        }

        if (!appendToStage(opts, file)) {
            LOG_WARN("Could not open input stream ", file);
            opts->Cleanup();
            return EXIT_FAILURE;
        }

        blknum_t bn = path_2_Bn(file);
        opts->prev_block = bn;
        bool isSnapToGrid = (bn > opts->first_snap && !(bn % opts->snap_to_grid));
        if (isSnapToGrid) {
            LOG_INFO("Calling stage_chunks in snapToGrid");
            if (!stage_chunks(opts)) {
                LOG_ERR("stage_chunks returned false");
                opts->Cleanup();
                return EXIT_FAILURE;
            }

            // TODO: BOGUS TURN ON TIMESTAMPS
            LOG_INFO("freshenTimestamps");
            // freshenTimestamps(opts->start_block + opts->block_cnt);

            opts->nRecsNow = fileSize(opts->newStage) / asciiAppearanceSize;
            uint64_t szBeforeWrite = fileSize(opts->newStage);
            blknum_t chunkSize = min(opts->nRecsNow, opts->apps_per_chunk);
            LOG_INFO("chunkSize: ", chunkSize);
            opts->write_chunks(chunkSize, true);
            cerr << "C++" << endl;
            cerr << "fileName:         " << opts->newStage << endl;
            cerr << "szBeforeWrite:    " << szBeforeWrite << endl;
            cerr << "recordSize:       " << asciiAppearanceSize << endl;
            cerr << "opts->nRecsNow:   " << opts->nRecsNow << endl;
            cerr << "szAfterWrite:     " << fileSize(opts->newStage) << endl;
        }
    }

    opts->tmpStagingStream.close();

    LOG_INFO("Calling stage_chunks in regular processing");
    if (!stage_chunks(opts)) {
        LOG_ERR("stage_chunks returned false");
        opts->Cleanup();
        return EXIT_FAILURE;
    }

    // TODO: BOGUS TURN ON TIMESTAMPS
    LOG_INFO("freshenTimestamps");
    // freshenTimestamps(opts->start_block + opts->block_cnt);

    opts->nRecsNow = fileSize(opts->newStage) / asciiAppearanceSize;
    uint64_t szBeforeWrite = fileSize(opts->newStage);
    blknum_t chunkSize = min(opts->nRecsNow, opts->apps_per_chunk);
    LOG_INFO("chunkSize: ", chunkSize);

    if (opts->nRecsNow <= opts->apps_per_chunk) {
        cerr << "C++" << endl;
        cerr << "fileName:         " << opts->newStage << endl;
        cerr << "szBeforeWrite:    " << szBeforeWrite << endl;
        cerr << "recordSize:       " << asciiAppearanceSize << endl;
        cerr << "opts->nRecsNow:   " << opts->nRecsNow << endl;
        cerr << "szAfterWrite:     " << fileSize(opts->newStage) << endl;
        return EXIT_SUCCESS;
    }

    bool ret = opts->write_chunks(opts->apps_per_chunk, false);
    cerr << "C++" << endl;
    cerr << "fileName:         " << opts->newStage << endl;
    cerr << "szBeforeWrite:    " << szBeforeWrite << endl;
    cerr << "recordSize:       " << asciiAppearanceSize << endl;
    cerr << "opts->nRecsNow:   " << opts->nRecsNow << endl;
    cerr << "szAfterWrite:     " << fileSize(opts->newStage) << endl;
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
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

#define LOG_INFO1(a, b) LOG_INFO("\t", a, b)
#define LOG_INFO2(a, b, c) LOG_INFO("\t", a, b, c)
//--------------------------------------------------------------------------
bool stage_chunks(COptions* opts) {
    string_q prevStage = getLastFileInFolder(indexFolder_staging, false);
    opts->newStage = indexFolder_staging + padNum9(opts->prev_block) + ".txt";
    LOG_INFO1("prevStage:          ", prevStage);
    LOG_INFO1("opts->newStage:     ", opts->newStage);

    if (prevStage == opts->newStage) {
        LOG_INFO("prevStage == opts->newStage - No Cleanup");
        return true;
    }

    string_q tmpFile = indexFolder + "temp.txt";
    LOG_INFO1("tmpFile:            ", tmpFile);
    LOG_INFO("opts->tmpStagingFn: ", opts->tmpStagingFn);
    if (prevStage != opts->tmpStagingFn) {
        LOG_INFO("prevStage != opts->tmpStagingFn");
        if (!appendFile(tmpFile /* to */, prevStage /* from */)) {
            LOG_INFO("!appendFile1 -- Cleanup -- remove(tmpFile)");
            opts->Cleanup();
            ::remove(tmpFile.c_str());
            return false;
        }
    }

    if (!appendFile(tmpFile /* to */, opts->tmpStagingFn /* from */)) {
        LOG_INFO("!appendFile1 -- Cleanup -- remove(tmpFile)");
        opts->Cleanup();
        ::remove(tmpFile.c_str());
        return false;
    }

    lockSection();
    LOG_INFO1("opts->tmpStagingFn: ", opts->tmpStagingFn);
    LOG_INFO1("rename tmpFile to opts->newStage", "");
    LOG_INFO1("                    ", tmpFile);
    LOG_INFO1("                    ", opts->newStage);
    ::rename(tmpFile.c_str(), opts->newStage.c_str());
    LOG_INFO1("remove tmpStagingFn", "");
    LOG_INFO1("                    ", opts->tmpStagingFn);
    ::remove(opts->tmpStagingFn.c_str());
    if (fileExists(prevStage) && prevStage != opts->newStage) {
        LOG_INFO("!appendFile1 -- Cleanup -- remove(tmpFile)");
        ::remove(prevStage.c_str());
    }
    unlockSection();

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

    return true;
}
