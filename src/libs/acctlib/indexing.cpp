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
#include "indexing.h"

namespace qblocks {

//----------------------------------------------------------------
static const string_q STR_STEP1 = "  Extracting addresses...";
static const string_q STR_STEP2 = STR_STEP1 + "extracting appearances...";
static const string_q STR_STEP3 = STR_STEP2 + "exporting...";
static const string_q STR_STEP4 = STR_STEP3 + "finalizing...";
static const string_q STR_STEP5 = STR_STEP4 + "binary file created ";
static const string_q STR_STEP5_A = STR_STEP4 + "ascii file created ";

//----------------------------------------------------------------
void writeIndexAsAscii(const string_q& outFn, const CStringArray& lines) {
    ASSERT(!fileExists(outFn));

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CIndexedAppearanceArray blockTable;

    LOG_INFO(cYellow, STR_STEP1, cOff, "\r");

    // We want to notify 12 times
    uint64_t notifyCnt = lines.size() / 12;
    uint64_t progress = 0;

    string_q msg = "";
    ostringstream addrStream;
    for (const auto& line : lines) {
        if (!(++progress % notifyCnt)) {
            msg += ".";
            LOG_INFO(cYellow, STR_STEP1, msg, cOff, "\r");
        }
        CStringArray parts;
        explode(parts, line, '\t');
        CIndexedAppearance rec(parts[1], parts[2]);
        blockTable.push_back(rec);
        if (!prev.empty() && parts[0] != prev) {
            addrStream << prev << "\t";
            addrStream << padNum6(offset) << "\t";
            addrStream << padNum6(cnt) << endl;
            offset += cnt;
            cnt = 0;
            nAddrs++;
        }
        cnt++;
        prev = parts[0];
    }
    // The above algo always misses the last address, so we add it here
    addrStream << prev << "\t";
    addrStream << padNum6(offset) << "\t";
    addrStream << padNum6(cnt) << endl;
    nAddrs++;

    LOG_INFO(cYellow, STR_STEP2, cOff, "\r");
    ostringstream blockStream;
    for (auto record : blockTable) {
        blockStream << padNum9(record.blk) << "\t";
        blockStream << padNum5(record.txid) << endl;
    }

    LOG_INFO(cYellow, STR_STEP3, cOff, "\r");
    ostringstream headerStream;
    headerStream << padNum7(MAGIC_NUMBER) << "\t";
    headerStream << versionHash << "\t";
    headerStream << padNum7(nAddrs) << "\t";
    headerStream << padNum7((uint32_t)blockTable.size()) << endl;

    LOG_INFO(cYellow, STR_STEP4, cOff, "\r");
    lockSection();
    stringToAsciiFile(outFn, headerStream.str() + addrStream.str() + blockStream.str());
    unlockSection();

    LOG_INFO(cYellow, STR_STEP5_A, greenCheck, cOff);
}

//----------------------------------------------------------------
bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines, FILEVISITOR pinFunc, void* pinFuncData) {
    // ASSUMES THE ARRAY IS SORTED!

    ASSERT(!fileExists(outFn));
    string_q tmpFile = substitute(outFn, ".bin", ".tmp");

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CIndexedAppearanceArray blockTable;

    hashbytes_t hash = hash_2_Bytes(versionHash);
    LOG8("versionHash: ", versionHash);

    CBloomArray blooms;

    // We want to notify 12 times
    uint64_t notifyCnt = lines.size() / 12;
    uint64_t progress = 0;

    LOG_INFO(cYellow, STR_STEP1, cOff, "\r");

    CArchive archive(WRITING_ARCHIVE);
    archive.Lock(tmpFile, modeWriteCreate, LOCK_NOWAIT);
    archive.Seek(0, SEEK_SET);  // write the header even though it's not fully detailed to preserve the space
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)blockTable.size());  // not accurate yet
    string_q msg = "";
    for (size_t l = 0; l < lines.size(); l++) {
        if (!(++progress % notifyCnt)) {
            msg += ".";
            LOG_INFO(cYellow, STR_STEP1, msg, cOff, "\r");
        }
        string_q line = lines[l];
        ASSERT(countOf(line, '\t') == 2);
        CStringArray parts;
        explode(parts, line, '\t');
        CIndexedAppearance rec(parts[1], parts[2]);
        blockTable.push_back(rec);
        if (!prev.empty() && parts[0] != prev) {
            addToSet(blooms, prev);
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
    addToSet(blooms, prev);
    addrbytes_t bytes = addr_2_Bytes(prev);
    archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
    archive.Write(offset);
    archive.Write(cnt);
    nAddrs++;

    LOG_INFO(cYellow, STR_STEP2, cOff, "\r");
    for (auto record : blockTable) {
        archive.Write(record.blk);
        archive.Write(record.txid);
    }

    LOG_INFO(cYellow, STR_STEP3, cOff, "\r");
    archive.Seek(0, SEEK_SET);  // re-write the header now that we have full data
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)blockTable.size());
    archive.Release();

    // We've built the data in a temporary file. We do this in case we're interrupted during the building of the
    // data so it's not corrupted. In this way, we only move the data to its final resting place once. It's safer.
    LOG_INFO(cYellow, STR_STEP4, cOff, "\r");
    string_q bloomFile = substitute(substitute(outFn, "/finalized/", "/blooms/"), ".bin", ".bloom");
    lockSection();                          // disallow control+c
    writeBloomToBinary(bloomFile, blooms);  // write the bloom file
    copyFile(tmpFile, outFn);               // move the index file
    ::remove(tmpFile.c_str());              // remove the tmp file
    unlockSection();

    LOG_INFO(cYellow, STR_STEP5, greenCheck, cOff);

    return (pinFunc ? ((*pinFunc)(outFn, pinFuncData)) : true);
}

