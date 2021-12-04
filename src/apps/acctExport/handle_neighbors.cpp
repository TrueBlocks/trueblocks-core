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
        size_t size = fileSize(mapFile);
        reverseMap = new uint32_t[size];
        archive.Read(reverseMap, sizeof(char), size);
        archive.Release();
        for (uint32_t i = 0; i < nApps; i++) {
            uint32_t index = (i * 3);
            if (!(index % 1000)) {
                LOG_INFO(padNum9(reverseMap[index + 1]), ".", padNum5(reverseMap[index + 2]), " ",
                         padNum7(reverseMap[index]), " ", mapFile, "        \r");
            }
        }
        LOG_INFO("Finished reading ", mapFile, string_q(30, ' '));
        return true;
    }

    uint32_t recordSize = 3 * sizeof(uint32_t);
    reverseMap = new uint32_t[nApps * recordSize];

    for (uint32_t i = 0; i < nApps; i++) {
        reverseMap[(i * 3)] = i;
        reverseMap[(i * 3) + 1] = appearances1[i].blk;
        reverseMap[(i * 3) + 2] = appearances1[i].txid;
    }

    qsort(reverseMap, nApps, recordSize, sortRecords);

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(mapFile, modeWriteCreate, LOCK_WAIT)) {
        LOG_ERR("Could not open file ", mapFile);
        return false;
    }
    archive.Write(reverseMap, sizeof(char), recordSize * nApps);
    archive.Release();
    for (uint32_t i = 0; i < nApps; i++) {
        uint32_t index = (i * 3);
        if (!(index % 10013)) {
            LOG_INFO(reverseMap[index + 1], ".", reverseMap[index + 2], " ", reverseMap[index]);
        }
    }
    LOG_PROG("Processed: " + getFilename());
    return true;
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

    LOG_INFO("  Searching: ", app.blk, ".", app.txid, string_q(100, ' '));
    if (!theIndex->reverseMap) {
        LOG_ERR("Could not allocate reverseMap");
        return false;
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool neighbors_Pre(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);

    establishFolder(indexFolder_map);

    CStringArray lines;
    asciiFileToLines(getConfigPath("manifest/manifest.txt"), lines);
    sort(lines.begin(), lines.end());

    CBlockRangeArray ranges;
    for (auto line : lines) {
        if (line.length() < 10)
            continue;  // not a valid line
        blkrange_t range = str_2_Range(line);
        ranges.push_back(range);
    }
    LOG_INFO("Found ", ranges.size(), " chunks");

#define REMOVE_ME 1000000

    uint64_t curRange = 0;
    for (trav->index = 0; trav->index < opt->monApps.size() && !shouldQuit(); trav->index++) {
        if (trav->index > REMOVE_ME)
            break;
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
