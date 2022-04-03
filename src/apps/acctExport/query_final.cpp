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

//---------------------------------------------------------------
bool visitChunkToFreshenFinal(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitChunkToFreshenFinal, data);

    } else {
        // Pick up some useful data for either method...
        COptions* options = reinterpret_cast<COptions*>(data);
        options->stats.nFiles++;

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. This silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || !endsWith(path, ".bloom")) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        timestamp_t unused;
        options->fileRange.first = path_2_Bn(path, options->fileRange.second, unused);
        ASSERT(unused != NOPOS && options->fileRange.first != NOPOS && options->fileRange.second != NOPOS);

        // Note that `start` and `end` options are ignored when scanning
        if (!rangesIntersect(options->needRange, options->fileRange)) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        options->possibles.clear();
        for (auto m : options->allMonitors) {
            blknum_t nextVisit = m.getNextBlockToVisit(true /* ifExists */);
            if (nextVisit == 0 || nextVisit <= options->fileRange.first) {
                options->possibles.push_back(m);
            }
        }

        if (options->possibles.size() == 0) {
            options->stats.nSkipped++;
            return !shouldQuit();
        }

        if (isTestMode() && options->fileRange.second > 5000000) {
            options->stats.nSkipped++;
            return false;
        }

        bool ret = options->visitBinaryFile(path, data) && !shouldQuit();
        return ret;
    }

    ASSERT(0);  // should not happen
    return !shouldQuit();
}

//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void* data) {
    COptions* options = reinterpret_cast<COptions*>(data);
    options->stats.nChecked++;

    CAddressBoolMap hitMap;

    string_q bloomPath = path;
    string_q indexPath = substitute(substitute(path, indexFolder_blooms, indexFolder_finalized), ".bloom", ".bin");
    if (fileExists(bloomPath)) {
        CBloomFilter bloomFilter;
        bloomFilter.readBloomFilter(bloomPath, true /* readBits */);
        bool hit = false;
        // Note: we used to stop searching on the first hit, and then scan the larger data files for all monitors in
        // this run, but now we keep a map of addresses that were bloom hits and only scan the ones that match.
        for (size_t a = 0; a < possibles.size(); a++) {  // && !hit; a++) { (remove after groking above comment)
            if (bloomFilter.isMemberOf(possibles[a].address)) {
                hit = true;
                hitMap[possibles[a].address] = true;
            }
        }

        if (!hit) {
            // none of them hit, so write last block for each of them
            for (auto monitor : possibles)
                monitor.writeNextBlockToVisit(fileRange.second + 1, monitor.isStaging);
            options->stats.nBloomMisses++;
            LOG_PROGRESS(SKIPPING, options->fileRange.first, options->needRange.second, " bloom miss\r");
            return true;
        }
    }

    if (!establishIndexChunk(indexPath)) {
        LOG_WARN("Could not download index chunk " + indexPath + ".");
        return false;
    }

    options->stats.nBloomHits++;
    LOG_PROGRESS(SCANNING, options->fileRange.first, options->needRange.second, " bloom hit      \r");

    CArchive* chunk = NULL;
    char* rawData = NULL;
    uint32_t nAddrs = 0;

    bool indexHit = false;
    string_q hits;
    for (size_t mo = 0; mo < possibles.size() && !shouldQuit(); mo++) {
        CMonitor* monitor = &possibles[mo];
        if (hitMap[monitor->address]) {
            if (!monitor->openForWriting(monitor->isStaging)) {
                LOG_WARN("Could not open monitor file for " + monitor->address + ".");
                return false;
            }

            CAppearanceArray_mon items;
            items.reserve(300000);

            addrbytes_t array = addr_2_Bytes(monitor->address);

            if (!chunk) {
                chunk = new CArchive(READING_ARCHIVE);
                if (!chunk || !chunk->Lock(indexPath, modeReadOnly, LOCK_NOWAIT)) {
                    LOG_WARN("Could not open index file " + indexPath + ".");
                    return false;
                }

                size_t sz = fileSize(indexPath);
                rawData = reinterpret_cast<char*>(malloc(sz + (2 * asciiAppearanceSize)));
                if (!rawData) {
                    chunk->Release();
                    delete chunk;
                    chunk = NULL;
                    LOG_WARN("Could not allocate memory for data.");
                    return false;
                }

                bzero(rawData, sz + (2 * asciiAppearanceSize));
                size_t nRead = chunk->Read(rawData, sz, sizeof(char));
                if (nRead != sz) {
                    LOG_WARN("Could not read entire file.");
                    return false;
                }

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
                    CAppearance_mon item(blocksOnFile[i].blk, blocksOnFile[i].txid);
                    items.push_back(item);
                }

                if (items.size()) {
                    lockSection();
                    monitor->writeAppendNewApps(items);
                    unlockSection();
                }
            } else {
                options->stats.nFalsePositive++;
            }
        }
    }

    for (auto monitor : possibles)
        monitor.writeNextBlockToVisit(fileRange.second + 1, monitor.isStaging);

    if (chunk) {
        chunk->Release();
        delete chunk;
        chunk = NULL;
    }

    if (rawData) {
        delete rawData;
        rawData = NULL;
    }

    {  // do not remove the frame
        static int cnt = 0;
        if (!(++cnt % 23)) {
            if (indexHit) {
                LOG_PROGRESS(SCANNING, options->fileRange.first, options->needRange.second,
                             " bloom hit index hit      \r");
            } else {
                LOG_PROGRESS(SCANNING, options->fileRange.first, options->needRange.second,
                             " bloom hit false positive \r");
            }
        }
    }

    return !shouldQuit();
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
        LOG_PROGRESS(EXTRACT, fileRange.first, needRange.second, " from IPFS" + cOff);
        if (!pinlib_getChunkFromRemote(pin, .25))
            LOG_ERR("Could not retrieve file from IPFS: ", fullPathToChunk);
    } else {
        LOG_ERR("Could not find file in manifest: ", fullPathToChunk);
    }

    return fileExists(fullPathToChunk);
}
