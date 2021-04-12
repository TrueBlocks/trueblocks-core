/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool acct_Pre(CTraverser* trav, void* data);
extern bool acct_Display(CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_accounting(void) {
    CTraverser trav(this, cout, "txs");
    trav.preFunc = acct_Pre;
    trav.displayFunc = acct_Display;
    trav.dataFunc = loadData;

    CTraverserArray traversers;
    traversers.push_back(trav);

    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->nProcessed++;
    if (!opt->freshen) {
        opt->markNeighbors(trav->trans1);
        opt->articulateAll(trav->trans1);
        if (opt->accounting) {
            blknum_t next = trav->index < opt->apps.size() - 1 ? opt->apps[trav->index + 1].blk : NOPOS;
            process_reconciliation(opt, trav->trans1, opt->prevStatements, next, opt->tokens);
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans1.Format() << endl;
        opt->firstOut = false;
    }

    prog_Log(trav, data, trav->inCache1 ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool acct_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    if (opt->apps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->apps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}
