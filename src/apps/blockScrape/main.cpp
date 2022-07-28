#include "acctlib.h"
#include "bloom.h"

// TODO: BOGUS - TESTING SCRAPING
bool DebuggingOn = fileExists("./testing");

size_t fff = 52;
char x = '-';
#define LOG_IN(a, f)                                                                                                   \
    string_q nm = (f);                                                                                                 \
    if ((a) != x) {                                                                                                    \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
    }                                                                                                                  \
    LOG_INFO(string_q(12, (a)), " in ", (nm), " ", string_q(fff, (a)));

#define LOG_OUT(a)                                                                                                     \
    LOG_INFO(string_q(12, (a)), " out ", (nm), " ", string_q(fff, (a)));                                               \
    if ((a) != x) {                                                                                                    \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
    }

#define LOG_FILE(a, b) LOG_INFO((a), substitute((b), "/Users/jrush/Data/trueblocks/unchained/", "./"))

//-----------------------------------------------------------------------------
class COptions {
  public:
    uint64_t apps_per_chunk{0};
    uint64_t unripe_dist{0};
    uint64_t snap_to_grid{0};
    uint64_t first_snap{0};
    uint64_t start_block{0};
    uint64_t prev_block{0};
    uint64_t nRecsThen{0};
    uint64_t nRecsNow{0};

    string_q tmpStagingFn{""};
    string_q newStage{""};
    ofstream tmpStagingStream;

    COptions(void) {
        start_block = str_2_Uint(getEnvStr("TB_BLAZE_STARTBLOCK"));
        apps_per_chunk = str_2_Uint(getEnvStr("TB_SETTINGS_APPSPERCHUNK"));
        unripe_dist = str_2_Uint(getEnvStr("TB_SETTINGS_UNRIPEDIST"));
        snap_to_grid = str_2_Uint(getEnvStr("TB_SETTINGS_SNAPTOGRID"));
        first_snap = str_2_Uint(getEnvStr("TB_SETTINGS_FIRSTSNAP"));
        tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
        tmpStagingStream.open(tmpStagingFn, ios::out | ios::trunc);

        string_q stageFn = getLastFileInFolder(indexFolder_staging, false);
        prev_block = path_2_Bn(stageFn);
        nRecsThen = fileSize(stageFn) / asciiAppearanceSize;

        if (DebuggingOn) {
            LOG_INFO("startBlock:          ", start_block);
            LOG_INFO("nAppsPerChunk:       ", apps_per_chunk);
            LOG_INFO("unripeDist:          ", unripe_dist);
            LOG_INFO("snapToGrid:          ", snap_to_grid);
            LOG_INFO("firstSnap:           ", first_snap);
            LOG_FILE("tmpStagingFn:        ", tmpStagingFn);
            LOG_FILE("stageFn:             ", stageFn);
            LOG_INFO("prevBlocks:          ", prev_block);
            LOG_INFO("nRecsThen:           ", nRecsThen);
        }
    }

