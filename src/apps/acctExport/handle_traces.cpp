/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool traces_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->nProcessed += trav->trans.traces.size();
    if (opt->freshen)
        return true;

    opt->markNeighbors(trav->trans);
    if (opt->articulate)
        opt->articulateAll(trav->trans);

    for (auto trace : trav->trans.traces) {
        bool isSuicide = trace.action.selfDestructed != "";
        bool isCreation = trace.result.newContract != "";

        CTrace copy = trace;
        if (!isSuicide) {
            if (!opt->factory) {
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy;
                opt->firstOut = false;
            }
        }

        if (isSuicide) {  // suicide
            copy.action.from = trace.action.selfDestructed;
            copy.action.to = trace.action.refundAddress;
            copy.action.callType = "suicide";
            copy.action.value = trace.action.balance;
            copy.traceAddress.push_back("s");
            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
            copy.action.input = "0x";
            if (!opt->factory) {
                cout << ((isJson() && !opt->firstOut) ? ", " : "");
                cout << copy;
                opt->firstOut = false;
            }
        }

        if (isCreation) {  // contract creation
            copy.action.from = "0x0";
            copy.action.to = trace.result.newContract;
            copy.action.callType = "creation";
            copy.action.value = trace.action.value;
            if (copy.traceAddress.size() == 0)
                copy.traceAddress.push_back("null");
            copy.traceAddress.push_back("s");
            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
            copy.action.input = trace.action.input;
            cout << ((isJson() && !opt->firstOut) ? ", " : "");
            cout << copy;
            opt->firstOut = false;
        }
        // if (!isTestMode() && isApiMode()) {
        //     qblocks::eLogger->setEndline('\r');
        //     LOG_INFO("\t\t\t\t\t\tGetting trace ", trav->trans.blockNumber, ".", trav->trans.transactionIndex,
        //     "-",
        //              trace.getValueByName("traceAddress"), string_q(50, ' '));
        //     qblocks::eLogger->setEndline('\n');
        // }
    }

    prog_Log(trav, data, trav->inCache ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool traces_Data(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    if (loadData(trav, data)) {
        if (!loadTraces(trav->trans, trav->app->blk, trav->app->txid, opt->cache_traces,
                        (opt->skip_ddos && excludeTrace(&trav->trans, opt->max_traces)))) {
            return false;
        }
    }
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool COptions::handle_traces(void) {
    CTraverser trav(this, cout, "traces");
    trav.displayFunc = traces_Display;
    trav.dataFunc = traces_Data;

    CTraverserArray traversers;
    traversers.push_back(trav);

    cache_txs = false;
    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}
