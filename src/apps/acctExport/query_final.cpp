/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------
bool visitFinalIndexFiles(const string_q& path, void* data) {
    ENTER("visitFinalIndexFiles");

    if (endsWith(path, "/")) {
        EXIT_NOMSG(forEveryFileInFolder(path + "*", visitFinalIndexFiles, data));

    } else {
        // Pick up some useful data for either method...
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. This silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || !endsWith(path, ".bloom")) {
            options->stats.nSkipped++;
            EXIT_NOMSG(!shouldQuit());
        }

        timestamp_t unused;
        options->fileRange.first = bnFromPath(path, options->fileRange.second, unused);
        ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that `start` and `end` options are ignored when scanning
        if (!rangesIntersect(options->listRange, options->fileRange)) {
            options->stats.nSkipped++;
            EXIT_NOMSG(!shouldQuit());
        }

        options->possibles.clear();
        for (auto m : options->allMonitors) {
            if (m.getLastBlockInMonitor() == 0 || m.getLastBlockInMonitor() < options->fileRange.first)
                options->possibles.push_back(m);
        }

        if (options->possibles.size() == 0) {
            options->stats.nSkipped++;
            EXIT_NOMSG(!shouldQuit());
        }

        if (isTestMode() && options->fileRange.second > 5000000) {
            options->stats.nSkipped++;
            EXIT_NOMSG(false);
        }

        // LOG4("Scanning ", path);
        bool ret = options->visitBinaryFile(path, data) && !shouldQuit();
        EXIT_NOMSG(ret);
    }

    ASSERT(0);  // should not happen
    EXIT_NOMSG(!shouldQuit());
}

//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void* data) {
    ENTER("visitBinaryFile");

    COptions* options = reinterpret_cast<COptions*>(data);
    options->stats.nChecked++;

    CAddressBoolMap hitMap;

    string_q bloomPath = path;
    string_q indexPath = substitute(substitute(path, indexFolder_blooms, indexFolder_finalized), ".bloom", ".bin");
    if (fileExists(bloomPath)) {
        CBloomArray bloomArray;
        readBloomFromBinary(bloomPath, bloomArray);
        bool hit = false;
        // Note: we used to stop searching on the first hit, and then scan the larger data files for all monitors in
        // this run, but now we keep a map of addresses that were bloom hits and only scan the ones that match.
        for (size_t a = 0; a < possibles.size(); a++) {  // && !hit; a++) { (remove after groking above comment)
            if (isMember(bloomArray, possibles[a].getBloom())) {
                hit = true;
                hitMap[possibles[a].address] = true;
            }
        }

        if (!hit) {
            // none of them hit, so write last block for each of them
            for (auto monitor : possibles) {
                monitor.writeLastBlockInMonitor(fileRange.second + 1);
            }
            options->stats.nBloomMisses++;
            LOG_PROGRESS("Skipping", options->fileRange.first, options->listRange.second, " bloom miss\r");
            EXIT_NOMSG(true);
        }
    }

    if (!establishIndexChunk(indexPath))
        EXIT_FAIL("Could not download index chunk " + indexPath + ".");

    options->stats.nBloomHits++;
    LOG_PROGRESS("Scanning", options->fileRange.first, options->listRange.second, " bloom hit \r");

    CArchive* chunk = NULL;
    char* rawData = NULL;
    uint32_t nAddrs = 0;

    bool indexHit = false;
    string_q hits;
    for (size_t mo = 0; mo < possibles.size() && !shouldQuit(); mo++) {
        CMonitor* monitor = &possibles[mo];
        if (hitMap[monitor->address]) {
            if (!monitor->openForWriting())
                EXIT_FAIL("Could not open cache file " + monitor->getMonitorPath(monitor->address, monitor->fm_mode) +
                          ".");

            CAppearanceArray_base2 items;
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

                CIndexHeader* h = reinterpret_cast<CIndexHeader*>(rawData);
                ASSERT(h->magic == MAGIC_NUMBER);
                ASSERT(bytes_2_Hash(h->hash) == versionHash);
                nAddrs = h->nAddrs;
                // uint32_t nRows = h->nRows; not used
            }

            CIndexedAddress search;
            for (size_t i = 0; i < 20; i++)
                search.bytes[i] = array[i];
            CIndexedAddress* found = (CIndexedAddress*)bsearch(&search, (rawData + sizeof(CIndexHeader)),  // NOLINT
                                                               nAddrs, sizeof(CIndexedAddress), findAddresses);

            if (found) {
                indexHit = true;
                options->stats.nPositive++;
                hits += (monitor->address.substr(0, 6) + "...");
                CIndexedAddress* addrsOnFile = reinterpret_cast<CIndexedAddress*>(rawData + sizeof(CIndexHeader));
                CIndexedAppearance* blocksOnFile = reinterpret_cast<CIndexedAppearance*>(&addrsOnFile[nAddrs]);
                options->stats.nTotalHits += found->cnt;
                for (size_t i = found->offset; i < found->offset + found->cnt; i++) {
                    CAppearance_base2 item(blocksOnFile[i].blk, blocksOnFile[i].txid);
                    items.push_back(item);
                }

                if (items.size()) {
                    lockSection();
                    monitor->writeMonitorArray(items);
                    unlockSection();
                }
            } else {
                options->stats.nFalsePositive++;
            }
        }
    }

    for (auto monitor : possibles)
        monitor.writeLastBlockInMonitor(fileRange.second + 1);

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
    LOG_PROGRESS("Scanning", options->fileRange.first, options->listRange.second, " bloom hit" + result);

    EXIT_NOMSG(!shouldQuit());
}

//---------------------------------------------------------------
bool getIndexChunkFromIPFS(const string_q& chunk) {
    return true;
}

//---------------------------------------------------------------
bool COptions::establishIndexChunk(const string_q& fullPathToChunk) {
    if (fileExists(fullPathToChunk))
        return true;

    string_q fileName = substitute(substitute(fullPathToChunk, indexFolder_finalized, ""), ".bin", "");
    static CPinnedChunkArray pins;
    if (pins.size() == 0) {
        if (!pinlib_readManifest(pins)) {
            LOG_ERR("Could not read the manifest.");
            return false;
        }
    }
    CPinnedChunk pin;
    if (pinlib_findChunk(pins, fileName, pin)) {
        LOG_PROGRESS(cGreen + "Unchaining-index", fileRange.first, listRange.second, " from IPFS" + cOff);
        if (!pinlib_getChunkFromRemote(pin, CHUNK_TYPE, .25))
            LOG_ERR("Could not retrieve file from IPFS: ", fullPathToChunk);
    } else {
        LOG_ERR("Could not find file in manifest: ", fullPathToChunk);
    }

    return fileExists(fullPathToChunk);
}
