/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitIndexFiles(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitIndexFiles, data);

    } else {

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. Silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || !endsWith(path, ".bin"))
            return !shouldQuit();

        // Pick up some useful data for either method...
        COptions *options = reinterpret_cast<COptions*>(data);
        options->firstBlockInFile = bnFromPath(path, options->lastBlockInFile);
        ASSERT(options->firstBlockInFile != NOPOS);
        ASSERT(options->lastBlockInFile != NOPOS);

        if (options->startScrape > options->lastBlockInFile)
            return !shouldQuit();

        cerr << "Searching file : " << path << "\r"; cerr.flush();
        return options->visitBinaryFile(path, data);
    }
    ASSERT(0); // should not happen
    return !shouldQuit();
}

//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void *data) {

    CArchive *chunk = NULL;
    char *rawData = NULL;
    uint32_t nAddrs = 0;

    for (size_t ac = 0 ; ac < monitors.size() && !shouldQuit() ; ac++) {
        CAccountWatch *acct = &monitors[ac];
        if (!acct->openCacheFile1()) {
            cerr << "Could not open transaction cache file " << getMonitorPath(acct->address) << ". Quitting...";
            return false;
        }

        CAppearanceArray_base items;
        items.reserve(300000);

        addrbytes_t array = addr_2_Bytes(acct->address);
        if (!chunk) {
            chunk = new CArchive(READING_ARCHIVE);
            if (!chunk || !chunk->Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                cerr << "Could not open index file " << path << ". Quitting...";
                return false;
            }

            size_t sz = fileSize(path);
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
            CHeaderRecord_base *h = (CHeaderRecord_base*)rawData;
            ASSERT(h->magic == MAGIC_NUMBER);
            ASSERT(bytes_2_Hash(h->hash) == versionHash);
            nAddrs = h->nAddrs;
            //uint32_t nRows = h->nRows; not used
        }

        CAddressRecord_base search;
        for (size_t i = 0 ; i < 20 ; i++)
            search.bytes[i] = array[i];
        CAddressRecord_base *found =
            (CAddressRecord_base *)bsearch(&search, (rawData+sizeof(CHeaderRecord_base)), nAddrs, sizeof(CAddressRecord_base), findAppearance);

        if (found) {
            CAddressRecord_base *addrsOnFile = (CAddressRecord_base *)(rawData+sizeof(CHeaderRecord_base));
            CAppearance_base *blocksOnFile = (CAppearance_base *)&addrsOnFile[nAddrs];
            for (size_t i = found->offset ; i < found->offset + found->cnt ; i++) {
                CAppearance_base item(blocksOnFile[i].blk, blocksOnFile[i].txid);
                items.push_back(item);
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
