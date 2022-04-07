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

//----------------------------------------------------------------
bool showApp(const CAppearance& item, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << item;
    opt->firstOut = false;
    return !shouldQuit();
}

extern bool getChunkRanges(CBlockRangeArray& ranges);

//-----------------------------------------------------------------------
int compareEntry(const CReverseAppMapEntry* a, const CReverseAppMapEntry* b) {
    int ret = int(a->blk) - int(b->blk);
    if (ret)
        return ret;
    ret = int(a->tx) - int(b->tx);
    if (ret)
        return ret;
    return 0;
}

//----------------------------------------------------------------
int findRevMapEntry(const void* v1, const void* v2) {
    const CReverseAppMapEntry* at1 = (CReverseAppMapEntry*)v1;  // NOLINT
    const CReverseAppMapEntry* at2 = (CReverseAppMapEntry*)v2;  // NOLINT
    return compareEntry(at1, at2);
}

//----------------------------------------------------------------
bool isSame(const CReverseAppMapEntry* a, const CReverseAppMapEntry* b) {
    return !compareEntry(a, b);
}

//-----------------------------------------------------------------------
bool assignReason(const CAccountName& accountedFor, CAppearance& app, const CTransaction& trans) {
    if (app.tx > 99996) {  // leave this here for searching: 99999
        app.reason = "miner";
        return true;
    }
    if (app.addr == trans.from) {
        app.reason = "from";
        return true;
    }
    if (app.addr == trans.to) {
        app.reason = "to";
        return true;
    }
    if (app.addr == trans.receipt.contractAddress) {
        app.reason = "creation";
        return true;
    }
    bool junk = false;
    string_q ss = substitute(app.addr, "0x", "");
    if (contains(trans.input, ss)) {
        app.reason = "input";
        if (accountedFor.address == app.addr) {
            return true;
        }
        junk = true;
    }

    for (size_t i = 0; i < trans.receipt.logs.size(); i++) {
        const CLogEntry* l = &trans.receipt.logs[i];
        if (l->address == app.addr) {
            app.reason = "log_" + uint_2_Str(i) + "_generator";
            return true;
        }
        if (contains(l->topics[0], ss)) {
            app.reason = "log_" + uint_2_Str(i) + "_topic_" + uint_2_Str(0);
            return true;
        }
        for (size_t j = 1; j < l->topics.size(); j++) {
            if (contains(l->topics[j], ss)) {
                app.reason = "log_" + uint_2_Str(i) + "_topic_" + uint_2_Str(j);
                if (accountedFor.address == app.addr) {
                    return true;
                }
                junk = true;
            }
        }
        if (contains(l->data, ss)) {
            app.reason = "log_" + uint_2_Str(i) + "_data";
            if (accountedFor.address == app.addr) {
                return true;
            }
            junk = true;
        }
    }

    if (!junk) {
        app.reason = "trace";
    }
    return true;  // !junk;
}

//-----------------------------------------------------------------------
CIndexArchiveWithNeighborMaps::~CIndexArchiveWithNeighborMaps() {
    clean();
}

//-----------------------------------------------------------------------
int sortRecords(const void* i1, const void* i2) {
    int32_t* p1 = (int32_t*)i1;
    int32_t* p2 = (int32_t*)i2;
    if (p1[1] == p2[1]) {
        if (p1[2] == p2[2]) {
            return (p1[0] - p2[0]);
        }
        return p1[2] - p2[2];
    }
    return p1[1] - p2[1];
}

//-----------------------------------------------------------------------
bool CIndexArchiveWithNeighborMaps::LoadReverseMaps(const blkrange_t& range) {
    if (reverseAppMap) {
        delete[] reverseAppMap;
        reverseAddrRanges.clear();
        reverseAppMap = nullptr;
    }

    uint32_t nAppsHere = header.nApps;

    string_q mapFile = substitute(getFilename(), indexFolder_finalized, indexFolder_map);
    if (fileExists(mapFile)) {
        CArchive archive(READING_ARCHIVE);
        if (!archive.Lock(mapFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open file ", mapFile);
            return false;
        }
        size_t nRecords = fileSize(mapFile) / sizeof(CReverseAppMapEntry);
        ASSERT(nRecords == nAppsHere);
        // Cleaned up on destruction of the chunk
        reverseAppMap = new CReverseAppMapEntry[nRecords];
        if (!reverseAppMap) {
            LOG_ERR("Could not allocate memory for CReverseAppMapEntry");
            return false;
        }
        archive.Read((char*)reverseAppMap, sizeof(char), nRecords * sizeof(CReverseAppMapEntry));
        archive.Release();
        for (uint32_t i = 0; i < header.nAddrs; i++) {
            reverseAddrRanges.push_back(getAppRangeForAddrAt(i));
        }
        return true;
    }

    // Cleaned up on destruction of the chunk
    reverseAppMap = new CReverseAppMapEntry[nAppsHere];
    if (!reverseAppMap) {
        LOG_ERR("Could not allocate memory for CReverseAppMapEntry");
        return false;
    }
    for (uint32_t i = 0; i < nAppsHere; i++) {
        reverseAppMap[i].n = i;
        CIndexedAppearance* app = getAppearanceAt(i);
        reverseAppMap[i].blk = app->blk;
        reverseAppMap[i].tx = app->txid;
    }

    for (uint32_t i = 0; i < header.nAddrs; i++) {
        reverseAddrRanges.push_back(getAppRangeForAddrAt(i));
    }

    qsort(reverseAppMap, nAppsHere, sizeof(CReverseAppMapEntry), sortRecords);

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(mapFile, modeWriteCreate, LOCK_WAIT)) {
        LOG_ERR("Could not open file ", mapFile);
        return false;
    }
    archive.Write(reverseAppMap, sizeof(char), nAppsHere * sizeof(CReverseAppMapEntry));
    archive.Release();

    LOG_PROG("Processed: " + getFilename());
    return true;
}

