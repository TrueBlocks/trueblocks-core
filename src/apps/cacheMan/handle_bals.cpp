/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

const string_q clearStr(20, ' ');
//------------------------------------------------------------------------
bool handleCacheBals(COptions& options) {

    ASSERT(filenames.size() == 1);

    if (!nodeHasBalances())
        return options.usage("Cannot cache balances on a machine that doesn't have balances. Quitting...");

    // The node needs to be there if we want to cache balances
    nodeRequired();

    CAppearanceArray_base dataArray;

    // Check to see if it's one of the pre-funded accounts
    string_q contents;
    asciiFileToString(configPath("prefunds.txt"), contents);
    CStringArray prefunds;
    explode(prefunds, contents, '\n');
    for (auto const& watch : options.monitors) {
        for (auto prefund : prefunds) {
            address_t account = nextTokenClear(prefund, '\t');
            if (account == watch.address) {
                CAppearance_base item;
                item.blk = 1;
                item.txid = (uint32_t)-1;
                dataArray.push_back(item);
                biguint_t bal = getBalanceAt(watch.address, item.blk);
                continue;
            }
        }
    }

    CArchive txCache(READING_ARCHIVE);
    if (txCache.Lock(options.monitors[0].name, modeReadOnly, LOCK_NOWAIT)) {
        CAppearance_base last;
        while (!txCache.Eof()) {
            CAppearance_base item;
            txCache >> item.blk >> item.txid;
            if (item.blk > 0 || !last.blk) {
                dataArray.push_back(item);
                cerr << "Reading transactions: " << cTeal << item.blk << "." << item.txid << cOff << clearStr << "\r";
                cerr.flush();
            }
            last = item;
        }
        txCache.Release();
        cerr << clearStr << clearStr << "\r";
        cerr.flush();
    } else {
        return options.usage("Could not open file: " + options.monitors[0].name + ". Quitting.");
    }
    sort(dataArray.begin(), dataArray.end());

    establishFolder("./balances/");
    for (auto const& watch : options.monitors) {

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
            if (item.blk == 1 || item.blk >= watch.firstBlock) {
                bal = getBalanceAt(watch.address, item.blk);
                if (bal != lastBal && item.blk != lastBlock) {
                    balCache << item.blk << watch.address << bal;
                    balCache.flush();
                    nWritten++;
                }
                lastBal = bal;
                lastBlock = item.blk;
            }
            cout << "\t";
            cout << cTeal << watch.address << cOff;
            cout << " (" << nWritten << "/" << cnt++ << "/" << dataArray.size() << ") ";
            cout << item.blk << " " << bal << clearStr << "\r";
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

    ASSERT(monitors.size() > 0);

    for (auto const& watch : options.monitors) {

        string_q binaryFilename = "./balances/" + watch.address + ".bals.bin";
        if (!fileExists(binaryFilename))
            return options.usage("Cannot find file " + binaryFilename + ". Quitting...");

        if (fileSize(binaryFilename) > 0) {

            address_t lastAddr;
            // If the binary file exists, we use that
            CArchive balCache(READING_ARCHIVE);
            if (balCache.Lock(binaryFilename, modeReadOnly, LOCK_NOWAIT)) {

                do {

                    CAppearance app;
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
