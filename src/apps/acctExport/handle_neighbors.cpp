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
    // opt->tTrav->nProcessed++;
    COptions* opt = reinterpret_cast<COptions*>(data);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    if (opt->neighborSelfies) {
        CAppearance a = item;
        a.addr = substitute(item.addr, opt->accountedFor.address, "-----------------self---------------------");
        cout << a;
    } else {
        cout << item;
    }
    opt->firstOut = false;
    return !shouldQuit();
}

#define indexFolder_map (getIndexPath("maps/"))
extern bool getChunkRanges(CBlockRangeArray& ranges);

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
bool CIndexArchive::LoadReverseMaps(const blkrange_t& range) {
    if (reverseAppMap) {
        delete[] reverseAppMap;
        reverseAddrRanges.clear();
        reverseAppMap = nullptr;
    }

    uint32_t nApps = header1->nRows;

    string_q mapFile = substitute(getFilename(), indexFolder_finalized, indexFolder_map);
    if (fileExists(mapFile)) {
        CArchive archive(READING_ARCHIVE);
        if (!archive.Lock(mapFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open file ", mapFile);
            return false;
        }
        size_t nRecords = fileSize(mapFile) / sizeof(CReverseAppMapEntry);
        ASSERT(nRecords == nApps);
        // Cleaned up on destruction of the chunk
        reverseAppMap = new CReverseAppMapEntry[nRecords];
        if (!reverseAppMap) {
            LOG_ERR("Could not allocate memory for CReverseAppMapEntry");
            return false;
        }
        archive.Read((char*)reverseAppMap, sizeof(char), nRecords * sizeof(CReverseAppMapEntry));
        archive.Release();
        blknum_t cur = 0;
        for (uint32_t i = 0; i < nAddrs1; i++) {
            blkrange_t r;
            r.first = cur + addresses1[i].offset;
            r.second = r.first + addresses1[i].cnt - 1;
            reverseAddrRanges.push_back(r);
        }
        // for (uint32_t i = 0; i < nRecords; i++) {
        //     if (!(i % 9765)) {
        //         LOG_INFO(padNum9(reverseAppMap[i].blk), ".", padNum5(reverseAppMap[i].tx), " ",
        //         padNum7(reverseAppMap[i].n));
        //     }
        // }
        // LOG_INFO("Finished reading ", mapFile, string_q(30, ' '));
        return true;
    }

    // Cleaned up on destruction of the chunk
    reverseAppMap = new CReverseAppMapEntry[nApps];
    if (!reverseAppMap) {
        LOG_ERR("Could not allocate memory for CReverseAppMapEntry");
        return false;
    }
    for (uint32_t i = 0; i < nApps; i++) {
        reverseAppMap[i].n = i;
        reverseAppMap[i].blk = appearances1[i].blk;
        reverseAppMap[i].tx = appearances1[i].txid;
    }

    blknum_t cur = 0;
    for (uint32_t i = 0; i < nAddrs1; i++) {
        blkrange_t r;
        r.first = cur + addresses1[i].offset;
        r.second = r.first + addresses1[i].cnt - 1;
        reverseAddrRanges.push_back(r);
    }

    qsort(reverseAppMap, nApps, sizeof(CReverseAppMapEntry), sortRecords);

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(mapFile, modeWriteCreate, LOCK_WAIT)) {
        LOG_ERR("Could not open file ", mapFile);
        return false;
    }
    archive.Write(reverseAppMap, sizeof(char), nApps * sizeof(CReverseAppMapEntry));
    archive.Release();
    // for (uint32_t i = 0; i < nApps; i++) {
    //     if (!(i % 9765)) {
    //         LOG_INFO(reverseAppMap[i].blk, ".", reverseAppMap[i].tx, " ", reverseAppMap[i].n);
    //     }
    // }
    LOG_PROG("Processed: " + getFilename());
    return true;
}

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
    return !junk;
}

