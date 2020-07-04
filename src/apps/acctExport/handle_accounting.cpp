/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::exportAccounting(void) {
    ENTER("exportData");

    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;
    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);

    CReconciliation lastStatement;
    if (items.size() > 0 && first_record != 0)
        lastStatement.endBal = getBalanceAt(expContext().accountedFor, items[0].blk - 1);

    bool first = true;
    for (size_t i = 0; i < items.size() && (!freshen || (nExported < freshen_max)); i++) {
        const CAppearance_base* item = &items[i];
        if (shouldQuit() || item->blk >= ts_cnt)
            break;

        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = item->blk;
            CTransaction trans;
            trans.pBlock = &block;
            string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
            if (item->blk != 0 && fileExists(txFilename)) {
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                // This data isn't stored, so we need to recreate it
                if (accounting) {
                    blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.bn = trans.blockNumber;
                    nums.ts = trans.timestamp;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, next, &trans);
                    // trans.reconciliations.clear();
                    trans.reconciliations.push_back(nums);
                    trans.statements.clear();
                    CReconciliationOutput st(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                addNeighbor(fromAddrMap, trans.from);
                addNeighbor(toAddrMap, trans.to);
                for (auto log : trans.receipt.logs)
                    addNeighbor(emitterAddrMap, log.address);
                for (auto trace : trans.traces) {
                    addNeighbor(fromTraceAddrMap, trace.action.from);
                    addNeighbor(toTraceAddrMap, trace.action.to);
                }

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && (nExported % FREQ)) {
                    blknum_t current = first_record + nExported;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << monitors[0].address << "\r";
                    LOG_PROGRESS1("Reading", current, nTransactions, post.str());
                }

            } else {
                if (item->blk == 0) {
                    address_t addr = prefundAddrMap[item->txid];
                    trans.transactionIndex = item->txid;
                    trans.loadAsPrefund(addr, prefundWeiMap[addr]);

                } else if (item->txid == 99997 || item->txid == 99998 || item->txid == 99999) {
                    trans.loadAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

                } else {
                    getTransaction(trans, item->blk, item->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                if (accounting) {
                    blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.bn = trans.blockNumber;
                    nums.ts = trans.timestamp;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, next, &trans);
                    CReconciliationOutput st(nums);
                    trans.reconciliations.push_back(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                addNeighbor(fromAddrMap, trans.from);
                addNeighbor(toAddrMap, trans.to);
                for (auto log : trans.receipt.logs)
                    addNeighbor(emitterAddrMap, log.address);
                for (auto trace : trans.traces) {
                    addNeighbor(fromTraceAddrMap, trace.action.from);
                    addNeighbor(toTraceAddrMap, trace.action.to);
                }

                if (articulate) {
                    abiMap[trans.to]++;
                    if (abiMap[trans.to] == 1 || fileExists(getAbiPath(trans.to))) {
                        CStringArray unused;
                        loadAbiAndCache(abis, trans.to, false, unused);
                    }
                    abis.articulateTransaction(&trans);

                    for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
                        CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];
                        string_q str = log->Format();
                        if (contains(str, bytesOnly)) {
                            abiMap[log->address]++;
                            if (abiMap[log->address] == 1 || fileExists(getAbiPath(log->address))) {
                                CStringArray unused;
                                loadAbiAndCache(abis, log->address, false, unused);
                            }
                            abis.articulateLog(log);
                        }
                    }

                    for (size_t j = 0; j < trans.traces.size(); j++) {
                        CTrace* trace = (CTrace*)&trans.traces[j];
                        abiMap[trace->action.to]++;
                        if (abiMap[trace->action.to] == 1 || fileExists(getAbiPath(trace->action.to))) {
                            CStringArray unused;
                            loadAbiAndCache(abis, trace->action.to, false, unused);
                        }
                        abis.articulateTrace(trace);
                    }
                }
                if ((write_opt & CACHE_TXS))
                    writeTransToBinary(trans, txFilename);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && (nExported % FREQ)) {
                    blknum_t current = first_record + nExported;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << monitors[0].address << "\r";
                    LOG_PROGRESS1("Extracting", current, nTransactions, post.str());
                }
            }

            if (logs) {
                for (auto log : trans.receipt.logs) {
                    if (!emitter || log.address == monitors[0].address) {
                        if (isJson && shouldDisplay && !first)
                            cout << ", ";
                        if (shouldDisplay)
                            cout << log.Format() << endl;
                        nExported++;
                        first = false;
                    }
                }
            } else {
                if (isJson && shouldDisplay && !first)
                    cout << ", ";
                if (shouldDisplay)
                    cout << trans.Format() << endl;
                nExported++;
                first = false;
            }
        }
    }

    LOG_PROGRESS1("Reported", (first_record + nExported), nTransactions,
                  " txs for address " + monitors[0].address + "\r");

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportNeighbors();

    EXIT_NOMSG(true);
}

