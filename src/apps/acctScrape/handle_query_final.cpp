/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

extern bool establishIndexChunk(const string_q& chunk);
//---------------------------------------------------------------
bool visitFinalIndexFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitFinalIndexFiles, data);

    } else {
        // Pick up some useful data for either method...
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. Silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || !endsWith(path, ".bloom")) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        timestamp_t unused;
        options->fileRange.first = bnFromPath(path, options->fileRange.second, unused);
        ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that the --start and --end options are ignored.
        if (!rangesIntersect(options->scanRange, options->fileRange)) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        if (isTestMode() && options->fileRange.second > 5000000) {
            options->stats.nSkipped++;
            return false;
        }

        // LOG4("Scanning ", path);
        return options->visitBinaryFile(path, data) && !shouldQuit();
    }

    ASSERT(0);  // should not happen
    return !shouldQuit();
}

//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void* data) {
    string_q l_funcName = "visitBinaryFile";

    COptions* options = reinterpret_cast<COptions*>(data);
    options->stats.nChecked++;

    typedef map<address_t, bool> CHitMap;
    CHitMap hitMap;

    string_q bloomPath = path;
    string_q indexPath = substitute(substitute(path, indexFolder_blooms, indexFolder_finalized), ".bloom", ".bin");
    if (fileExists(bloomPath)) {
        CBloomArray bloomArray;
        readBloomFromBinary(bloomPath, bloomArray);
        bool hit = false;
        // Note: we used to stop searching on the first hit, and then scan the larger data files for all monitors in
        // this run, but now we keep a map of addresses that were bloom hits and only scan the ones that match.
        for (size_t a = 0; a < monitors.size(); a++) {  // && !hit; a++) { (remove after groking above comment)
            if (isMember(bloomArray, monitors[a].getBloom())) {
                hit = true;
                hitMap[monitors[a].address] = true;
            }
        }

        if (!hit) {
            // none of them hit, so write last block for each of them
            for (auto monitor : monitors) {
                string_q filename = getMonitorPath(monitor.address);
                monitor.fm_mode = (fileExists(filename) ? FM_PRODUCTION : FM_STAGING);
                monitor.writeLastBlock(fileRange.second + 1);
            }
            options->stats.nBloomMisses++;
            LOG_PROGRESS1("Skipping", options->fileRange.first, options->scanRange.second, " bloom miss\r");
            return true;
        }
    }

    if (!establishIndexChunk(indexPath))
        EXIT_FAIL("Could not download index chunk " + indexPath + ". Quitting...");

    options->stats.nBloomHits++;
    LOG_PROGRESS1("Scanning", options->fileRange.first, options->scanRange.second, " bloom hit\r");

    CArchive* chunk = NULL;
    char* rawData = NULL;
    uint32_t nAddrs = 0;

    bool indexHit = false;
    string_q hits;
    for (size_t mo = 0; mo < monitors.size() && !shouldQuit(); mo++) {
        CMonitor* monitor = &monitors[mo];
        if (hitMap[monitor->address]) {
            string_q filename = getMonitorPath(monitor->address);
            bool exists = fileExists(filename);
            monitor->fm_mode = (exists ? FM_PRODUCTION : FM_STAGING);

            if (!monitor->openCacheFile1())
                EXIT_FAIL("Could not open cache file " + getMonitorPath(monitor->address, monitor->fm_mode) + ".");

            CAppearanceArray_base items;
            items.reserve(300000);

            addrbytes_t array = addr_2_Bytes(monitor->address);

            if (!chunk) {
                chunk = new CArchive(READING_ARCHIVE);
                if (!chunk || !chunk->Lock(indexPath, modeReadOnly, LOCK_NOWAIT))
                    EXIT_FAIL("Could not open index file " + indexPath + ".");

                size_t sz = fileSize(indexPath);
                rawData = reinterpret_cast<char*>(malloc(sz + (2 * 59)));
                if (!rawData) {
                    chunk->Release();
                    delete chunk;
                    chunk = NULL;
                    EXIT_FAIL("Could not allocate memory for data.");
                }

                bzero(rawData, sz + (2 * 59));
                size_t nRead = chunk->Read(rawData, sz, sizeof(char));
                if (nRead != sz)
                    EXIT_FAIL("Could not read entire file.");

                CHeaderRecord_base* h = reinterpret_cast<CHeaderRecord_base*>(rawData);
                ASSERT(h->magic == MAGIC_NUMBER);
                ASSERT(bytes_2_Hash(h->hash) == versionHash);
                nAddrs = h->nAddrs;
                // uint32_t nRows = h->nRows; not used
            }

            CAddressRecord_base search;
            for (size_t i = 0; i < 20; i++)
                search.bytes[i] = array[i];
            CAddressRecord_base* found =
                (CAddressRecord_base*)bsearch(&search, (rawData + sizeof(CHeaderRecord_base)),  // NOLINT
                                              nAddrs, sizeof(CAddressRecord_base), findAppearance);

            if (found) {
                indexHit = true;
                options->stats.nPositive++;
                hits += (monitor->address.substr(0, 6) + "..");
                CAddressRecord_base* addrsOnFile =
                    reinterpret_cast<CAddressRecord_base*>(rawData + sizeof(CHeaderRecord_base));
                CAppearance_base* blocksOnFile = reinterpret_cast<CAppearance_base*>(&addrsOnFile[nAddrs]);
                options->stats.nRecords += found->cnt;
                for (size_t i = found->offset; i < found->offset + found->cnt; i++) {
                    CAppearance_base item(blocksOnFile[i].blk, blocksOnFile[i].txid);
                    items.push_back(item);
                }

                if (items.size()) {
                    lockSection(true);
                    monitor->writeAnArray(items);
                    monitor->writeLastBlock(fileRange.second + 1);
                    lockSection(false);
                }
            } else {
                options->stats.nFalsePositive++;
                monitor->writeLastBlock(fileRange.second + 1);
            }
        }
    }

    if (chunk) {
        chunk->Release();
        delete chunk;
        chunk = NULL;
    }

    if (rawData) {
        delete rawData;
        rawData = NULL;
    }

    string_q result = indexHit ? " index hit " + hits : " false positive";
    LOG_PROGRESS1("Scanning", options->fileRange.first, options->scanRange.second, " bloom hit" + result);

    return !shouldQuit();
}

//---------------------------------------------------------------
bool getIndexChunkFromIPFS(const string_q& chunk) {
    return true;
}

//---------------------------------------------------------------
bool establishIndexChunk(const string_q& fileName) {
    if (!fileExists(fileName)) {
        cerr << fileName << " not present. Retreiving from IPFS" << endl;
        CPinnedItem pin;
        if (!getChunkByHash(substitute(substitute(fileName, indexFolder_finalized, ""), ".bin", ""), pin)) {
            cerr << "Could not retrieve file from IPFS: " << fileName << endl;
        }
    }
    return fileExists(fileName);
}