//-----------------------------------------------------------------------
bool COptions::showAddrsInTx(const blkrange_t& range, const CAppearance_mon& app) {
    string_q fn = range_2_Str(range);
    string_q chunkPath = indexFolder_finalized + fn + ".bin";

    if (!theIndex || theIndex->getFilename() != chunkPath) {
        if (theIndex) {
            delete theIndex;
            theIndex = nullptr;
        }
        theIndex = new CIndexArchive(READING_ARCHIVE);
        if (theIndex->ReadIndexFromBinary(chunkPath)) {
            theIndex->LoadReverseMaps(range);
            if (!theIndex->reverseAppMap) {
                LOG_ERR("Could not allocate reverseAppMap");
                return false;
            }
        }
    }

    CReverseAppMapEntry search;
    search.blk = app.blk;
    search.tx = app.txid;
    CReverseAppMapEntry* found = (CReverseAppMapEntry*)bsearch(&search, theIndex->reverseAppMap, theIndex->nApps1,
                                                               sizeof(CReverseAppMapEntry), findRevMapEntry);

    // CIndexedAddress* theAddys = theIndex->addresses1;
    // CIndexedAddress* endOfAddys = theIndex->addresses1 + (theIndex->nAddrs1 & sizeof(CIndexedAddress));
    if (found) {
        CTransaction trans;
        string_q txFilename = getBinaryCacheFilename(CT_TXS, app.blk, app.txid);
        bool inCache = app.blk != 0 && fileExists(txFilename);
        if (inCache) {
            readTransFromBinary(trans, txFilename);

        } else {
            if (app.blk == 0 || app.txid >= 99997) {
                // skip
            } else {
                getTransaction(trans, app.blk, app.txid);
                getFullReceipt(&trans, true);
            }
        }

        size_t cnt = 0;
        // back up in case we hit an entry past the first one
        // LOG_INFO(bGreen, "  Found:    ", found->blk, ".", found->tx, " ", found->n, ": ", isSame(found, &search),
        // cOff);
        while (found > theIndex->reverseAppMap) {
            found--;
            // LOG_INFO(bGreen, "  Found-", cnt++, ":    ", found->blk, ".", found->tx, " ", found->n, ": ",
            //          isSame(found, &search), cOff);
            if (!isSame(found, &search)) {
                found++;
                cnt--;
                break;
            }
        }
        // LOG_INFO(bGreen, "  Start-", cnt, ":    ", found->blk, ".", found->tx, " ", found->n, ": ",
        //          isSame(found, &search), cOff);
        // bool ick = false;
        size_t start = 0;
        CReverseAppMapEntry* endOfRevMap = theIndex->reverseAppMap + (theIndex->nApps1 * sizeof(CReverseAppMapEntry));
        while (found < endOfRevMap && isSame(found, &search)) {
            // if (found->n > theIndex->nApps) {
            //     LOG_INFO(bRed, "Past end of array", cOff);
            // } else {
            // LOG_INFO("found->n: ", found->n, " ", theIndex->reverseAddrRanges.size());
            for (size_t i = start; i < theIndex->reverseAddrRanges.size(); i++) {
                blkrange_t* r = &theIndex->reverseAddrRanges[i];
                // if (!(i % 2001)) {
                //     LOG_INFO("  found->n: ", found->n, " r: ", r->first, ".", r->second, "\r");
                // }
                if (inRange(found->n, *r)) {
                    // LOG_INFO(bYellow, "  found one: ", found->n, " ", found->blk, ".", found->tx, " ",
                    //          bytes_2_Addr(theIndex->addresses1[i].bytes), cOff);
                    CAppearance app;
                    app.bn = found->blk;
                    app.tx = found->tx;
                    app.addr = bytes_2_Addr(theIndex->addresses1[i].bytes);
                    if (assignReason(accountedFor, app, trans)) {
                        showApp(app, this);
                    }
                    // cout << found->blk << "," << found->tx << "," < < < < endl;
                    start = i;
                    // ick = true;
                    // LOG_INFO("I am here...");
                    break;
                }
            }
            // if (ick) {
            //     LOG_INFO("Jumping to here");
            //     ick = false;
            // }
            // }
            // while () {
            //     if (found->n < theIndex->nAddrs1) {
            //         cout << found->n << " " << found->blk << "." << found->tx << " "
            //              << bytes_2_Addr(theIndex->addresses1[found->n].bytes) << endl;
            //     }
            // }
            // LOG_INFO("isSame: ", found->n, " ", isSame(found, &search));
            found++;
            // LOG_INFO("isSame: ", found->n, " ", isSame(found, &search));
            // LOG_INFO(found, " ", endOfRevMap, " (", size_t(endOfRevMap - found), "): ", (found < endOfRevMap));
            // LOG_INFO("I am here");
        }
        // LOG_ERR("Done with this appearance.");
    } else {
        LOG_ERR("Appearance not found.");
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
// For neighbors we handle the entire production of data in the _Pre
// because what we actually want to do is scan across the index chunks
bool neighbors_Pre(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    LOG_INFO("Processing address ", opt->accountedFor.address);
    opt->neighborSelfies = getEnvStr("NEIGHBOR_SELFIES") == "true";

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
        // LOG_INFO(bYellow, "Clearing the reverse map for ", opt->theIndex->getFilename(), cOff);
        delete opt->theIndex;
        opt->theIndex = nullptr;
    } else {
        LOG_INFO(bYellow, "Clearing the reverse map", cOff);
    }

    LOG_INFO("   Done...");
    return false;  // !shouldQuit();
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
    // LOG_INFO("Found ", ranges.size(), " chunks");
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
