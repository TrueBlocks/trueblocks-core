#include "acctlib.h"

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
        LOG_INFO("");                                                                                                  \
    }                                                                                                                  \
    LOG_INFO(string_q(12, (a)), " in ", (nm), " ", string_q(fff, (a)));

#define LOG_OUT(a, b)                                                                                                  \
    LOG_INFO(string_q(12, (a)), " out ", (nm), " ", (b), " ", string_q(fff, (a)));                                     \
    if ((a) != x) {                                                                                                    \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
        LOG_INFO("");                                                                                                  \
    }

#define LOG_FILE(a, b) LOG_INFO((a), substitute((b), "/Users/jrush/Data/trueblocks/unchained/", "./"))

//---------------------------------------------------------------------------
class bloom_t {
  public:
    uint32_t nInserted;
    uint8_t* bits;

  public:
    bloom_t(void);
    ~bloom_t(void);
    bloom_t(const bloom_t& b);
    void lightBit(size_t bit);
};

//-----------------------------------------------------------------------------
class COptions {
  public:
    uint64_t apps_per_chunk{0};
    uint64_t unripe_dist{0};
    uint64_t snap_to_grid{0};
    uint64_t first_snap{0};
    uint64_t start_block{0};
    uint64_t prev_block{0};

    string_q newStage{""};
    string_q stageStreamFn{""};
    ofstream stageStream;

    COptions(void) {
        start_block = str_2_Uint(getEnvStr("TB_BLAZE_STARTBLOCK"));
        apps_per_chunk = str_2_Uint(getEnvStr("TB_SETTINGS_APPSPERCHUNK"));
        unripe_dist = str_2_Uint(getEnvStr("TB_SETTINGS_UNRIPEDIST"));
        snap_to_grid = str_2_Uint(getEnvStr("TB_SETTINGS_SNAPTOGRID"));
        first_snap = str_2_Uint(getEnvStr("TB_SETTINGS_FIRSTSNAP"));
        stageStreamFn = indexFolder_staging + "000000000-temp.txt";
        prev_block = start_block - 1;

        if (DebuggingOn) {
            LOG_INFO("startBlock:          ", start_block);
            LOG_INFO("nAppsPerChunk:       ", apps_per_chunk);
            LOG_INFO("unripeDist:          ", unripe_dist);
            LOG_INFO("snapToGrid:          ", snap_to_grid);
            LOG_INFO("firstSnap:           ", first_snap);
            LOG_FILE("stageStreamFn:       ", stageStreamFn);
            LOG_INFO("prevBlocks:          ", prev_block);
        }
    }

    void Cleanup(void) {
        if (DebuggingOn) {
            LOG_INFO("Clean folders");
            LOG_INFO("  index_unripe:      ", indexFolder_unripe);
            LOG_INFO("  index_ripe:        ", indexFolder_ripe);
            LOG_FILE("  stageStreamFn:     ", stageStreamFn);
        }
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);

        stageStream.close();
        ::remove(stageStreamFn.c_str());
    }
};

#define BLOOM_WIDTH_IN_BYTES (1048576 / 8)
#define BLOOM_WIDTH_IN_BITS (BLOOM_WIDTH_IN_BYTES * 8)
#define MAX_ADDRS_IN_BLOOM 50000
#define BLOOM_K 5

//---------------------------------------------------------------------------
void getLitBits(const address_t& addrIn, CUintArray& litBitsOut) {
#define EXTRACT_WID 8
    for (size_t k = 0; k < BLOOM_K; k++) {
        string_q fourByte = ("0x" + extract(addrIn, 2 + (k * EXTRACT_WID), EXTRACT_WID));
        uint64_t bit = (str_2_Uint(fourByte) % BLOOM_WIDTH_IN_BITS);
        litBitsOut.push_back(bit);
    }
    return;
}

