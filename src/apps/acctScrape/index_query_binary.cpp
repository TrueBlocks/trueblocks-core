/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void *data) {

    CArchive *chunk = NULL;
    char *rawData = NULL;
    uint32_t nAddrs = 0, nRows = 0;

    for (size_t ac = 0 ; ac < monitors.size() && !shouldQuit() ; ac++) {
        CAccountWatch *acct = &monitors[ac];
        if (!acct->openCacheFile1()) {
            cerr << "Could not open transaction cache file " << getMonitorPath(acct->address) << ". Quitting...";
            return false;
        }

        CAcctCacheItemArray items;
        items.reserve(300000);

        string_q pathy = path;
        addrbytes_t array = addr_2_Bytes(acct->address);
        if (!chunk) {
            chunk = new CArchive(READING_ARCHIVE);
            if (!chunk || !chunk->Lock(pathy, modeReadOnly, LOCK_NOWAIT)) {
                cerr << "Could not open index file " << pathy << ". Quitting...";
                return false;
            }

            size_t sz = fileSize(pathy);
            rawData = (char*)malloc(sz + (2*59));
            if (!rawData) {
                cerr << "Could not allocate memory for data. Quitting...";
                chunk->Release();
                delete chunk;
                chunk = NULL;
                return false;
            }
            bzero(rawData, sz + (2*59));
            size_t nRead = chunk->Read(rawData, sz, sizeof(char));
            if (nRead != sz) {
                cerr << "Could not read entire file. Quitting..." << endl;
                return true;
            }
            Header_ondisc *h = (Header_ondisc*)rawData;
            ASSERT(h->magic == MAGIC_NUMBER);
            ASSERT(bytes_2_Hash(h->hash) == versionHash);
            nAddrs = h->nAddrs;
            nRows = h->nRows;
        }

        AddressRecord_ondisc search;
        for (size_t i = 0 ; i < 20 ; i++)
            search.bytes[i] = array[i];
        AddressRecord_ondisc *found = (AddressRecord_ondisc *)bsearch(&search,
                                                                      (rawData+sizeof(Header_ondisc)),
                                                                      nAddrs,
                                                                      sizeof(AddressRecord_ondisc),
                                                                      findRecord);
        if (found) {

            AddressRecord_ondisc *addrsOnFile = (AddressRecord_ondisc *)(rawData+sizeof(Header_ondisc));
            BlockRecord_ondisc *blocksOnFile = (BlockRecord_ondisc *)&addrsOnFile[nAddrs];
            for (size_t i = found->offset ; i < found->offset + found->cnt ; i++) {
                CAcctCacheItem item(blocksOnFile[i].blk, blocksOnFile[i].txid);
                items.push_back(item);
                cout << padNum9(blocksOnFile[i].blk) << "\t" << padNum5(blocksOnFile[i].txid) << endl;
            }

            if (items.size()) {
                lockSection(true);
                acct->writeAnArray(items);
                acct->writeLastBlock(lastBlockInFile + 1);
                lockSection(false);
            }
        } else {
            acct->writeLastBlock(lastBlockInFile + 1);
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

    return !shouldQuit();
}

#if 0
//----------------------------------------------------------------
void searchFile(const string_q& pathIn, CAccountWatchArray *watches) {

//    cerr << "searching file " << pathIn << "\r"; cerr.flush();
    if (!fileExists(pathIn))
        return;

    CArchive chunk(READING_ARCHIVE);
    char *rawData = NULL;
    uint32_t nAddrs = 0, nRows = 0;
    if (chunk.Lock(pathIn, modeReadOnly, LOCK_NOWAIT)) {
        size_t size = fileSize(pathIn);
        rawData = (char*)malloc(size + (2*59));
        if (!rawData)
            return;
        bzero(rawData, size + (2*59));
        size_t nRead = chunk.Read(rawData, size, sizeof(char));
        if (nRead != size) {
            cerr << "Could not read entire file. Quitting..." << endl;
            return;
        }

        Header_ondisc *h = (Header_ondisc*)rawData;
        ASSERT(h->magic == MAGIC_NUMBER);
        ASSERT(bytes_2_Hash(h->hash) == versionHash);
        nAddrs = h->nAddrs;
        nRows = h->nRows;
        chunk.Release();
    }

    cerr << "searching file " << pathIn << "\r"; cerr.flush();
    for (size_t j = 0 ; j < watches->size() ; j++) {
        AddressRecord_ondisc search;
        addrbytes_t bytes = addr_2_Bytes(watches->operator[](j).address);
        for (size_t i = 0 ; i < 20 ; i++)
            search.bytes[i] = bytes[i];
        AddressRecord_ondisc *found = (AddressRecord_ondisc *)bsearch(&search,
                                                                      (rawData+sizeof(Header_ondisc)),
                                                                      nAddrs,
                                                                      sizeof(AddressRecord_ondisc),
                                                                      findRecord);
        if (found) {
            AddressRecord_ondisc *addrsOnFile = (AddressRecord_ondisc *)(rawData+sizeof(Header_ondisc));
            BlockRecord_ondisc *blocksOnFile = (BlockRecord_ondisc *)&addrsOnFile[nAddrs];
            for (size_t i = found->offset ; i < found->offset + found->cnt ; i++)
                cout << padNum9(blocksOnFile[i].blk) << "\t" << padNum5(blocksOnFile[i].txid) << endl;
        }
    }

    ASSERT(rawData);
    delete rawData;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        if (contains(path, ".bin")) {
            visitBinaryFile(path, data);
            //searchFile(path, (CAccountWatchArray *)data);
        }
    }

    return true;
}

//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    acctlib_init(quickQuitHandler);
    if (argc <= 1)
        return 0;

    CAccountWatchArray watches;
    for (int i = 1 ; i < argc; i++) {
        if (isAddress(argv[i])) {
            CAccountWatch watch;
            watch.address = argv[i];
            watches.push_back(watch);
        }
    }
    cerr << watches.size() << endl;
    forEveryFileInFolder(indexFolder_binary_v2, visitFile, &watches);

    acctlib_cleanup();
}