    void Cleanup(void) {
        if (DebuggingOn) {
            LOG_INFO("Clean folders");
            LOG_INFO("  index_unripe:      ", indexFolder_unripe);
            LOG_INFO("  index_ripe:        ", indexFolder_ripe);
            LOG_FILE("  tmpStagingFn:      ", tmpStagingFn);
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
    if (DebuggingOn) {
        colorsOff();
    }
    LOG_IN('=', "main")

    COptions options;
    COptions* opts = &options;
    string_q tmpStagingFn = indexFolder_staging + "000000000-temp.txt";
    if (!opts->tmpStagingStream.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        LOG_OUT('=')
        return EXIT_FAILURE;
    }

    CStringArray files;
    if (!listFilesInFolder(files, indexFolder_ripe, false)) {
        // There is nothing to process, not an error - we're at the front of the chain
        opts->Cleanup();
        LOG_OUT('=')
        return EXIT_FAILURE;
    }

    for (auto file : files) {
        if (shouldQuit()) {
            opts->Cleanup();
            LOG_OUT('=')
            return EXIT_FAILURE;
        }

        if (!appendToStage(opts, file)) {
            LOG_WARN("Could not open input stream ", file);
            opts->Cleanup();
            LOG_OUT('=')
            return EXIT_FAILURE;
        }

        blknum_t bn = path_2_Bn(file);
        opts->prev_block = bn;
        bool isSnapToGrid = (bn > opts->first_snap && !(bn % opts->snap_to_grid));
        if (isSnapToGrid) {
            if (DebuggingOn) {
                LOG_INFO("Calling stage_chunks in snapToGrid");
            }
            if (!stage_chunks(opts)) {
                LOG_ERR("stage_chunks returned false");
                opts->Cleanup();
                LOG_OUT('=')
                return EXIT_FAILURE;
            }

            opts->nRecsNow = fileSize(opts->newStage) / asciiAppearanceSize;
            uint64_t szBeforeWrite = fileSize(opts->newStage);
            blknum_t chunkSize = min(opts->nRecsNow, opts->apps_per_chunk);
            opts->write_chunks(chunkSize, true);
            if (DebuggingOn) {
                LOG_INFO("chunkSize:            ", min(opts->nRecsNow, opts->apps_per_chunk));
                LOG_FILE("fileName:             ", opts->newStage);
                LOG_INFO("szBeforeWrite:        ", szBeforeWrite);
                LOG_INFO("recordSize:           ", asciiAppearanceSize);
                LOG_INFO("opts->nRecsNow:       ", opts->nRecsNow);
                LOG_INFO("szAfterWrite:         ", fileSize(opts->newStage));
            }
        }
    }

    opts->tmpStagingStream.close();

    if (!stage_chunks(opts)) {
        LOG_ERR("stage_chunks returned false");
        opts->Cleanup();
        LOG_OUT('=')
        return EXIT_FAILURE;
    }

    opts->nRecsNow = fileSize(opts->newStage) / asciiAppearanceSize;
    uint64_t szBeforeWrite = fileSize(opts->newStage);
    if (opts->nRecsNow <= opts->apps_per_chunk) {
        if (DebuggingOn) {
            LOG_INFO("chunkSize:            ", min(opts->nRecsNow, opts->apps_per_chunk));
            LOG_FILE("fileName:             ", opts->newStage);
            LOG_INFO("szBeforeWrite:        ", szBeforeWrite);
            LOG_INFO("recordSize:           ", asciiAppearanceSize);
            LOG_INFO("opts->nRecsNow:       ", opts->nRecsNow);
            LOG_INFO("szAfterWrite:         ", fileSize(opts->newStage));
        }
        LOG_OUT('=')
        return EXIT_SUCCESS;
    }

    bool ret = opts->write_chunks(opts->apps_per_chunk, false);
    if (DebuggingOn) {
        LOG_INFO("chunkSize:            ", min(opts->nRecsNow, opts->apps_per_chunk));
        LOG_FILE("fileName:             ", opts->newStage);
        LOG_INFO("szBeforeWrite:        ", szBeforeWrite);
        LOG_INFO("recordSize:           ", asciiAppearanceSize);
        LOG_INFO("opts->nRecsNow:       ", opts->nRecsNow);
        LOG_INFO("szAfterWrite:         ", fileSize(opts->newStage));
    }
    LOG_OUT('=')
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}

//--------------------------------------------------------------------------
bool appendToStage(COptions* opts, const string_q& file) {
    // LOG_IN('-', "appendToStage");
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
    // LOG_IN('-', "appendFile");
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
    LOG_IN('-', "stage_chunks");
    string_q prevStage = getLastFileInFolder(indexFolder_staging, false);
    opts->newStage = indexFolder_staging + padNum9(opts->prev_block) + ".txt";
    if (DebuggingOn) {
        LOG_FILE("\tprevStage:          ", prevStage);
        LOG_FILE("\topts->newStage:     ", opts->newStage);
    }

    if (prevStage == opts->newStage) {
        LOG_INFO("prevStage == opts->newStage - No Cleanup");
        LOG_OUT('-')
        return true;
    }

    string_q tmpFile = indexFolder + "temp.txt";
    if (DebuggingOn) {
        LOG_FILE("\ttmpFile:            ", tmpFile);
        LOG_FILE("\topts->tmpStagingFn: ", opts->tmpStagingFn);
    }
    if (prevStage != opts->tmpStagingFn) {
        if (DebuggingOn) {
            LOG_INFO("prevStage != opts->tmpStagingFn");
        }
        if (!appendFile(tmpFile /* to */, prevStage /* from */)) {
            if (DebuggingOn) {
                LOG_INFO("!appendFile1 -- remove(tmpFile)");
            }
            ::remove(tmpFile.c_str());
            LOG_OUT('-')
            return false;
        }
    }

    if (!appendFile(tmpFile /* to */, opts->tmpStagingFn /* from */)) {
        LOG_INFO("!appendFile2 -- remove(tmpFile)");
        ::remove(tmpFile.c_str());
        LOG_OUT('-')
        return false;
    }

    lockSection();
    if (DebuggingOn) {
        LOG_FILE("\topts->tmpStagingFn: ", opts->tmpStagingFn);
        LOG_FILE("\trename              ", tmpFile);
        LOG_FILE("\t  to                ", opts->newStage);
    }
    ::rename(tmpFile.c_str(), opts->newStage.c_str());
    if (DebuggingOn) {
        LOG_FILE("\tremove tmpStagingFn ", opts->tmpStagingFn);
    }
    ::remove(opts->tmpStagingFn.c_str());
    if (fileExists(prevStage) && prevStage != opts->newStage) {
        if (DebuggingOn) {
            LOG_INFO("!appendFile1 -- Cleanup -- remove(tmpFile)");
        }
        ::remove(prevStage.c_str());
    }
    unlockSection();

    LOG_OUT('-')
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
    LOG_IN('-', "writeIndexAsBinary");
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
        LOG_OUT('-')
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
    LOG_OUT('-')
    return !shouldQuit();
}

//---------------------------------------------------------------------------------------------------
bool COptions::write_chunks(blknum_t chunkSize, bool snapped) {
    LOG_IN('-', "write_chunks");

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

                for (uint64_t record = loc; record < lines.size(); record++) {
                    remainingLines.push_back(lines[record]);
                }
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

    LOG_OUT('-')
    return true;
}
