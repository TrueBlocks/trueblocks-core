/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportData(void) {
    ENTER("exportData");

    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;
    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);

    if (isJson && shouldDisplay)
        cout << "[";

    //#define NEW_ACCOUNTING

    bool first = true;
    for (size_t i = 0;
         i < items.size() && !shouldQuit() && items[i].blk < ts_cnt && (!freshen || (nExported < freshen_max)); i++) {
        const CAppearance_base* item = &items[i];
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
#ifdef NEW_ACCOUNTING
                if (accounting) {
                    static CReconciliation lastStatement;
                    blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
                    CReconciliation nums;
                    nums.blockNum = trans.blockNumber;
                    CStringArray corrections;
                    nums.reconcile(corrections, lastStatement, accountedFor, next, &trans);
                    CReconciliationOutput st(nums);
                    trans.statements.push_back(st);
                    lastStatement = nums;
                }

                toNameExistsMap[trans.to]++;
                fromNameExistsMap[trans.from]++;
                if (articulate) {
                    abiMap[trans.to]++;
                    if (abiMap[trans.to] == 1 || fileExists(getAbiPath(trans.to))) {
                        CStringArray unused;
                        loadAbiAndCache(abis, trans.to, false, unused);
                    }
                    abis.articulateTransaction(&trans);

                    for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
                        CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];
                        abiMap[log->address]++;
                        if (abiMap[log->address] == 1 || fileExists(getAbiPath(log->address))) {
                            CStringArray unused;
                            loadAbiAndCache(abis, log->address, false, unused);
                        }
                        abis.articulateLog(log);
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
#else
                if ((write_opt & CACHE_TXS) && !fileExists(txFilename))
#endif
                    writeTransToBinary(trans, txFilename);
            }

            if (traces) {
                // acctExport --traces
                loadTraces(trans, item->blk, item->txid, (write_opt & CACHE_TRACES),
                           (skip_ddos && excludeTrace(&trans, max_traces)));
                for (auto trace : trans.traces) {
                    bool isSuicide = trace.action.selfDestructed != "";
                    bool isCreation = trace.result.newContract != "";

                    if (!isSuicide) {
                        if (!isTestMode() && isApiMode()) {
                            qblocks::eLogger->setEndline('\r');
                            LOG_INFO("\t\t\t\t\t\tGetting trace ", trans.blockNumber, ".", trans.transactionIndex, "-",
                                     trace.getValueByName("traceAddress"), string_q(50, ' '));
                            qblocks::eLogger->setEndline('\n');
                        }
#ifdef NEW_ACCOUNTING
#else
                        if (articulate)
                            abis.articulateTrace(&trace);
#endif
                        if (isJson && shouldDisplay && !first)
                            cout << ", ";
                        nExported++;
                        if (shouldDisplay)
                            cout << trace.Format() << endl;
                        first = false;
                    }

                    if (isSuicide) {  // suicide
                        CTrace copy = trace;
                        copy.action.from = trace.action.selfDestructed;
                        copy.action.to = trace.action.refundAddress;
                        copy.action.callType = "suicide";
                        copy.action.value = trace.action.balance;
                        copy.traceAddress.push_back("s");
                        copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                        copy.action.input = "0x";
                        if (isJson && shouldDisplay && !first)
                            cout << ", ";
                        nExported++;
                        if (shouldDisplay)
                            cout << copy.Format() << endl;
                    }

                    if (isCreation) {  // contract creation
                        CTrace copy = trace;
                        copy.action.from = "0x0";
                        copy.action.to = trace.result.newContract;
                        copy.action.callType = "creation";
                        copy.action.value = trace.action.value;
                        if (copy.traceAddress.size() == 0)
                            copy.traceAddress.push_back("null");
                        copy.traceAddress.push_back("s");
                        copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                        copy.action.input = trace.action.input;
                        if (isJson && shouldDisplay && !first)
                            cout << ", ";
                        nExported++;
                        if (shouldDisplay)
                            cout << copy.Format() << endl;
                    }
                }

            } else {
                if (receipts) {
                    // acctExport --receipts
#ifdef NEW_ACCOUNTING
#else
                    if (articulate)
                        abis.articulateTransaction(&trans);
#endif
                    if (isJson && shouldDisplay && !first)
                        cout << ", ";
                    nExported++;
                    if (shouldDisplay)
                        cout << trans.receipt.Format() << endl;
                    first = false;

                } else if (logs) {
                    // acctExport --logs
                    for (auto log : trans.receipt.logs) {
                        if (!emitter || log.address == monitors[0].address) {
                            if (isJson && shouldDisplay && !first)
                                cout << ", ";
#ifdef NEW_ACCOUNTING
#else
                            if (articulate) {
                                if (toAddrMap[log.address] == 1 || fileExists(getAbiPath(log.address))) {
                                    CStringArray unused;
                                    loadAbiAndCache(abis, log.address, false, unused);
                                }
                                abis.articulateLog(&log);
                            }
#endif
                            nExported++;
                            if (shouldDisplay)
                                cout << log.Format() << endl;
                            first = false;
                        }
                    }

                } else {
#ifdef NEW_ACCOUNTING
#else
                    if (accounting) {
                        static CReconciliation lastStatement;
                        blknum_t next = i < items.size() - 1 ? items[i + 1].blk : NOPOS;
                        CReconciliation nums;
                        nums.blockNum = trans.blockNumber;
                        CStringArray corrections;
                        nums.reconcile(corrections, lastStatement, accountedFor, next, &trans);
                        CReconciliationOutput st(nums);
                        trans.statements.push_back(st);
                        lastStatement = nums;
                    }

                    toAddrMap[trans.to]++;
                    fromAddrMap[trans.from]++;
                    for (auto log : trans.receipt.logs)
                        emitterAddrMap[log.address]++;
                    for (auto trace : trans.traces) {
                        fromTraceAddrMap[trace.action.from]++;
                        toTraceAddrMap[trace.action.to]++;
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
                            abiMap[log->address]++;
                            if (abiMap[log->address] == 1 || fileExists(getAbiPath(log->address))) {
                                CStringArray unused;
                                loadAbiAndCache(abis, log->address, false, unused);
                            }
                            abis.articulateLog(log);
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
#endif
                    if (isJson && shouldDisplay && !first)
                        cout << ", ";
                    nExported++;
                    if (shouldDisplay)
                        cout << trans.Format() << endl;
                    first = false;
                }
            }

            HIDE_FIELD(CFunction, "message");
            if (!isTestMode() && (isApiMode() || !(i % 3))) {
                ostringstream os;
                os << "Exporting " << nExported << " ";
                os << plural(className) << "of ";
                os << nTransactions << " (max " << nProcessing << ") txs for address " << monitors[0].address;
                LOG_INFO(os.str() + "\r");
            }
        }
    }

    if (isJson && shouldDisplay)
        cout << "]";

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportOnNeighbors();
    EXIT_NOMSG(true);
}