#if 0
TODO: If an abi file is changed, we should re-articulate.
#endif

// #if 0
// * -------------------------------------------------------------------------
// * This source code is confidential proprietary information which is
// * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
// * All Rights Reserved
// *------------------------------------------------------------------------ * /
//#include "options.h"
//
////-----------------------------------------------------------------------
// bool COptions::exportAccounting(void) {
//    ENTER("exportAccounting");
//
//    if (items.size() == 0)
//        EXIT_FAIL("Nothing to export. Quitting...");
//    articulate = true;
//
//    ASSERT(isApiMode());
//    ASSERT(!freshen && !appearances && !logs && !receipts && !traces && articulate);
//    ASSERT(expContext().exportFmt == API1);
//    ASSERT(monitors.size() == 1);
//    ASSERT(nodeHasBalances(false));
//
//    fileInfo knownAbis = getNewestFileInFolder(configPath("known_abis/"));
//    bool knownIsStale = false;
//    if (oldestMonitor < knownAbis.fileTime) {
//        // The known abis cache has been updated. We need to re-articulate this cache
//        knownIsStale = true;
//    }
//
//    bool first = true;
//    cout << "[";
//    for (size_t i = 0; i < items.size() && !shouldQuit() && items[i].blk < ts_cnt; i++) {
//        const CAppearance_base* item = &items[i];
//        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
//            CBlock block;
//            block.blockNumber = item->blk;
//            CTransaction trans;
//            trans.pBlock = &block;
//
//            string_q codePath = "Unknown";
//            static CReconciliation lastStatement;
//
//            string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
//            if (item->blk != 0 && fileExists(txFilename)) {
//                // Note: all data other than the articulation is from the node, so if we have the file we can
//                // use the node data (as it never changes), but...
//                readTransFromBinary(trans, txFilename);
//                trans.finishParse();
//                trans.pBlock = &block;
//                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];
//
//                CReconciliation rec = trans.reconciliations[trans.reconciliations.size() - 1];
//                lastStatement = rec;
//
//                // This data isn't stored, so we need to recreate it
//                CReconciliationOutput st(rec);
//                trans.statements.push_back(st);
//
//                // ...if the cached transaction is older than it's abi, we need to re-articulate
//                // TODO(tjayrush): stale abis for traces are ignored
//                bool abiIsStale =
//                    fileExists(getAbiPath(trans.to)) && oldestMonitor < fileLastModifyDate(getAbiPath(trans.to));
//                if (knownIsStale || abiIsStale) {
//                    abis.articulateTransaction(&trans);
//                    writeTransToBinary(trans, txFilename);
//                    codePath = "Rearticulating ";
//                    //LOG_INFO("rearticulating");
//                } else {
//                    codePath = "Reading ";
//                }
//
//            } else {
//                if (item->blk == 0) {
//                    address_t addr = prefundAddrMap[item->txid];
//                    trans.transactionIndex = item->txid;
//                    trans.loadAsPrefund(addr, prefundWeiMap[addr]);
//                } else if (item->txid == 99997 || item->txid == 99998 || item->txid == 99999) {
//                    trans.loadAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);
//                } else {
//                    getTransaction(trans, item->blk, item->txid);
//                    getFullReceipt(&trans, true);
//                }
//                trans.pBlock = &block;
//                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];
//
//                blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
//                CReconciliation nums;
//                nums.blockNum = trans.blockNumber;
//                CStringArray corrections;
//                nums.reconcile(corrections, lastStatement, next, &trans);
//                trans.reconciliations.push_back(nums);
//                CReconciliationOutput st(nums);
//                trans.statements.push_back(st);
//                if (toAddrMap[trans.to] == 1 || fileExists(getAbiPath(trans.to))) {
//                    CStringArray unused;
//                    loadAbiAndCache(abis, trans.to, false, unused);
//                }
//                abis.articulateTransaction(&trans);
//                writeTransToBinary(trans, txFilename);
//                lastStatement = nums;
//                codePath = "Extracting ";
//            }
//            toAddrMap[trans.to]++;
//            fromAddrMap[trans.from]++;
//            if (!first)
//                cout << ", ";
//            nExported++;
//            cout << trans.Format() << endl;
//            first = false;
//            HIDE_FIELD(CFunction, "message");
//            if (!isTestMode())
//                LOG_INFO(codePath, i, " of ", nTransactions, " records (max ", nProcessing, ")          \r");
//        }
//    }
//    cout << "]";
//
//    for (auto monitor : monitors) {
//        string_q path = getMonitorPath(monitor.address);
//        if (fileExists(path)) {
//            if (items.size() > 0)
//                monitor.writeLastExport(items[items.size() - 1].blk);
//            ostringstream os;
//            os << "touch " << path;
//            // clang-format off
//            if (system(os.str().c_str())) {}  // Don't remove. Silences warnings
//            // clang-format on
//        }
//    }
//
//    reportNeighbors();
//    EXIT_NOMSG(true);
//}
//
//#else
/*
//(write_opt & CACHE_TRACES)

/ *-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------* /
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportAccounting(void) {
    ENTER("exportAccounting");

    ASSERT(isApiMode());
    ASSERT(!freshen && !appearances && !logs && !receipts && !traces && articulate);
    ASSERT(expContext().exportFmt == API1);
    ASSERT(monitors.size() == 1);

    if (items.size() == 0)
        EXIT_FAIL("Nothing to export. Quitting...");

    CReconciliation lastStatement;
    lastStatement.blockNum = NOPOS;

    bool first = true;
    cout << "[";

    nCacheItemsRead = 0;
    nExported = 0;

    string_q readFilename = getMonitorCach(expContext().accountedFor);
    bool readFileExists = fileExists(readFilename);

    LOG8(string_q(120, '+'));
    LOG8("readFile: ", readFilename, " (", fileSize(readFilename), ") readFileExists: ", readFileExists);

    if (readFileExists && fileSize(readFilename) > 0) {
        // If the cache already exists, we read it into memory...
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(readFilename, modeReadOnly, LOCK_WAIT)) {
            archive >> nCacheItemsRead;
            archive >> lastStatement.blockNum;
            for (blknum_t i = 0; i < nCacheItemsRead; i++) {
                // Read in the data...
                CTransaction trans;
                archive >> trans;
                archive >> trans.statements;
                archive >> trans.traces;

                // We've read in block 'n', so we can start at 'n + 1'...
                scanRange.first = trans.blockNumber + 1;
                // Remember which addresses we've seen...
                fromNameExistsMap[trans.from]++;
                toNameExistsMap[trans.to]++;

                // If we have the file, ignore this range test and just deliver the entire file
                if (true) {  // inRange(i, first_record, (first_record + max_records - 1))) {

                    // Display the transaction...
                    abis.articulateTransaction(&trans);
                    if (!first)
                        cout << ", ";
                    cout << trans.Format() << endl;
                    first = false;
                    nExported++;
                    if (!isTestMode())
                        LOG_INFO("Reading ", (i + 1), " of ", nTransactions, " cache records (max ", nProcessing,
                                 ").          \r");
                }
            }
            archive.Release();

        } else if (readFileExists) {
            EXIT_FAIL("Could not open file " + readFilename + " or file size is zero. Quitting...");
        }
    }

    // At this point, either the file was empty or we've displayed all transactions in the file. Remember this...
    nCacheItemsWritten = nCacheItemsRead;

    // We open the file in staging to protect it from interruption...
    LOG8(string_q(120, '-'));

    string_q stagingFilename = getMonitorCach(expContext().accountedFor);
    bool stagingFileExists = false;
    if (readFileExists) {
        copyFile(readFilename, stagingFilename);
        stagingFileExists = fileExists(stagingFilename);
    }

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(stagingFilename, stagingFileExists ? modeReadWrite : modeWriteCreate, LOCK_WAIT))
        EXIT_FAIL("Could not open file " + stagingFilename + ". Quitting...");

    if (!stagingFileExists) {
        // If the file did not yet exist, we need to save some space of the counts, so write those and flush...
        lockSection(true);
        archive.Seek(0, SEEK_SET);
        archive << 0;  // set it to zero until we can get it right
        archive << lastStatement.blockNum;
        archive.flush();
        lockSection(false);

    } else {
        // Otherwise go to the end of the file.
        archive.Seek(0, SEEK_END);
        lastStatement.endBal = lastStatement.endBalCalc = getBalanceAt(expContext().accountedFor, lastStatement.blockNum);
    }

    LOG8(string_q(120, '='));

    for (size_t i = 0; i < items.size() && !shouldQuit() && items[i].blk <= scanRange.second; i++) {
        const CAppearance_base* item = &items[i];
        bool include = inRange((blknum_t)item->blk, scanRange.first, scanRange.second);
        bool dClude = nCacheItemsWritten < max_records;
        bool tClude = items[i].blk < ts_cnt;

        if (include && dClude && tClude) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = item->blk;
            CTransaction trans;
            trans.pBlock = &block;
            if (item->blk == 0) {
                address_t addr = prefundAddrMap[item->txid];
                trans.transactionIndex = item->txid;
                trans.loadAsPrefund(addr, prefundWeiMap[addr]);

            } else if (item->txid == 99997 || item->txid == 99998 || item->txid == 99999) {
                trans.loadAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

            } else {
                getTransaction(trans, item->blk, item->txid);
                getFullReceipt(&trans, true);
            }

            trans.pBlock = &block;
            trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

            blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;

            CReconciliation nums;
            nums.blockNum = trans.blockNumber;
            CStringArray corrections;
            nums.reconcile(corrections, lastStatement, next, &trans);
            lastStatement = nums;
            CReconciliationOutput st(nums);
            trans.statements.push_back(st);

            fromNameExistsMap[trans.from]++;
            toNameExistsMap[trans.to]++;

            abis.articulateTransaction(&trans);

            nCacheItemsWritten++;

            lockSection(true);
            archive << trans;
            archive << trans.statements;
            archive << trans.traces;
            archive.Seek(0, SEEK_SET);
            archive << nCacheItemsWritten;
            archive << lastStatement.blockNum;
            archive.Seek(0, SEEK_END);
            archive.flush();
            monitors[0].writeLastExport(lastStatement.blockNum);
            lockSection(false);
            LOG8("wrote: ", trans.blockNumber, ".", trans.transactionIndex, ": ", nCacheItemsWritten);

            if (!first)
                cout << ", ";
            cout << trans.Format() << endl;
            first = false;
            nExported++;
            // LOG_INFO("Exporting ", nCacheItemsWritten, " of ", nTransactions, " records (max ", nProcessing,
            //         ").          \r");
        }
    }
    archive.Release();
    if (nCacheItemsWritten > nCacheItemsRead) {
        // If we wrote anything, copy the file to production
        lockSection(true);
        string_q prodFilename = getMonitorCach(expContext().accountedFor);
        if (fileExists(stagingFilename))
            moveFile(stagingFilename, prodFilename);
        lockSection(false);
    }

    LOG8(string_q(120, '@'));

    cout << "]" << endl;

    reportNeighbors();

    LOG_INFO(string_q(120, ' '), "\r");
    EXIT_NOMSG(true);
}
*/
//#endif
