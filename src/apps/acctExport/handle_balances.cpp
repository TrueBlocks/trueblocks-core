/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportBalances(void) {
    ENTER("exportBalances");

    bool nodeHasBals = nodeHasBalances(false);
    string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
    if (!nodeHasBals) {
        string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);

        // ...and the user has told us about another balance provider...
        if (rpcProvider == balanceProvider || balanceProvider.empty())
            EXIT_FAIL("The RPC server does not have historical state. Quitting...");

        setRpcProvider(balanceProvider);
        if (!nodeHasBalances(false))
            return usage("balanceServer set but it does not have historical state. Quitting...");
    }

    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);
    if (isJson && !freshen)
        cout << "[";

    for (auto monitor : monitors) {
        CAppearanceArray_base apps;
        loadOneAddress(apps, monitor.address);

        uint64_t nDeltas = 0;
        uint64_t lastDelta = 0;
        map<blknum_t, CBalanceDelta> deltasMap;
        string_q balFile = getMonitorBals(monitor.address);
        if (fileExists(balFile)) {
            CArchive balIn(READING_ARCHIVE);
            if (balIn.Lock(balFile, modeReadOnly, LOCK_NOWAIT)) {
                balIn >> nDeltas;
                balIn >> lastDelta;
                while (deltasMap.size() < nDeltas) {
                    CBalanceDelta rec;
                    balIn >> rec;
                    rec.address = monitor.address;
                    deltasMap[rec.blockNumber] = rec;
                }
                balIn.Release();
            }
            if (isTestMode()) {
                cerr << "nDeltas: " << nDeltas << "\tlastDelta: --lastDelta--" << endl;
                for (auto delta : deltasMap)
                    cerr << delta.first << "\t" << delta.second;
            }
        }

        wei_t priorBalance = 0;
        bool first = true;
        for (size_t i = 0;
             i < apps.size() && !shouldQuit() && apps[i].blk < ts_cnt && (!freshen || (nExported < freshen_max)); i++) {
            const CAppearance_base* item = &apps[i];
            if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
                CBalanceRecord record;
                record.blockNumber = item->blk;
                record.transactionIndex = item->txid;
                record.address = monitor.address;

                CBalanceDelta none;
                // handle the prior balance -- note we always have this in the delta map other than zero block
                record.priorBalance = priorBalance;
                // TODO(tjayrush): remove this
                //                if (record.blockNumber < lastDelta) {
                //                    auto it = deltasMap.lower_bound(record.blockNumber);
                ////                    cout << "Getting from cache " << record.blockNumber << " it: " << (it ==
                /// deltasMap.end() ? 10 : it->first) << " " << (it == deltasMap.end() ? none : it->second);
                //                    if (it == deltasMap.end())
                //                        --it;
                //                    record.balance = it->second.balance;
                //                } else {
                //                    cout << ++cnt << " Getting from node " << record.blockNumber << endl;
                record.balance = getBalanceAt(monitor.address, record.blockNumber);
                //                }
                record.diff = (bigint_t(record.balance) - bigint_t(record.priorBalance));

                if (!freshen && !deltas) {
                    if (isJson && !first)
                        cout << ", ";
                    cout << record;
                    nExported++;
                    first = false;
                }
                priorBalance = record.balance;

                CBalanceDelta rec;
                rec.blockNumber = record.blockNumber;
                rec.address = monitor.address;
                rec.balance = record.balance;
                rec.diff = record.diff;
                if (record.diff != 0) {
                    deltasMap[rec.blockNumber] = rec;
                } else {
                    static size_t cnt = 0;
                    if (!(++cnt % 11) || isRedirected() || (freshen && !(cnt % 3)))
                        LOG_INFO("Balance: ", i, " of ", nRead, " (", rec.blockNumber, ")      ", "\r");
                }
            }
        }

        if (deltas) {
            for (auto delta : deltasMap) {
                if (isJson && !first)
                    cout << ", ";
                cout << delta.second;
                nExported++;
                first = false;
            }
        }

        // cache the deltas
        CArchive balOut(WRITING_ARCHIVE);
        if (!isTestMode() && balOut.Lock(balFile, modeWriteCreate, LOCK_NOWAIT)) {
            nDeltas = deltasMap.size();
            lastDelta = (scanRange.second + 1);
            balOut << nDeltas;
            balOut << lastDelta;
            for (auto delta : deltasMap) {
                balOut << delta.second;
            }
            balOut.Release();

            if (isTestMode()) {
                cerr << "nDeltas: " << nDeltas << "\tlastDelta: --lastDelta--" << endl;
                for (auto delta : deltasMap)
                    cerr << delta.first << "\t" << delta.second;
            }
        }
    }

    if (isJson && !freshen)
        cout << "]";

    // return to the default provider
    if (!nodeHasBals)
        setRpcProvider(rpcProvider);

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
