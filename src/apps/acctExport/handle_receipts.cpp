/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool receipts_Display(CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_receipts(void) {
    CTraverser trav(this, cout, "receipts");
    trav.displayFunc = receipts_Display;

    CTraverserArray traversers;
    traversers.push_back(trav);

    cache_txs = false;  // no need
    forEveryAppearance(traversers, apps, nullptr);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool receipts_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->nProcessed++;
    if (opt->freshen)
        return true;

    opt->markNeighbors(trav->trans);
    opt->articulateAll(trav->trans);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << trav->trans.receipt.Format() << endl;
    opt->firstOut = false;

    prog_Log(trav, data, trav->inCache ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}
