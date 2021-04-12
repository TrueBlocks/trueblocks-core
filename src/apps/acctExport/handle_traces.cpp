/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool traces_Pre(const CTraverser* trav, void* data);
extern bool traces_Display(const CTraverser* trav, void* data);
extern bool traces_Post(const CTraverser* trav, void* data);
extern void traces_Log(const CTraverser* trav, void* data, TraverserLog mode);
//-----------------------------------------------------------------------
bool COptions::handle_traces(void) {
    CTraverser trav(this, cout, "traces");
    traces_Pre(&trav, nullptr);

    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < 5000)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= expContext().tsCnt)
            break;
        if (inRange(blknum_t(app->blk), scanRange.first, scanRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = app->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, app->blk, app->txid);
            bool inCache = app->blk != 0 && fileExists(txFilename);
            if (inCache) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

            } else {
                if (app->blk == 0) {
                    address_t addr = prefundAddrMap[app->txid];
                    trans.loadTransAsPrefund(app->blk, app->txid, addr, expContext().prefundMap[addr]);

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
                trans.timestamp = block.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                // ... we don't write the data here since it will not be complete.
                // if (false) // (cache_txs && !fileExists(txFilename))
                //    writeTransToBinary(trans, txFilename);
            }

            loadTraces(trans, app->blk, app->txid, cache_traces, (skip_ddos && excludeTrace(&trans, max_traces)));

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
                        abi_spec.articulateTrace(&trace);
                    nProcessed++;
                    if (!freshen && !factory) {
                        cout << ((isJson() && !firstOut) ? ", " : "");
                        cout << trace.Format() << endl;
                        firstOut = false;
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
                    if (!freshen && !factory) {
                        cout << ((isJson() && !firstOut) ? ", " : "");
                        cout << copy.Format() << endl;
                        firstOut = false;
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
                    if (!freshen && !factory) {
                        cout << ((isJson() && !firstOut) ? ", " : "");
                        cout << copy.Format() << endl;
                        firstOut = false;
                    } else if (factory) {
                        cout << ((isJson() && !firstOut) ? ", " : "");
                        cout << copy.Format() << endl;
                        firstOut = false;
                    }
                }
            }

            if (!isTestMode() && (isApiMode() || !(i % 3))) {
                traces_Log(&trav, nullptr, TR_END);
            }

        } else if (app->blk > scanRange.second) {
            break;
        }
    }

    traces_Post(&trav, nullptr);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool traces_Pre(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    opt->firstBlock = opt->apps.size() ? opt->apps[0].blk : 0;
    opt->lastBlock = opt->apps.size() ? opt->apps[opt->apps.size() - 1].blk : opt->latestBlock;

    return true;
}

//-----------------------------------------------------------------------
bool traces_Post(const CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    if (trav->lastExpBlock != NOPOS)
        for (auto monitor : opt->allMonitors)
            monitor.writeLastExport(trav->lastExpBlock);

    opt->reportNeighbors();

    if (!isTestMode())
        traces_Log(trav, data, TR_END);

    return true;
}

//-----------------------------------------------------------------------
void traces_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (mode == TR_END) {
        end_Log(trav, data, mode);

    } else if (mode == TR_PROGRESS) {
        prog_Log(trav, data, mode);
    }
}
