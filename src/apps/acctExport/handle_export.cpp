/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportData(void) {

    ENTER("exportData");

    if (exportFmt == JSON1 && !freshenOnly)
        cout << "[";

    bool first = true;
    size_t nExported = 0;
    for (size_t i = 0 ; i < items.size() && !shouldQuit() && items[i].blk < ts_cnt ; i++) {
        const CAppearance_base *item = &items[i];
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {

            CBlock block; // do not move this from this scope
            block.blockNumber = item->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
            if (fileExists(txFilename)) {
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk*2)+1];

            } else {
                if (item->blk == 0) {
                    trans.transactionIndex = item->txid;
                    trans.loadAsPrefund(prefunds, prefundMap[item->txid]);

                } else if (item->txid == 99998 || item->txid == 99999) {
                    trans.loadAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

                } else {
                    getTransaction(trans, item->blk, item->txid);
                    getFullReceipt(&trans, true);
                }
                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk*2)+1];
                if (writeTxs && !fileExists(txFilename))
                    writeTransToBinary(trans, txFilename);
            }

            if (doTraces) {

                loadTraces(trans, item->blk, item->txid, writeTraces, (skipDdos && excludeTrace(&trans, maxTraces)));
                for (auto trace : trans.traces) {

                    bool isSuicide = trace.action.address != "";
                    bool isCreation = trace.result.address != "";

                    if (!isSuicide) {
                        if (doABIs) {
                            abiMap[trace.action.to] = true;
                        } else {
                            if (exportFmt == JSON1 && !first && !freshenOnly)
                                cout << ", ";
                            if (articulate)
                                abis.articulateTrace(&trace);
                            if (!freshenOnly)
                                cout << trace.Format() << endl;
                            first = false;
                        }
                    }

                    if (isSuicide) { // suicide
                        CTrace copy = trace;
                        copy.action.from = trace.action.address;
                        copy.action.to = trace.action.refundAddress;
                        copy.action.callType = "suicide";
                        copy.action.value = trace.action.balance;
                        copy.traceAddress.push_back("s");
                        copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionPosition);
                        copy.action.input = "0x";
                        if (doABIs) {
                            abiMap[trace.action.to] = true;
                        } else {
                            if (exportFmt == JSON1 && !first && !freshenOnly)
                                cout << ", ";
                            if (!freshenOnly)
                                cout << copy.Format() << endl;
                        }
                    }

                    if (isCreation) { // contract creation
                        CTrace copy = trace;
                        copy.action.from = "0x0";
                        copy.action.to = trace.result.address;
                        copy.action.callType = "creation";
                        copy.action.value = trace.action.value;
                        if (copy.traceAddress.size() == 0)
                            copy.traceAddress.push_back("null");
                        copy.traceAddress.push_back("s");
                        copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionPosition);
                        copy.action.input = trace.action.input;
                        if (doABIs) {
                            abiMap[trace.action.to] = true;

                        } else {
                            if (exportFmt == JSON1 && !first && !freshenOnly)
                                cout << ", ";
                            if (!freshenOnly)
                                cout << copy.Format() << endl;
                        }
                    }
                }

            } else {
                if (doLogs) {
                    for (auto log : trans.receipt.logs) {
                        if (exportFmt == JSON1 && !first && !freshenOnly)
                            cout << ", ";
                        if (articulate)
                            abis.articulateLog(&log);
                        if (!freshenOnly)
                            cout << log.Format() << endl;
                        first = false;
                    }
                } else {
                    if (exportFmt == JSON1 && !first) {
                        if (!freshenOnly)
                            cout << ", ";
                        // we only articulate the transaction if we're JSON
                        if (articulate)
                            abis.articulateTransaction(&trans);
                    }
                    if (!freshenOnly)
                        cout << trans.Format() << endl;
                    first = false;
                }
            }

            nExported++;
            HIDE_FIELD(CFunction, "message");
            if (isRedirected()) {  // we are in --output mode
                cerr << "   " << i << " of " << items.size() << " (" << trans.blockNumber << "): " << trans.hash << "\r";
                cerr.flush();
            } else {
                static size_t cnt = 0;
                if (!(++cnt % 71)) { // not reporting every tx is way faster
                    cerr << "   " << i << " of " << items.size() << ": " << trans.hash << "\r";
                    cerr.flush();
                }
            }
        }
    }

    if (doABIs) {
        for (pair<address_t,bool> item : abiMap) {
            if (isContractAt(item.first)) {
                CAbi unused;
                loadAbiAndCache(unused, item.first, false, true, false);
                cout << "ABI for " << item.first << " ";
                cout << (fileExists(getCachePath("abis/" + item.first + ".json")) ? "cached" : "not cached") << endl;
            }
        }
    }

#if 0
/* We need to handle the non-txt case where we're exporting the fully articulated transaction, all it's traces, all its data */
#endif

    if (!freshenOnly)
        cerr << "exported " << nExported << " of " << items.size() << " records." << string_q(45,' ') << "\n";
    cerr.flush();
    if (exportFmt == JSON1 && !freshenOnly)
        cout << "]";

    for (auto watch : monitors)
        watch.writeLastExport(items[items.size()-1].blk);

    EXIT_NOMSG(true);
}
