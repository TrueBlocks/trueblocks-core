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

    blknum_t firstBlock = apps.size() ? apps[0].blk : 0;
    blknum_t lastBlock = apps.size() ? apps[apps.size() - 1].blk : latestBlock;

    bool first = true;
    blknum_t lastExported = scanRange.second;
    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < freshen_max)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= ts_cnt) {
            lastExported = app->blk - 1;
            break;
        }
        if (inRange((blknum_t)app->blk, scanRange.first, scanRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = app->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, app->blk, app->txid);
            if (app->blk != 0 && fileExists(txFilename)) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)ts_array[(app->blk * 2) + 1];

            } else {
                if (app->blk == 0) {
                    address_t addr = prefundAddrMap[app->txid];
                    trans.loadTransAsPrefund(app->blk, app->txid, addr, prefundWeiMap[addr]);

                } else if (app->txid == 99997 || app->txid == 99999) {
                    trans.loadTransAsBlockReward(app->blk, app->txid, blkRewardMap[app->blk]);

                } else if (app->txid == 99998) {
                    uint64_t nUncles = getUncleCount(app->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, app->blk, u);
                        if (uncle.miner == blkRewardMap[app->blk]) {
                            trans.loadTransAsUncleReward(app->blk, uncle.blockNumber, uncle.miner);
                        }
                    }

                } else {
                    getTransaction(trans, app->blk, app->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)ts_array[(app->blk * 2) + 1];

                // ... we don't write the data here since it will not be complete.
                // if (false) // (write_opt & CACHE_TXS) && !fileExists(txFilename))
                //    writeTransToBinary(trans, txFilename);
            }

            loadTraces(trans, app->blk, app->txid, (write_opt & CACHE_TRACES),
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
                    nProcessed++;
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
                    nProcessed++;
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
                    nProcessed++;
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
                os << "Exported " << nProcessed << " ";
                os << plural(className) << " from ";
                os << nProcessing << " txs (blocks: ";
                os << firstBlock << "-[" << cGreen;
                os << app->blk << cOff << "]-";
                os << lastBlock << " ";
                os << "for address " << monitors[0].address;
                LOG_INFO(os.str() + "\r");
            }
        }
    }

    if (!isTestMode())
        LOG_PROGRESS1("Reported", (first_record + nProcessed), nTransactions,
                      " traces for address " + monitors[0].address + "\r");

    for (auto monitor : monitors)
        monitor.updateLastExport(lastExported);

    reportNeighbors();

    EXIT_NOMSG(true);
}
