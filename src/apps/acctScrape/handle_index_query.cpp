/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitFinalIndexFiles(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitFinalIndexFiles, data);

    } else {

        // Pick up some useful data for either method...
        COptions *options = reinterpret_cast<COptions*>(data);

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. Silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || !endsWith(path, ".bin"))
            return !shouldQuit();

//        blknum_t unused =
            bnFromPath(path, options->lastBlockInFile);
        ASSERT(unused != NOPOS);
        ASSERT(options->lastBlockInFile != NOPOS);

        if (options->earliestStart > options->lastBlockInFile)
            return !shouldQuit();

        bool ret = options->visitBinaryFile(path, data);
        return ret && !shouldQuit();

    }
    ASSERT(0); // should not happen
    return !shouldQuit();
}

//----------------------------------------------------------------------------------
bool newReadBloomFromBinary(CNewBloomArray& blooms, const string_q& fileName) {
    blooms.clear();
    CArchive bloomCache(READING_ARCHIVE);
    if (bloomCache.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        uint32_t n;
        bloomCache.Read(n);
        for (size_t i = 0 ; i < n ; i++) {
            bloom_nt bloom;
            uint32_t nI;
            bloomCache.Read(nI);
            bloom.nInserted = nI;
            bloomCache.Read(bloom.bits, sizeof(uint8_t), bloom_nt::BYTE_SIZE);
            blooms.push_back(bloom);
        }
        bloomCache.Close();
        return true;
    }
    return false;
}

//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void *data) {

#define BREAK_PT 5
    static uint32_t n = 0;

    COptions *options = reinterpret_cast<COptions*>(data);
    string_q bPath = substitute(substitute(path, indexFolder_finalized_v2, indexFolder_blooms_v2), ".bin", ".bloom");
    if (options->useBlooms && fileExists(bPath)) {
        CNewBloomArray blooms;
        newReadBloomFromBinary(blooms, bPath);
        bool hit = false;
        for (size_t a = 0 ; a < monitors.size() && !hit ; a++) {
            if (isMember(blooms, monitors[a].address))
                hit = true;
        }

        if (!hit) {
            LOG(options->log_file << "Skipping blocks: " << substitute(path, indexFolder_finalized_v2, "./") << endl);
            if (!(++n%BREAK_PT)) {
                cerr << "Skipping blocks:  " << substitute(path, indexFolder_finalized_v2, "./");
                cerr << string_q((n/(BREAK_PT*7)), '.');
                cerr << "\r";
                cerr.flush();
            }
            // none of them hit, so write last block for each of them
            for (size_t a = 0 ; a < monitors.size() && !hit ; a++)
                monitors[a].writeLastBlock(lastBlockInFile + 1);
            return true;
        }
    }

    LOG(options->log_file << "Searching blocks: " << substitute(path, indexFolder_finalized_v2, "./") << endl);
    if (!(++n%BREAK_PT)) {
        cerr << "Searching blocks: " << substitute(path, indexFolder_finalized_v2, "./");
        cerr << string_q((n/(BREAK_PT*7)), '.');
        cerr << "\r";
        cerr.flush();
    }

    CArchive *chunk = NULL;
    char *rawData = NULL;
    uint32_t nAddrs = 0;

    for (size_t ac = 0 ; ac < monitors.size() && !shouldQuit() ; ac++) {

        CAccountWatch *acct = &monitors[ac];
        string_q filename = getMonitorPath(acct->address);
        bool exists = fileExists(filename);
        acct->fm_mode = (exists ? FM_PRODUCTION : FM_STAGING);

        if (!acct->openCacheFile1()) {
            LOG(options->log_file << "Could not open cache file " << getMonitorPath(acct->address, acct->fm_mode) << ". Quitting..." << endl);
                             cerr << "Could not open cache file " << getMonitorPath(acct->address, acct->fm_mode) << ". Quitting..." << endl;
            return false;
        }

        CAppearanceArray_base items;
        items.reserve(300000);

        addrbytes_t array = addr_2_Bytes(acct->address);

        if (!chunk) {

            chunk = new CArchive(READING_ARCHIVE);
            if (!chunk || !chunk->Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                LOG(options->log_file << "Could not open index file " << path << ". Quitting..." << endl);
                                 cerr << "Could not open index file " << path << ". Quitting..." << endl;
                return false;
            } else {
                LOG(options->log_file << "Index file opened " << path << endl);
            }

            size_t sz = fileSize(path);
            rawData = (char*)malloc(sz + (2*59));
            if (!rawData) {
                LOG(options->log_file << "Could not allocate memory for data. Quitting..." << endl);
                                 cerr << "Could not allocate memory for data. Quitting..." << endl;
                chunk->Release();
                delete chunk;
                chunk = NULL;
                return false;
            } else {
                LOG(options->log_file << "Allocated memory" << endl);
            }
            bzero(rawData, sz + (2*59));
            size_t nRead = chunk->Read(rawData, sz, sizeof(char));
            if (nRead != sz) {
                LOG(options->log_file << "Could not read entire file. Quitting..." << endl);
                                 cerr << "Could not read entire file. Quitting..." << endl;
                return true;
            } else {
                LOG(options->log_file << "Read entire file" << endl);
            }
            CHeaderRecord_base *h = (CHeaderRecord_base*)rawData;
            ASSERT(h->magic == MAGIC_NUMBER);
            ASSERT(bytes_2_Hash(h->hash) == versionHash);
            nAddrs = h->nAddrs;
            //uint32_t nRows = h->nRows; not used
        }

        LOG(options->log_file << "Done reading." << endl);

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

            LOG(options->log_file << "Found" << endl);
            if (items.size()) {
                lockSection(true);
                acct->writeAnArray(items);
                acct->writeLastBlock(lastBlockInFile + 1);
                lockSection(false);
            }
        } else {

            LOG(options->log_file << "Not found" << endl);
            acct->writeLastBlock(lastBlockInFile + 1);
        }

    }

    if (chunk) {
        LOG(options->log_file << "Closing file" << endl);
        chunk->Release();
        delete chunk;
        chunk = NULL;
    }

    if (rawData) {
        LOG(options->log_file << "Freeing memory" << endl);
        delete rawData;
        rawData = NULL;
    }

    return !shouldQuit();
}

//---------------------------------------------------------------
bool visitStagingIndexFiles(const string_q& path, void *data) {
    return false;
}

//---------------------------------------------------------------
bool visitPendingIndexFiles(const string_q& path, void *data) {
    return false;
}
