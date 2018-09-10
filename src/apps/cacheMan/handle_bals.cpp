/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

const string_q clearStr(' ', 20);
//------------------------------------------------------------------------
bool handleCacheBals(COptions& options) {

    if (!fileExists("./config.toml"))
        return usage("Could not open file: ./config.toml. Quitting.");
    CToml toml("./config.toml");
    options.loadWatches(toml);

    if (!nodeHasBalances())
        return usage("Cannot cache balances on a machine that doesn't have balances. Quitting...");

    CAcctCacheItemArray dataArray;
    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(options.filenames[0], binaryReadOnly, LOCK_NOWAIT)) {
        CAcctCacheItem last;
        while (!txCache.Eof()) {
            CAcctCacheItem item;
            txCache >> item.blockNum >> item.transIndex;
            if (item.blockNum > 0 || !last.blockNum) {
                dataArray.push_back(item);
                cerr << "Reading transactions: " << cTeal << item.blockNum << "." << item.transIndex << cOff << clearStr << "\r";
                cerr.flush();
            }
            last = item;
        }
        txCache.Release();
        cerr << clearStr << clearStr << "\r";
        cerr.flush();
    } else {
        return usage("Could not open file: " + options.filenames[0] + ". Quitting.");
    }
    sort(dataArray.begin(), dataArray.end());

    establishFolder("./balances/");
    for (uint32_t w = 0 ; w < options.watches.size() ; w++) {
        const CAccountWatch *watch = &options.watches[w];

        string_q binaryFilename = "./balances/" + watch->address + ".bals.bin";
        CArchive balCache(WRITING_ARCHIVE);
        if (!balCache.Lock(binaryFilename, binaryWriteCreate, LOCK_WAIT))
            return usage("Cannot open file " + binaryFilename + ". Quitting...");

        uint64_t nWritten = 0;
        biguint_t lastBal = biguint_t((uint64_t)NOPOS);
        biguint_t bal = 0;
        for (uint32_t d = 0 ; d < dataArray.size() ; d++) {
            const CAcctCacheItem *item  = &dataArray[d];
            if (item->blockNum >= watch->firstBlock) {
                bal = getBalance(watch->address, item->blockNum, false);
                if (bal != lastBal) {
                    balCache << item->blockNum << watch->address << bal;
                    balCache.flush();
                    nWritten++;
                }
                lastBal = bal;
            }
            cout << "\t";
            cout << cTeal << watch->address << cOff;
            cout << " (" << nWritten << "/" << d << "/" << dataArray.size() << ") ";
            cout << item->blockNum << " " << bal << clearStr << "\r";
            cout.flush();
        }
        cerr << "\tWrote " << padNum5T(nWritten++) << " balances to binary file " << cTeal << binaryFilename << cOff << clearStr << "\n";
        cerr.flush();
        balCache.Release();
    }

    return true;
}

//------------------------------------------------------------------------
bool listBalances(COptions& options) {

    if (!fileExists("./config.toml"))
        return usage("Could not open file: ./config.toml. Quitting.");
    CToml toml("./config.toml");
    options.loadWatches(toml);

    for (uint32_t w = 0 ; w < options.watches.size() ; w++) {
        const CAccountWatch  *watch = &options.watches[w];

        string_q binaryFilename = "./balances/" + watch->address + ".bals.bin";
        if (!fileExists(binaryFilename))
            return usage("Cannot find file " + binaryFilename + ". Quitting...");

        if (fileSize(binaryFilename) > 0) {

            address_t lastAddr;
            // If the binary file exists, we use that
            CArchive balCache(READING_ARCHIVE);
            if (balCache.Lock(binaryFilename, binaryReadOnly, LOCK_NOWAIT)) {
                while (!balCache.Eof()) {
                    blknum_t bn;
                    address_t addr;
                    biguint_t bal;
                    if (addr != lastAddr)
                        cout << "\n";
                    lastAddr = addr;
                    balCache >> bn >> addr >> bal;
                    cout << addr << "\t" << bn << "\t" << bal << "\n";
                    cout.flush();
                }
            }
        }
    }

    return true;
}
