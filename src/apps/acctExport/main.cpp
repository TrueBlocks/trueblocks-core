#define MAX_TXS NOPOS
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportData(void) {

    ENTER("exportData");
    if (exportFmt == JSON1)
        cout << "[";

    bool first = true;
    size_t nExported = 0;
    for (size_t i = 0 ; i < items.size() && !shouldQuit() && i < MAX_TXS && items[i].blk < ts_cnt ; i++) {
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
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[item->blk];

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
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[item->blk];
                if (writeTrxs && !fileExists(txFilename))
                    writeTransToBinary(trans, txFilename);
            }

            if (doTraces) {

                loadTraces(trans, item->blk, item->txid, true, (skipDdos && excludeTrace(&trans, maxTraces)));
                for (auto trace : trans.traces) {

                    bool isSuicide = trace.action.address != "";
                    bool isCreation = trace.result.address != "";

                    if (!isSuicide) {
                        if (exportFmt == JSON1 && !first)
                            cout << ", ";
                        if (articulate)
                            abis.articulateTrace(&trace);
                        cout << trace.Format() << endl;
                        first = false;
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
                        if (exportFmt == JSON1 && !first)
                            cout << ", ";
                        cout << copy.Format() << endl;
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
                        if (exportFmt == JSON1 && !first)
                            cout << ", ";
                        cout << copy.Format() << endl;
                    }
                }

            } else {
                if (doLogs) {
                    for (auto log : trans.receipt.logs) {
                        if (exportFmt == JSON1 && !first)
                            cout << ", ";
                        if (articulate)
                            abis.articulateLog(&log);
                        cout << log.Format() << endl;
                        first = false;
                    }
                } else {
                    if (exportFmt == JSON1 && !first) {
                        cout << ", ";
                        // we only articulate the transaction if we're JSON
                        if (articulate)
                            abis.articulateTransaction(&trans);
                    }
                    cout << trans.Format() << endl;
                    first = false;
                }
            }
            nExported++;
            HIDE_FIELD(CFunction, "message");
            if (isRedirected()) {  // we are not in --output mode
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
#if 0
/*
Do we really need to store both trace 0 and the transaction? I think not, so we store only trace 1 and on
writeNodeToBinary but do so as an array and start at item 1 not item 0
We need to handle the non-txt case where we're exporting the fully articulated transaction, all it's traces, all its data
We want to name everything we can name
What about the header in trace mode -- different for each trace, trans, receipt, log
*/
#endif

    cerr << "Exported " << nExported << " of " << items.size() << " records." << string_q(45,' ') << "\n";
    cerr.flush();
    if (exportFmt == JSON1)
        cout << "]";

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (options.loadData()) {
            if (once)
                cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CLogEntry));
            options.exportData();
            once = false;
        }
    }
    cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

    acctlib_cleanup();
    return 0;
}
