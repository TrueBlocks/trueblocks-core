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

// //----------------------------------------------------------------
// bool showApp(const CAppearance& item, void* data) {
//     COptions* opt = reinterpret_cast<COptions*>(data);
//     opt->tTrav->nProcessed++;
//     cout << ((isJson() && !opt->firstOut) ? ", " : "");
//     cout << item;
//     opt->firstOut = false;
//     return !shouldQuit();
// }

#define indexFolder_map (getIndexPath("maps/"))

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
bool CIndexArchive::LoadReverseMap(void) {
    if (reverseMap) {
        delete[] reverseMap;
        reverseMap = nullptr;
    }

    uint32_t nApps = header1->nRows;

    string_q mapFile = substitute(getFilename(), indexFolder_finalized, indexFolder_map);
    if (fileExists(mapFile)) {
        CArchive archive(READING_ARCHIVE);
        if (!archive.Lock(mapFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open file ", mapFile);
            return false;
        }
        size_t nRecords = fileSize(mapFile) / sizeof(CReverseMapEntry);
        ASSERT(nRecords == nApps);
        // Cleaned up on destruction of the chunk
        reverseMap = new CReverseMapEntry[nRecords];
        if (!reverseMap) {
            LOG_ERR("Could not allocate memory for CReverseMapEntry");
            return false;
        }
        archive.Read((char*)reverseMap, sizeof(char), nRecords * sizeof(CReverseMapEntry));
        archive.Release();
        for (uint32_t i = 0; i < nRecords; i++) {
            if (!(i % 9765)) {
                LOG_INFO(padNum9(reverseMap[i].blk), ".", padNum5(reverseMap[i].tx), " ", padNum7(reverseMap[i].n));
            }
        }
        LOG_INFO("Finished reading ", mapFile, string_q(30, ' '));
        return true;
    }

    // Cleaned up on destruction of the chunk
    reverseMap = new CReverseMapEntry[nApps];
    if (!reverseMap) {
        LOG_ERR("Could not allocate memory for CReverseMapEntry");
        return false;
    }
    for (uint32_t i = 0; i < nApps; i++) {
        reverseMap[i].n = i;
        reverseMap[i].blk = appearances1[i].blk;
        reverseMap[i].tx = appearances1[i].txid;
    }

    qsort(reverseMap, nApps, sizeof(CReverseMapEntry), sortRecords);

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(mapFile, modeWriteCreate, LOCK_WAIT)) {
        LOG_ERR("Could not open file ", mapFile);
        return false;
    }
    archive.Write(reverseMap, sizeof(char), nApps * sizeof(CReverseMapEntry));
    archive.Release();
    for (uint32_t i = 0; i < nApps; i++) {
        if (!(i % 9765)) {
            LOG_INFO(reverseMap[i].blk, ".", reverseMap[i].tx, " ", reverseMap[i].n);
        }
    }
    LOG_PROG("Processed: " + getFilename());
    return true;
}

//-----------------------------------------------------------------------
int compareEntry(const CReverseMapEntry* a, const CReverseMapEntry* b) {
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
    const CReverseMapEntry* at1 = (CReverseMapEntry*)v1;  // NOLINT
    const CReverseMapEntry* at2 = (CReverseMapEntry*)v2;  // NOLINT
    return compareEntry(at1, at2);
}

//----------------------------------------------------------------
bool isSame(const CReverseMapEntry* a, const CReverseMapEntry* b) {
    return !compareEntry(a, b);
}