//---------------------------------------------------------------
bool visitBinaryFile(const string_q& path, void *data) {

    CAccountWatchArray *watches = (CAccountWatchArray *)data;

    CArchive *chunk = NULL;
    char *rawData = NULL;
    uint32_t nAddrs = 0, nRows = 0;

    for (size_t ac = 0 ; ac < watches->size() && !shouldQuit() ; ac++) {
        CAccountWatch *acct = &watches->operator[](ac);
        if (!acct->openCacheFile1()) {
            cerr << "Could not open transaction cache file " << getMonitorPath(acct->address) << ". Quitting...";
            return false;
        }

        CAcctCacheItemArray items;
        items.reserve(300000);

        string_q pathy = path; //substitute(path, indexFolder_sorted_v2, "");
        //pathy = "/Users/jrush/src.GitHub/quickBlocks/src/other/addrs/output-bin/" + substitute(pathy,".txt",".bin");
        addrbytes_t array = addr_2_Bytes(acct->address);
        if (!chunk) {
            chunk = new CArchive(READING_ARCHIVE);
            if (!chunk || !chunk->Lock(pathy, modeReadOnly, LOCK_NOWAIT)) {
                cerr << "Could not open index file " << pathy << ". Quitting...";
                return false;
            }

            size_t sz = fileSize(pathy);
            rawData = (char*)malloc(sz + (2*59));
            if (!rawData) {
                cerr << "Could not allocate memory for data. Quitting...";
                chunk->Release();
                delete chunk;
                chunk = NULL;
                return false;
            }
            bzero(rawData, sz + (2*59));
            size_t nRead = chunk->Read(rawData, sz, sizeof(char));
            int32_t back = ((int32_t)sizeof(uint32_t)*2);
            chunk->Seek(-back, SEEK_END);
            nRead = chunk->Read(&nAddrs, sizeof(uint32_t), 1);
            nRead = chunk->Read(&nRows, sizeof(uint32_t), 1);
        }

        AddressRecord_ondisc search;
        for (size_t i = 0 ; i < 20 ; i++)
            search.bytes[i] = array[i];
        AddressRecord_ondisc *found = (AddressRecord_ondisc *)bsearch(&search,
                                                                      rawData,
                                                                      nAddrs,
                                                                      sizeof(AddressRecord_ondisc),
                                                                      findRecord);
        if (found) {

            AddressRecord_ondisc *addrsOnFile = (AddressRecord_ondisc *)rawData;
            BlockRecord_ondisc *blocksOnFile = (BlockRecord_ondisc *)&addrsOnFile[nAddrs];
            for (size_t i = found->offset ; i < found->offset + found->cnt ; i++) {
                CAcctCacheItem item(blocksOnFile[i].blk, blocksOnFile[i].txid);
                items.push_back(item);
                cout << padNum9((uint64_t)blocksOnFile[i].blk) << "\t" << padNum5((uint64_t)blocksOnFile[i].txid) << endl;
            }

            if (items.size()) {
                lockSection(true);
                acct->writeAnArray(items);
                acct->writeLastBlock(lastBlockInFile + 1);
                lockSection(false);
            }
        } else {
            acct->writeLastBlock(lastBlockInFile + 1);
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

    return !shouldQuit();
}
#endif
