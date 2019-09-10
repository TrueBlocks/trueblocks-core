/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportBalances(void) {

    ENTER("exportBalances");

    // If the node we're running against does not provide balances...
    bool nodeHasBals = nodeHasBalances();
    string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
    if (!nodeHasBals) {
        string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);

        // ...and the user has told us about another balance provider...
        if (rpcProvider == balanceProvider || balanceProvider.empty()) {
            EXIT_FAIL("Balances not available.");
        }

        // ..then we release the curl context, change the node server, and get a new context. We will replace this below.
        getCurlContext()->baseURL = balanceProvider;
        getCurlContext()->releaseCurl();
        getCurlContext()->getCurl();
    }

    bool isJson = (exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1);
    if (isJson && !freshen_only)
        cout << "[";

    for (auto monitor : monitors) {
        map<blknum_t, CBalanceRecord> deltas;
        CBalanceRecord deltaRecord;
        deltaRecord.address = monitor.address;
        deltaRecord.blockNumber = 0;
        deltaRecord.balance = prefundWeiMap[monitor.address];
        deltaRecord.diff = (bigint_t(deltaRecord.balance) - 0);
        deltas[0] = deltaRecord;

        CAppearanceArray_base apps;
        loadOneAddress(apps, monitor.address);

        bool first = true;
        for (size_t i = 0 ; i < apps.size() && !shouldQuit() && apps[i].blk < ts_cnt ; i++) {
            const CAppearance_base *item = &apps[i];
            if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
                CBalanceRecord record;
                record.blockNumber      = item->blk;
                record.transactionIndex = item->txid;
                record.address          = monitor.address;
                record.priorBalance     = ((item->blk == 0) ? 0 : getBalanceAt(monitor.address, item->blk-1));
                record.balance          = getBalanceAt(monitor.address, item->blk);
                record.diff             = (bigint_t(record.balance) - bigint_t(record.priorBalance));
                if (record.diff != 0)
                    deltas[item->blk] = record;
                if (!freshen_only && !deltas_only) {
                    if (isJson && !first)
                        cout << ", ";
                    cout << record;
                    nExported++;
                    first = false;
                }
            }
        }
        blknum_t last = (apps.size() > 0 ? apps[apps.size()-1].blk : scanRange.second) + 1;
        deltaRecord.blockNumber = last;
        deltaRecord.transactionIndex = (uint64_t)NOPOS;
        deltaRecord.priorBalance = (uint64_t)NOPOS;
        deltaRecord.balance = (uint64_t)NOPOS;
        deltaRecord.diff = (uint64_t)NOPOS;
        deltas[last] = deltaRecord;

        if (deltas_only) {
            for (auto delta : deltas) {
                if (isJson && !first)
                    cout << ", ";
                cout << delta.second;
                nExported++;
                first = false;
            }
        }
    }

    if (isJson && !freshen_only)
        cout << "]";

    // return to the default provider
    if (!nodeHasBals) {
        getCurlContext()->baseURL = rpcProvider;
        getCurlContext()->releaseCurl();
        getCurlContext()->getCurl();
    }

    EXIT_NOMSG(true);
}

    // So as to keep the file small, we only write balances where there is a change
#if 0
    if (balances.size() == 0 && fileExists(binaryFilename) && fileSize(binaryFilename) > 0) {

        CArchive balCache(READING_ARCHIVE);
        if (balCache.Lock(binaryFilename, modeReadOnly, LOCK_NOWAIT)) {
            blknum_t last = NOPOS;
            address_t lastA;
            do {
                blknum_t bn;
                address_t addr1;
                biguint_t bal;
                balCache >> bn >> addr1 >> bal;
                if (monitor.address == addr1) {
                    if (last != bn || bal != 0) {
                        CEthState newBal;
                        newBal.blockNumber = bn;
                        newBal.balance = bal;
                        record.push_back(newBal);
                        last = bn;
                    }
                }
            } while (!balCache.Eof());
        }
    }
#endif

#if 0
    // First, we try to find it using a binary search. Many times this will hit...
    CEthState search;
    search.blockNumber = blockNum;
    const CEthStateArray::iterator it = find(record.begin(), record.end(), search);
    if (it != record.end())
        return it->balance;

    // ...if it doesn't hit, we need to find the most recent balance
    biguint_t ret = 0;
    for (size_t i = 0 ; i < record.size() ; i++) {
        // if we hit the block number exactly return it
        if (record[i].blockNumber == blockNum)
            return record[i].balance;

        // ...If we've overshot, report the previous balance
        if (record[i].blockNumber > blockNum)
            return ret;

        ret = record[i].balance;
    }

    // We've run off the end of the array, return the most recent balance (if any)
    if (ret > 0)
        return ret;

    // We finally fall to the node in case we're near the head
    return getBalanceAt(addr, blockNum);
}
#endif
