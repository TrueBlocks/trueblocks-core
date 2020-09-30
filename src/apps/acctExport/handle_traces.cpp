/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::handle_traces(void) {
    ENTER("handle_traces");

    ASSERT(traces);
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    blknum_t firstBlock = items.size() ? items[0].blk : 0;
    blknum_t lastBlock = items.size() ? items[items.size() - 1].blk : latestBlock;

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
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(item->blk * 2) + 1];

            } else {
                if (item->blk == 0) {
                    address_t addr = prefundAddrMap[item->txid];
                    trans.loadTransAsPrefund(item->blk, item->txid, addr, prefundWeiMap[addr]);

                } else if (item->txid == 99997 || item->txid == 99999) {
                    trans.loadTransAsBlockReward(item->blk, item->txid, blkRewardMap[item->blk]);

                } else if (item->txid == 99998) {
                    uint64_t nUncles = getUncleCount(item->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, item->blk, u);
                        if (uncle.miner == blkRewardMap[item->blk]) {
                            trans.loadTransAsUncleReward(item->blk, uncle.blockNumber, uncle.miner);
                        }
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
                    nExported++;
                    if (shouldDisplay && !factory) {
                        cout << ((isJson() && !first) ? ", " : "");
                        cout << trace.Format() << endl;
                        first = false;
                    }
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
                    nExported++;
                    if (shouldDisplay && !factory) {
                        cout << ((isJson() && !first) ? ", " : "");
                        cout << copy.Format() << endl;
                        first = false;
                    }
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
                    nExported++;
                    if (shouldDisplay && !factory) {
                        cout << ((isJson() && !first) ? ", " : "");
                        cout << copy.Format() << endl;
                        first = false;
                    } else if (factory) {
                        cout << ((isJson() && !first) ? ", " : "");
                        cout << copy.Format() << endl;
                    }
                }
            }

            HIDE_FIELD(CFunction, "message");
            if (!isTestMode() && (isApiMode() || !(i % 3))) {
                ostringstream os;
                os << "Exported " << nExported << " ";
                os << plural(className) << " from ";
                os << nProcessing << " txs (blocks: ";
                os << firstBlock << "-[" << cGreen;
                os << item->blk << cOff << "]-";
                os << lastBlock << " ";
                os << "for address " << monitors[0].address;
                LOG_INFO(os.str() + "\r");
            }
        }
    }

    if (!isTestMode())
        LOG_PROGRESS1("Reported", (first_record + nExported), nTransactions,
                      " traces for address " + monitors[0].address + "\r");

    for (auto monitor : monitors)
        if (items.size() > 0)
            monitor.writeLastExport(items[items.size() - 1].blk);

    reportNeighbors();

    EXIT_NOMSG(true);
}