//-----------------------------------------------------------------------
bool COptions::showAddrsInTx(blkrange_t range, const CAppearance_mon& app) {
    string_q fn = range_2_Str(range);
    string_q chunkPath = indexFolder_finalized + fn + ".bin";

    if (!theIndex || theIndex->getFilename() != chunkPath) {
        if (theIndex) {
            delete theIndex;
            theIndex = nullptr;
        }
        theIndex = new CIndexArchive(READING_ARCHIVE);
        if (theIndex->ReadIndexFromBinary(chunkPath)) {
            theIndex->LoadReverseMap();
            if (!theIndex->reverseMap) {
                LOG_ERR("Could not allocate reverseMap");
                return false;
            }
        }
    }

    LOG_INFO("  Search: ", app.blk, ".", app.txid, string_q(100, ' '));
    if (!theIndex->reverseMap) {
        LOG_ERR("Could not allocate reverseMap");
        return false;
    }

    CReverseMapEntry search;
    search.blk = app.blk;
    search.tx = app.txid;
    CReverseMapEntry* found = (CReverseMapEntry*)bsearch(&search, theIndex->reverseMap, theIndex->nApps1,
                                                         sizeof(CReverseMapEntry), findRevMapEntry);
    if (found) {
        size_t cnt = 0;
        // back up in case we hit an entry past the first one
        LOG_INFO(bGreen, "  Found:    ", found->blk, ".", found->tx, " ", found->n, ": ", isSame(found, &search), cOff);
        while (found > theIndex->reverseMap) {
            found--;
            LOG_INFO(bGreen, "  Found-", cnt++, ":    ", found->blk, ".", found->tx, " ", found->n, ": ",
                     isSame(found, &search), cOff);
            if (!isSame(found, &search)) {
                found++;
                cnt--;
                break;
            }
        }
        LOG_INFO(bGreen, "  Start-", cnt, ":    ", found->blk, ".", found->tx, " ", found->n, ": ",
                 isSame(found, &search), cOff);
        CReverseMapEntry* endOfRevMap = theIndex->reverseMap + (theIndex->nApps1 * sizeof(CReverseMapEntry));
        while (found < endOfRevMap && isSame(found, &search)) {
            if (found->n > theIndex->nAddrs1) {
                LOG_ERR("Past end of array");
            } else {
                cout << found->n << " " << found->blk << "." << found->tx << " "
                     << bytes_2_Addr(theIndex->addresses1[found->n].bytes) << endl;
            }
            LOG_INFO("isSame: ", found->n, " ", isSame(found, &search));
            found++;
            LOG_INFO("isSame: ", found->n, " ", isSame(found, &search));
            LOG_INFO(found, " ", endOfRevMap, " (", size_t(endOfRevMap - found), "): ", (found < endOfRevMap));
            LOG_INFO("I am here");
        }
        LOG_ERR("Done with this appearance.");
    } else {
        LOG_ERR("Appearance not found.");
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
// For neighbors we handle the entire production of data in the _Pre
// because what we actually want to do is scan across the index chunks
extern bool getChunkRanges(CBlockRangeArray& ranges);
bool neighbors_Pre(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);

    establishFolder(indexFolder_map);

    CBlockRangeArray ranges;
    getChunkRanges(ranges);

    uint64_t curRange = 0;
    for (trav->index = 0; trav->index < opt->monApps.size() && !shouldQuit(); trav->index++) {
        CAppearance_mon app = opt->monApps[trav->index];
        while (curRange < ranges.size() && !inRange(blknum_t(app.blk), ranges[curRange])) {
            curRange++;
        }
        if (curRange < ranges.size()) {
            if (!opt->showAddrsInTx(ranges[curRange], app))
                return false;
            curRange--;  // back up one in case the next appearances is in the same range
        }
    }

    if (opt->theIndex) {
        LOG_INFO(bYellow, "Clearing the reverse map for ", opt->theIndex->getFilename(), cOff);
        delete opt->theIndex;
        opt->theIndex = nullptr;
    } else {
        LOG_INFO(bYellow, "Clearing the reverse map", cOff);
    }

    return !shouldQuit();
}

// //-----------------------------------------------------------------------
// bool neighbors_Display(CTraverser* trav, void* data) {
//     COptions* opt = reinterpret_cast<COptions*>(data);
//     opt->neighborCount = 0;
//     opt->tTrav = trav;
//     trav->trans.forEveryUniqueAppearanceInTxPerTx(visitApp, transFilter, data);
//     prog_Log(trav, data);
//     return !shouldQuit();
// }

//-----------------------------------------------------------------------
size_t neighbors_Count(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    return opt->neighborCount;
}

extern bool visitBloom(const string_q& path, void* data);
//-----------------------------------------------------------------------
bool getChunkRanges(CBlockRangeArray& ranges) {
    forEveryFileInFolder(getIndexPath("blooms/*"), visitBloom, &ranges);
    LOG_INFO("Found ", ranges.size(), " chunks");
    return true;
}

//-----------------------------------------------------------------------
bool visitBloom(const string_q& path, void* data) {
    if (endsWith(path, ".bloom")) {
        CBlockRangeArray* ranges = (CBlockRangeArray*)data;
        blkrange_t range = str_2_Range(substitute(path, getIndexPath("blooms/"), ""));
        ranges->push_back(range);
    }
    return true;
}
