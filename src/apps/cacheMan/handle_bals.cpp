/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

const string_q clearStr(20, ' ');
//------------------------------------------------------------------------
bool handleCacheBals(COptions& options) {

    if (!fileExists("./config.toml"))
        return options.usage("Could not open file: ./config.toml. Quitting.");
    CToml toml("./config.toml");
    options.loadWatches(toml);

    if (!nodeHasBalances())
        return options.usage("Cannot cache balances on a machine that doesn't have balances. Quitting...");

    CAcctCacheItemArray dataArray;

    // Check to see if it's one of the pre-funded accounts
    string_q contents;
    asciiFileToString(configPath("prefunds.txt"), contents);
    CStringArray prefunds;
    explode(prefunds, contents, '\n');
    for (auto const& watch : options.watches) {
        for (auto prefund : prefunds) {
            address_t account = nextTokenClear(prefund, '\t');
            if (account == watch.address) {
                CAcctCacheItem item;
                item.blockNum = 1;
                item.transIndex = NOPOS;
                dataArray.push_back(item);
                biguint_t bal = getBalanceAt(watch.address, item.blockNum);
//                cout << account << "\t" << item << "\t" << bal << endl;
                continue;
            }
        }
    }

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(options.filenames[0], modeReadOnly, LOCK_NOWAIT)) {
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
        return options.usage("Could not open file: " + options.filenames[0] + ". Quitting.");
    }
    sort(dataArray.begin(), dataArray.end());

    establishFolder("./balances/");
    for (auto const& watch : options.watches) {

        string_q binaryFilename = "./balances/" + watch.address + ".bals.bin";
        CArchive balCache(WRITING_ARCHIVE);
        if (!balCache.Lock(binaryFilename, modeWriteCreate, LOCK_WAIT))
            return options.usage("Cannot open file " + binaryFilename + ". Quitting...");

        uint64_t nWritten = 0;
        biguint_t lastBal = biguint_t((uint64_t)NOPOS);
        size_t cnt = 0;
        blknum_t lastBlock = NOPOS;
        biguint_t bal = 0;
        for (auto const& item : dataArray) {
            if (item.blockNum == 1 || item.blockNum >= watch.firstBlock) {
                bal = getBalanceAt(watch.address, item.blockNum);
                if (bal != lastBal && item.blockNum != lastBlock) {
                    balCache << item.blockNum << watch.address << bal;
                    balCache.flush();
                    nWritten++;
                }
                lastBal = bal;
                lastBlock = item.blockNum;
            }
            cout << "\t";
            cout << cTeal << watch.address << cOff;
            cout << " (" << nWritten << "/" << cnt++ << "/" << dataArray.size() << ") ";
            cout << item.blockNum << " " << bal << clearStr << "\r";
            cout.flush();
        }
        cerr << "\tWrote " << padNum5T(nWritten++) << " balances to binary file " << cTeal;
        cerr << binaryFilename << cOff << clearStr << "\n";
        cerr.flush();
        balCache.Release();
    }

    return true;
}

//------------------------------------------------------------------------
bool listBalances(COptions& options) {

    if (!fileExists("./config.toml"))
        return options.usage("Could not open file: ./config.toml. Quitting.");
    CToml toml("./config.toml");
    options.loadWatches(toml);

    for (auto const& watch : options.watches) {

        string_q binaryFilename = "./balances/" + watch.address + ".bals.bin";
        if (!fileExists(binaryFilename))
            return options.usage("Cannot find file " + binaryFilename + ". Quitting...");

        if (fileSize(binaryFilename) > 0) {

            address_t lastAddr;
            // If the binary file exists, we use that
            CArchive balCache(READING_ARCHIVE);
            if (balCache.Lock(binaryFilename, modeReadOnly, LOCK_NOWAIT)) {

                do {

                    CAddressAppearance app;
                    biguint_t bal;
                    balCache >> app.bn >> app.addr >> bal;
                    if (app.bn > 0) {
                        if (app.addr != lastAddr)
                            cout << endl;
                        lastAddr = app.addr;
                        cout << app.addr << "\t" << app.bn << "\t" << bal << endl;
                    }

                } while (!balCache.Eof());
            }
        }
    }

    return true;
}
