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

        if (options->lastBlockInFile != 0 && options->scanRange.first > options->lastBlockInFile)
            return !shouldQuit();

        if (isTestMode() && options->lastBlockInFile > 5000000)
            return !shouldQuit();

        return options->visitBinaryFile(path, data) && !shouldQuit();
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

#define BREAK_PT 1
//---------------------------------------------------------------
bool COptions::visitBinaryFile(const string_q& path, void *data) {

    string_q l_funcName = "visitBinaryFile";
    static uint32_t n = 0;

    COptions *options = reinterpret_cast<COptions*>(data);
    string_q bPath = substitute(substitute(path, indexFolder_finalized, indexFolder_blooms), ".bin", ".bloom");
    if (options->useBlooms && fileExists(bPath)) {
        CNewBloomArray blooms;
        newReadBloomFromBinary(blooms, bPath);
        bool hit = false;
        for (size_t a = 0 ; a < monitors.size() && !hit ; a++) {
            if (isMember(blooms, monitors[a].address))
                hit = true;
        }

        if (!hit) {
            if (! ( ++n % BREAK_PT)) {
                qblocks::eLogger->setEndline('\r');
                ostringstream os;
                os << bBlue << "Skip blocks" << cOff << " " << substitute(path, indexFolder_finalized, "./");
                LOG_INFO(os.str());
                qblocks::eLogger->setEndline('\n');
            }
            // none of them hit, so write last block for each of them
            for (size_t ac = 0 ; ac < monitors.size() && !hit ; ac++) {
                CAccountWatch *acct = &monitors[ac];
                string_q filename = getMonitorPath(acct->address);
                bool exists = fileExists(filename);
                acct->fm_mode = (exists ? FM_PRODUCTION : FM_STAGING);
                acct->writeLastBlock(lastBlockInFile + 1);
            }
            return true;
        }
    }

    if (! ( ++n % BREAK_PT)) {
        qblocks::eLogger->setEndline('\r');
        ostringstream os;
        os << cYellow << "Scan blocks" << cOff << " " << substitute(path, indexFolder_finalized, "./");
        LOG_INFO(os.str());
        qblocks::eLogger->setEndline('\n');
    }

    CArchive *chunk = NULL;
    char *rawData = NULL;
    uint32_t nAddrs = 0;

    for (size_t ac = 0 ; ac < monitors.size() && !shouldQuit() ; ac++) {

        CAccountWatch *acct = &monitors[ac];
        string_q filename = getMonitorPath(acct->address);
        bool exists = fileExists(filename);
        acct->fm_mode = (exists ? FM_PRODUCTION : FM_STAGING);

        if (!acct->openCacheFile1())
            EXIT_FAIL("Could not open cache file " + getMonitorPath(acct->address, acct->fm_mode) + ".");

        CAppearanceArray_base items;
        items.reserve(300000);

        addrbytes_t array = addr_2_Bytes(acct->address);

        if (!chunk) {

            chunk = new CArchive(READING_ARCHIVE);
            if (!chunk || !chunk->Lock(path, modeReadOnly, LOCK_NOWAIT))
                EXIT_FAIL("Could not open index file " + path + ".");

            size_t sz = fileSize(path);
            rawData = (char*)malloc(sz + (2*59));
            if (!rawData) {
                chunk->Release();
                delete chunk;
                chunk = NULL;
                EXIT_FAIL("Could not allocate memory for data.");
            }

            bzero(rawData, sz + (2*59));
            size_t nRead = chunk->Read(rawData, sz, sizeof(char));
            if (nRead != sz)
                EXIT_FAIL("Could not read entire file.");

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

//---------------------------------------------------------------
bool visitStagingIndexFiles(const string_q& path, void *data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitFinalIndexFiles, data);

    } else {
        cout << path << endl;
        // Pick up some useful data from the options
        COptions *options = reinterpret_cast<COptions*>(data);

        // Filenames in the staging folder take the form 'end.bin' where both 'end' is the
        // latest block in teh file. Silently skips unknown files (such as shell scripts).
        if (!startsWith(path, "0") || !endsWith(path, ".txt"))
            return !shouldQuit();

        cout << options->useBlooms << " " << path << endl;
    }

    return false;
}

//---------------------------------------------------------------
bool visitUnripeIndexFiles(const string_q& path, void *data) {
    return false;
}
