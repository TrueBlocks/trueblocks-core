/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportTraces(void) {
    ENTER("exportData");

    ASSERT(traces);
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;
    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);

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
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

            } else {
                if (item->blk == 0) {
                    address_t addr = prefundAddrMap[item->txid];
                    trans.transactionIndex = item->txid;
                    trans.loadTransAsPrefund(addr, prefundWeiMap[addr]);

                } else if (item->txid == 99997 || item->txid == 99999) {
                    trans.loadTransAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

                } else if (item->txid == 99998) {
                    uint64_t nUncles = getUncleCount(item->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, item->blk, u);
                        trans.loadTransAsUncleReward(item->blk, uncle.blockNumber);
                        trans.to = blkRewardMap[item->blk];
                    }

                } else {
                    getTransaction(trans, item->blk, item->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

                // ... we don't write the data here since it will not be complete.
                // if (false) // (write_opt & CACHE_TXS) && !fileExists(txFilename))
                //    writeTransToBinary(trans, txFilename);
            }

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
                    if (articulate)
                        abis.articulateTrace(&trace);
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

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportNeighbors();
    EXIT_NOMSG(true);
}