//-----------------------------------------------------------------------
bool COptions::showAddrsInTx(CTraverser* trav, const blkrange_t& range, const CAppearance_mon& app) {
    string_q fn = range_2_Str(range);
    string_q chunkPath = indexFolder_finalized + fn + ".bin";

    if (!theIndex || theIndex->getFilename() != chunkPath) {
        if (theIndex) {
            delete theIndex;
            theIndex = nullptr;
        }
        theIndex = new CIndexArchiveWithNeighborMaps(READING_ARCHIVE);
        if (theIndex->ReadIndexFromBinary(chunkPath, IP_ALL)) {
            theIndex->LoadReverseMaps(range);
            if (!theIndex->reverseAppMap) {
                LOG_ERR("Could not allocate reverseAppMap");
                return false;
            }
        } else {
            LOG_WARN("Cannot open index file ", chunkPath);
        }
    }

    CReverseAppMapEntry search;
    search.blk = app.blk;
    search.tx = app.txid;
    CReverseAppMapEntry* found = (CReverseAppMapEntry*)bsearch(&search, theIndex->reverseAppMap, theIndex->header.nApps,
                                                               sizeof(CReverseAppMapEntry), findRevMapEntry);

    if (found) {
        trav->app = &app;
        loadTx_Func(trav, this);

        while (found > theIndex->reverseAppMap) {
            found--;
            if (!isSame(found, &search)) {
                found++;
                break;
            }
        }

        size_t start = 0;
        CReverseAppMapEntry* endOfRevMap =
            theIndex->reverseAppMap + (theIndex->header.nApps * sizeof(CReverseAppMapEntry));
        while (found < endOfRevMap && isSame(found, &search)) {
            for (size_t i = start; i < theIndex->reverseAddrRanges.size(); i++) {
                blkrange_t* r = &theIndex->reverseAddrRanges[i];
                if (inRange(found->n, *r)) {
                    CAppearance appHere;
                    appHere.bn = found->blk;
                    appHere.tx = found->tx;
                    appHere.addr = bytes_2_Addr(theIndex->getAddressAt(i)->bytes);
                    if (assignReason(accountedFor, appHere, trav->trans)) {
                        trav->nProcessed++;
                        if (!prog_Log(trav, this))
                            return false;
                        showApp(appHere, this);
                    }
                    start = i;
                    break;
                }
            }
            found++;
        }
    } else {
        LOG_ERR("Appearance (", app.blk, ".", app.txid, ") for address \"", accountedFor.address, "\" not found in ",
                chunkPath);
    }

    return prog_Log(trav, this);
}

//-----------------------------------------------------------------------
// For neighbors we handle the entire production of data in the _Pre
// because what we actually want to do is scan across the index chunks
bool neighbors_Pre(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    opt->reportDef = 43;  // random prime

    // LOG_INFO("Processing address ", opt->accountedFor.address);

    CBlockRangeArray ranges;
    getChunkRanges(ranges);

    uint64_t curRange = 0;
    for (trav->index = 0; trav->index < opt->monApps.size() && !shouldQuit(); trav->index++) {
        CAppearance_mon app = opt->monApps[trav->index];
        //     opt->neighborCount = 0;
        while (curRange < ranges.size() && !inRange(blknum_t(app.blk), ranges[curRange])) {
            curRange++;
        }
        if (curRange < ranges.size()) {
            if (!opt->showAddrsInTx(trav, ranges[curRange], app))
                return false;
            curRange--;  // back up one in case the next appearances is in the same range
        }
    }

    if (opt->theIndex) {
        // LOG_INFO(bYellow, "Clearing the reverse map for ", opt->theIndex->getFilename(), cOff);
        delete opt->theIndex;
        opt->theIndex = nullptr;
    } else {
        LOG_INFO(bYellow, "Clearing the reverse map", cOff);
    }

    post_Func(trav, data);
    LOG_INFO("   Done...");
    return false;  // !shouldQuit();
}

// //-----------------------------------------------------------------------
// bool neighbors_Display(CTraverser* trav, void* data) {
//     Note that we do all the processing in the 'neighbors_Pre' function
// }

//-----------------------------------------------------------------------
size_t neighbors_Count(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    return opt->neighborCount;
}

extern bool visitBloom(const string_q& path, void* data);
//-----------------------------------------------------------------------
bool getChunkRanges(CBlockRangeArray& ranges) {
    forEveryFileInFolder(indexFolder_blooms + "*", visitBloom, &ranges);
    // LOG_INFO("Found ", ranges.size(), " chunks");
    return true;
}

//-----------------------------------------------------------------------
bool visitBloom(const string_q& path, void* data) {
    if (endsWith(path, ".bloom")) {
        CBlockRangeArray* ranges = (CBlockRangeArray*)data;
        blkrange_t range = str_2_Range(substitute(path, indexFolder_blooms, ""));
        ranges->push_back(range);
    }
    return true;
}
