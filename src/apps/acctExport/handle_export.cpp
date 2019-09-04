/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportData(void) {

    ENTER("exportData");

    if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !freshenOnly)
        cout << "[";

    bool first = true;
    for (size_t i = 0 ; i < items.size() && !shouldQuit() && items[i].blk < ts_cnt ; i++) {

        const CAppearance_base *item = &items[i];
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            if (doAppearances) {
                if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !first && !freshenOnly)
                    cout << ", ";
                if (!freshenOnly) {
                    CDisplayApp d(hackAppAddr, item->blk, item->txid);
                    cout << d.Format() << endl;
                    nExported++;
                }
                first = false;

            } else {

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

                    } else if (item->txid == 99997 || item->txid == 99998 || item->txid == 99999) {
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

                    // acctExport --traces
                    loadTraces(trans, item->blk, item->txid, writeTraces, (skipDdos && excludeTrace(&trans, maxTraces)));
                    for (auto trace : trans.traces) {

                        bool isSuicide = trace.action.address != "";
                        bool isCreation = trace.result.address != "";

                        if (!isSuicide) {
                            if (doABIs) {
                                abiMap[trace.action.to] = true;
                            } else {
                                if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !first && !freshenOnly)
                                    cout << ", ";
                                if (articulate)
                                    abis.articulateTrace(&trace);
                                if (!freshenOnly) {
                                    cout << trace.Format() << endl;
                                    nExported++;
                                }
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
                            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                            copy.action.input = "0x";
                            if (doABIs) {
                                abiMap[trace.action.to] = true;
                            } else {
                                if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !first && !freshenOnly)
                                    cout << ", ";
                                if (!freshenOnly) {
                                    nExported++;
                                    cout << copy.Format() << endl;
                                }
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
                            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
                            copy.action.input = trace.action.input;
                            if (doABIs) {
                                abiMap[trace.action.to] = true;

                            } else {
                                if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !first && !freshenOnly)
                                    cout << ", ";
                                if (!freshenOnly) {
                                    nExported++;
                                    cout << copy.Format() << endl;
                                }
                            }
                        }
                    }

                } else {

                    if (doLogs) {

                        // acctExport --logs
                        for (auto log : trans.receipt.logs) {
                            if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !first && !freshenOnly)
                                cout << ", ";
                            if (articulate)
                                abis.articulateLog(&log);
                            if (!freshenOnly) {
                                cout << log.Format() << endl;
                                nExported++;
                            }
                            first = false;
                        }

                    } else {

                        // acctExport (no options)
                        if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !first) {
                            if (!freshenOnly)
                                cout << ", ";
                            // we only articulate the transaction if we're JSON
                            if (articulate)
                                abis.articulateTransaction(&trans);
                        }
                        if (!freshenOnly) {
                            nExported++;
                            cout << trans.Format() << endl;
                        }
                        first = false;
                    }
                }

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
    }

    if (doABIs) {
        // acctExport --grabABIs (downloads and writes the ABIs for all the traces to disc)
        for (pair<address_t,bool> item : abiMap) {
            if (isContractAt(item.first)) {
                CAbi unused;
                loadAbiAndCache(unused, item.first, false, true, false);
                cout << "ABI for " << item.first << " ";
                cout << (fileExists(getCachePath("abis/" + item.first + ".json")) ? "cached" : "not cached") << endl;
            }
        }
    }

    if ((exportFmt == JSON1 || exportFmt == API1 || exportFmt == NONE1) && !freshenOnly)
        cout << "]";
    if (isTestMode())
        cout << endl;

    for (auto watch : monitors)
        watch.writeLastExport(items[items.size()-1].blk);

    EXIT_NOMSG(true);
}
