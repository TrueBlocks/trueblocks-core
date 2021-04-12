/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool logs_Pre(CTraverser* trav, void* data);
extern bool logs_Display(CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_logs(void) {
    CTraverser trav(this, cout, "logs");
    trav.preFunc = logs_Pre;
    trav.displayFunc = logs_Display;

    CTraverserArray traversers;
    traversers.push_back(trav);

    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool logs_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->nProcessed += trav->trans.receipt.logs.size();
    if (opt->freshen)
        return true;

    opt->markNeighbors(trav->trans);
    opt->articulateAll(trav->trans);
    for (auto log : trav->trans.receipt.logs) {
        bool showMe = true;
        if (opt->relevant) {
            showMe = opt->isRelevant(log);
            if (showMe && !opt->emitted_by.empty())
                showMe = opt->wasEmittedBy(log.address);
        } else if (!opt->emitted_by.empty()) {
            showMe = opt->wasEmittedBy(log.address);
        } else if (opt->emitter) {
            showMe = opt->isEmitter(log.address);
        }
        if (showMe) {
            cout << ((isJson() && !opt->firstOut) ? ", " : "");
            cout << log.Format() << endl;
            opt->firstOut = false;
        }
    }

    prog_Log(trav, data, trav->inCache ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool logs_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    if (opt->apps.size() > 0 && opt->first_record != 0)
        opt->lastStatement.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);

    start_Log(trav, data);
    return true;
}
