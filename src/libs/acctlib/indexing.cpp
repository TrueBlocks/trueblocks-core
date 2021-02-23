/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
void writeIndexAsAscii(const string_q& outFn, const CStringArray& lines) {
    ASSERT(!fileExists(outFn));

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CAppearanceArray_base blockTable;

    ostringstream os;
    os << "Extracting addresses...";

    ostringstream addrStream;
    for (auto line : lines) {
        CStringArray parts;
        explode(parts, line, '\t');
        CAppearance_base rec(parts[1], parts[2]);
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

    os << "extracting appearances...";
    ostringstream blockStream;
    for (auto record : blockTable) {
        blockStream << padNum9(record.blk) << "\t";
        blockStream << padNum5(record.txid) << endl;
    }

    os << "exporting...";
    ostringstream headerStream;
    headerStream << padNum7(MAGIC_NUMBER) << "\t";
    headerStream << versionHash << "\t";
    headerStream << padNum7(nAddrs) << "\t";
    headerStream << padNum7((uint32_t)blockTable.size()) << endl;

    os << "finalizing...";
    lockSection(true);
    stringToAsciiFile(outFn, headerStream.str() + addrStream.str() + blockStream.str());
    lockSection(false);

    LOG_INFO(cYellow, "  ", os.str(), " ascii file created: ", greenCheck, cOff);
}

//----------------------------------------------------------------
void writeIndexAsBinary(const string_q& outFn, const CStringArray& lines) {
    // ASSUMES THE ARRAY IS SORTED!

    ASSERT(!fileExists(outFn));
    string_q tmpFile = substitute(outFn, ".bin", ".tmp");

    address_t prev;
    uint32_t offset = 0, nAddrs = 0, cnt = 0;
    CAppearanceArray_base blockTable;

    hashbytes_t hash = hash_2_Bytes(versionHash);

    CBloomArray blooms;

    ostringstream os;
    os << "Extracting addresses...";

    CArchive archive(WRITING_ARCHIVE);
    archive.Lock(tmpFile, modeWriteCreate, LOCK_NOWAIT);
    archive.Seek(0, SEEK_SET);  // write the header even though it's not fully detailed to preserve the space
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)blockTable.size());  // not accurate yet
    for (size_t l = 0; l < lines.size(); l++) {
        string_q line = lines[l];
        ASSERT(countOf(line, '\t') == 2);
        CStringArray parts;
        explode(parts, line, '\t');
        CAppearance_base rec(parts[1], parts[2]);
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

    os << "extracting appearances...";
    for (auto record : blockTable) {
        archive.Write(record.blk);
        archive.Write(record.txid);
    }

    os << "exporting...";
    archive.Seek(0, SEEK_SET);  // re-write the header now that we have full data
    archive.Write(MAGIC_NUMBER);
    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
    archive.Write(nAddrs);
    archive.Write((uint32_t)blockTable.size());
    archive.Release();

    // We've built the data in a temporary file. We do this in case we're interrupted during the building of the
    // data so it's not corrupted. In this way, we only move the data to its final resting place once. It's safer.
    os << "finalizing...";
    lockSection(true);  // disallow control+c
    string_q bloomFile = substitute(substitute(outFn, "/finalized/", "/blooms/"), ".bin", ".bloom");
    writeBloomToBinary(bloomFile, blooms);  // write the bloom file
    copyFile(tmpFile, outFn);               // move the index file
    ::remove(tmpFile.c_str());              // remove the tmp file
    lockSection(false);

    LOG_INFO(cYellow, "  ", os.str(), " binary file created: ", greenCheck, cOff);
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
            CAddressRecord_base* rec = &archive.addresses[i];
            address_t addr = bytes_2_Addr(rec->bytes);
            bool ret = (*visitor->addrFunc)(addr, visitor->callData);
            if (!ret)
                return false;
        }
    }
    return true;
}

//--------------------------------------------------------------
bool forEveryAddressInIndex(ADDRESSFUNC func, void* data) {
    CChunkVisitor visitor;
    visitor.addrFunc = func;
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
            CAddressRecord_base* rec = &archive.addresses[i];
            address_t addr = bytes_2_Addr(rec->bytes);
            if (hasCodeAt(addr, visitor->atBlock)) {
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
    visitor.atBlock = getLatestBlock_client();
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
//            CAddressRecord_base* addr = &chunk.addresses[a];
//            for (uint64_t p = addr->offset ; p < (addr->offset + addr->cnt) ; p++) {
//                CAppearance_base* app =&chunk.appearances[p];
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
//            printf("");
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