//--------------------------------------------------------------
bool chunkVisitFunc(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", chunkVisitFunc, data);

    } else {
        CChunkVisitor* visitor = (CChunkVisitor*)data;  // NOLINT
        if (!visitor || !visitor->indexFunc)
            return false;
        CIndexArchive archive(READING_ARCHIVE);
        archive.ReadIndexFromBinary(path);
        return (*visitor->indexFunc)(archive, visitor->callData);
    }
    return true;
}

//--------------------------------------------------------------
bool forEveryIndexChunk(INDEXCHUNKFUNC func, void* data) {
    CChunkVisitor visitor;
    visitor.indexFunc = func;
    visitor.callData = data;
    return forEveryFileInFolder(indexFolder_finalized, chunkVisitFunc, &visitor);
}

//--------------------------------------------------------------
bool bloomVisitFunc(const string_q& path, void* data) {
    return true;
}

//--------------------------------------------------------------
bool forEveryIndexBloom(INDEXBLOOMFUNC func, void* data) {
    return forEveryFileInFolder(indexFolder_blooms, bloomVisitFunc, data);
}

//--------------------------------------------------------------
bool addressVisitFunc(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", chunkVisitFunc, data);

    } else {
        CChunkVisitor* visitor = (CChunkVisitor*)data;  // NOLINT
        if (!visitor || !visitor->addrFunc)
            return false;
        CIndexArchive archive(READING_ARCHIVE);
        archive.ReadIndexFromBinary(path);
        for (uint64_t i = 0; i < archive.nAddrs; i++) {
            CIndexedAddress* rec = &archive.addresses[i];
            address_t addr = bytes_2_Addr(rec->bytes);
            bool ret = (*visitor->addrFunc)(addr, visitor->callData);
            if (!ret)
                return false;
        }
    }
    return true;
}

//--------------------------------------------------------------
bool forEveryAddressInIndex(ADDRESSFUNC func, const blkrange_t& range, void* data) {
    CChunkVisitor visitor;
    visitor.addrFunc = func;
    visitor.range = range;
    visitor.callData = data;
    return forEveryFileInFolder(indexFolder_finalized, addressVisitFunc, &visitor);
    return true;
}

//--------------------------------------------------------------
bool hasCodeAt(const address_t& addr, blknum_t blk) {
    return !getCodeAt(addr, blk).empty();
}

//--------------------------------------------------------------
bool smartContractVisitFunc(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", chunkVisitFunc, data);

    } else {
        CChunkVisitor* visitor = (CChunkVisitor*)data;  // NOLINT
        if (!visitor || !visitor->addrFunc)
            return false;
        CIndexArchive archive(READING_ARCHIVE);
        archive.ReadIndexFromBinary(path);
        for (uint64_t i = 0; i < archive.nAddrs; i++) {
            CIndexedAddress* rec = &archive.addresses[i];
            address_t addr = bytes_2_Addr(rec->bytes);
            if (hasCodeAt(addr, visitor->range.first)) {
                bool ret = (*visitor->addrFunc)(addr, visitor->callData);
                if (!ret)
                    return false;
            }
        }
    }
    return true;
}

//--------------------------------------------------------------
bool forEverySmartContractInIndex(ADDRESSFUNC func, void* data) {
    CChunkVisitor visitor;
    visitor.addrFunc = func;
    visitor.callData = data;
    visitor.range.first = getBlockProgress(BP_CLIENT).client;
    return forEveryFileInFolder(indexFolder_finalized, smartContractVisitFunc, &visitor);
    return true;
}

//    //--------------------------------------------------------------
//    bool visitIndex(CIndexArchive& chunk, void* data) {
//        string_q fn = substitute(chunk.getFilename(), indexFolder_finalized, "");
//        LOG_INFO(fn, "\t", chunk.nApps, "\t", chunk.nAddrs, "\t", fileSize(chunk.getFilename()));
//
//        uint32_t cnt = 0;
//        CAppearanceArray apps;
//        for (uint64_t a = 0 ; a < chunk.nAddrs ; a++) {
//            CIndexedAddress* addr = &chunk.addresses[a];
//            for (uint64_t p = addr->offset ; p < (addr->offset + addr->cnt) ; p++) {
//                CIndexedAppearance* app =&chunk.appearances[p];
//                if (app->txid == 99998) {
//                    CAppearance aa;
//                    aa.bn = app->blk;
//                    aa.tx = app->txid;
//                    aa.addr = bytes_2_Addr(addr->bytes);
//                    aa.reason = "Uncle";
//                    apps.push_back(aa);
//                }
//                if (!(cnt++ % 23))
//                    cerr << "Checking: " << bytes_2_Addr(addr->bytes) << "\t" << app->blk << "\t" << app->txid <<
//                    "\r";cerr.flush();
//            }
//        }
//
//        sort(apps.begin(), apps.end());
//        for (auto app : apps) {
//            size_t count = getUncleCount(app.bn);
//            for (blknum_t i = 0 ; i < count ; i++) {
//                CBlock uncle;
//                getUncle(uncle, app.bn, i);
//                cout << i << "\t" << app.bn << "\t" << uncle.blockNumber << "\t" << (float(uncle.blockNumber + 8 -
//                app.bn) / 8) << endl;
//            }
//        }
//
//        return true;
//    }

//--------------------------------------------------------------
bool visitBloom(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitBloom, data);
    } else {
        (*((size_t*)data))++;  // NOLINT
    }
    return true;
}

//--------------------------------------------------------------
bool bloomsAreInitalized(void) {
    size_t counter = 0;
    forEveryFileInFolder(indexFolder_blooms, visitBloom, &counter);
    return counter > 100;
}

}  // namespace qblocks