//----------------------------------------------------------------------
bool addToSet(vector<bloom_t>& array, const address_t& addr) {
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

//---------------------------------------------------------------------------------------------------
bool write_chunks(COptions* opts, blknum_t chunkSize, bool isSnapToGrid) {
    blknum_t nRecords = fileSize(opts->newStage) / asciiAppearanceSize;
    while ((isSnapToGrid || nRecords > chunkSize)) {
        CStringArray lines;
        lines.reserve(nRecords + 100);
        asciiFileToLines(opts->newStage, lines);
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
                ASSERT(!fileExists(chunkPath));

                address_t prev;
                uint32_t offset = 0, nAddrs = 0, cnt = 0;
                CIndexedAppearanceArray appTable;

                // TODO: BOGUS - MANIFEST WRITING HASH INTO INDEX
                hashbytes_t hash = hash_2_Bytes(padLeft(keccak256(manifestVersion), 64, '0'));

                CArchive archive(WRITING_ARCHIVE);
                string_q tmpFile2 = substitute(chunkPath, ".bin", ".tmp");
                if (!archive.Lock(tmpFile2, modeWriteCreate, LOCK_NOWAIT)) {
                    return false;
                }

                archive.Seek(0, SEEK_SET);
                archive.Write(MAGIC_NUMBER);
                archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
                archive.Write(nAddrs);
                archive.Write((uint32_t)appTable.size());  // not accurate yet

                vector<bloom_t> ar;
                for (size_t l = 0; l < consolidatedLines.size(); l++) {
                    string_q line = consolidatedLines[l];
                    ASSERT(countOf(line, '\t') == 2);
                    CStringArray parts;
                    explode(parts, line, '\t');
                    CIndexedAppearance rec(parts[1], parts[2]);
                    appTable.push_back(rec);
                    if (!prev.empty() && parts[0] != prev) {
                        addToSet(ar, prev);
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
                addToSet(ar, prev);
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

                CArchive output(WRITING_ARCHIVE);
                string_q bloomFile = substitute(substitute(chunkPath, "/finalized/", "/blooms/"), ".bin", ".bloom");
                if (output.Lock(bloomFile, modeWriteCreate, LOCK_NOWAIT)) {
                    output.Write((uint32_t)ar.size());
                    for (auto bloom : ar) {
                        output.Write(bloom.nInserted);
                        output.Write(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
                    }
                    output.Release();
                    copyFile(tmpFile2, chunkPath);
                    ::remove(tmpFile2.c_str());
                    string_q range =
                        substitute(substitute(chunkPath, indexFolder_finalized, ""), indexFolder_blooms, "");
                    range = substitute(substitute(range, ".bin", ""), ".bloom", "");
                    appendToAsciiFile(cacheFolder_tmp + "chunks_created.txt", range + "\n");
                }

                ostringstream os;
                os << "Wrote " << consolidatedLines.size() << " records to " << cTeal << relativize(chunkPath);
                if (isSnapToGrid && (lines.size() - 1 == loc)) {
                    os << cYellow << " (isSnapToGrid to modulo " << opts->snap_to_grid << " blocks)";
                }
                os << cOff;
                LOG_INFO(os.str());

                loc++;

                for (uint64_t record = loc; record < lines.size(); record++) {
                    remainingLines.push_back(lines[record]);
                }
                ::remove(opts->newStage.c_str());
            }
        }

        if (remainingLines.size()) {
            linesToAsciiFile(opts->newStage, remainingLines);
        }

        nRecords = fileSize(opts->newStage) / asciiAppearanceSize;
        if (isSnapToGrid)
            isSnapToGrid = nRecords > 0;
        chunkSize = min(opts->apps_per_chunk, nRecords);
    }

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
bool stage_chunks(COptions* opts, bool snappy) {
    LOG_IN('-', "stage_chunks " + bool_2_Str(snappy));

    string_q lastFileInStage = getLastFileInFolder(indexFolder_staging, false);
    opts->newStage = indexFolder_staging + padNum9(opts->prev_block) + ".txt";
    if (DebuggingOn) {
        LOG_FILE("\tlastFileInStage:    ", lastFileInStage);
        LOG_FILE("\topts->newStage:     ", opts->newStage);
    }

    if (lastFileInStage == opts->newStage) {
        LOG_OUT('-', "prevStage == opts->newStage - No Cleanup")
        return true;
    }

    string_q tmpFile = indexFolder + "temp.txt";
    if (DebuggingOn) {
        LOG_FILE("\ttmpFile:            ", tmpFile);
        LOG_FILE("\topts->stageStreamFn:", opts->stageStreamFn);
    }
    if (lastFileInStage != opts->stageStreamFn) {
        if (DebuggingOn) {
            LOG_INFO("prevStage != opts->stageStreamFn");
        }
        if (!appendFile(tmpFile /* to */, lastFileInStage /* from */)) {
            ::remove(tmpFile.c_str());
            LOG_OUT('-', "!appendFile1 -- remove(tmpFile)")
            return false;
        }
    }

    if (!appendFile(tmpFile /* to */, opts->stageStreamFn /* from */)) {
        ::remove(tmpFile.c_str());
        LOG_OUT('-', "!appendFile2 -- remove(tmpFile)")
        return false;
    }

    if (DebuggingOn) {
        LOG_FILE("\trename              ", tmpFile);
        LOG_FILE("\t  to                ", opts->newStage);
        LOG_FILE("\tremove stageStreamFn", opts->stageStreamFn);
    }

    ::rename(tmpFile.c_str(), opts->newStage.c_str());
    ::remove(opts->stageStreamFn.c_str());
    if (fileExists(lastFileInStage) && lastFileInStage != opts->newStage) {
        if (DebuggingOn) {
            LOG_INFO("!appendFile1 -- Cleanup -- remove(tmpFile)");
        }
        ::remove(lastFileInStage.c_str());
    }

    LOG_OUT('-', "")
    return true;
}

//----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    if (DebuggingOn) {
        colorsOff();
    }
    LOG_IN('=', "main")

    COptions options;

    CStringArray files;
    if (!listFilesInFolder(files, indexFolder_ripe, false)) {
        // There is nothing to process, not an error - we're at the front of the chain
        options.Cleanup();
        LOG_OUT('=', "")
        return EXIT_FAILURE;
    }

    options.stageStream.open(options.stageStreamFn, ios::out | ios::trunc);
    if (!options.stageStream.is_open()) {
        LOG_OUT('=', "Could not open temporary staging file.")
        return EXIT_FAILURE;
    }

    for (auto file : files) {
        ifstream inputStream(file, ios::in);
        if (inputStream.is_open()) {
            lockSection();

            options.stageStream << inputStream.rdbuf();
            options.stageStream.flush();
            inputStream.close();
            ::remove(file.c_str());

            options.prev_block = path_2_Bn(file);
            uint64_t nRecsInStream = fileSize(options.stageStreamFn) / asciiAppearanceSize;

            bool isSnapToGrid =
                (options.prev_block > options.first_snap && !(options.prev_block % options.snap_to_grid));
            bool overtops = (nRecsInStream > options.apps_per_chunk);

            if (isSnapToGrid || overtops) {
                options.stageStream.close();
                if (stage_chunks(&options, isSnapToGrid)) {
                    uint64_t nRecsNow = fileSize(options.newStage) / asciiAppearanceSize;
                    blknum_t chunkSize = min(nRecsNow, options.apps_per_chunk);
                    write_chunks(&options, chunkSize, isSnapToGrid);
                    if (DebuggingOn) {
                        LOG_FILE("newStage:             ", options.newStage);
                        LOG_INFO("snapPoint:            ",
                                 uint64_t((nRecsInStream + options.prev_block - 1) / options.snap_to_grid) *
                                     options.snap_to_grid);
                        LOG_INFO("chunkSize:            ", min(nRecsNow, options.apps_per_chunk));
                        LOG_INFO("nRecsInStreamPre:     ", nRecsInStream);
                        LOG_INFO("nRecsInStreamPost:    ", fileSize(options.stageStreamFn) / asciiAppearanceSize);
                        LOG_INFO("nRecsNow:             ", nRecsNow);
                    }

                    options.stageStream.open(options.stageStreamFn, ios::out | ios::trunc);
                    if (!options.stageStream.is_open()) {
                        unlockSection();
                        LOG_OUT('=', "Could not open temporary staging file.")
                        return EXIT_FAILURE;
                    }

                } else {
                    unlockSection();
                    options.stageStream.close();
                    options.Cleanup();
                    LOG_OUT('=', "Stage_chunks returned false")
                    return EXIT_FAILURE;
                }
            }
            unlockSection();

        } else {
            options.stageStream.close();
            options.Cleanup();
            LOG_OUT('=', "Could not open input stream " + file)
            return EXIT_FAILURE;
        }

        if (shouldQuit()) {
            options.stageStream.close();
            options.Cleanup();
            LOG_OUT('=', "Control+C hit")
            return EXIT_FAILURE;
        }
    }

    options.stageStream.close();
    LOG_OUT('=', "");
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
bloom_t::bloom_t(void) {
    nInserted = 0;
    bits = new uint8_t[BLOOM_WIDTH_IN_BYTES];
    bzero(bits, BLOOM_WIDTH_IN_BYTES * sizeof(uint8_t));
}

//---------------------------------------------------------------------------
bloom_t::~bloom_t(void) {
    if (bits)
        delete[] bits;
}

//---------------------------------------------------------------------------
bloom_t::bloom_t(const bloom_t& b) {
    nInserted = 0;
    bits = new uint8_t[BLOOM_WIDTH_IN_BYTES];
    bzero(bits, BLOOM_WIDTH_IN_BYTES * sizeof(uint8_t));
    nInserted = b.nInserted;
    memcpy(bits, b.bits, BLOOM_WIDTH_IN_BYTES);
}

//---------------------------------------------------------------------------
void bloom_t::lightBit(size_t bit) {
    size_t which = (bit / 8);
    size_t whence = (bit % 8);
    size_t index = BLOOM_WIDTH_IN_BYTES - which - 1;
    uint8_t mask = uint8_t(1 << whence);
    bits[index] |= mask;
}
