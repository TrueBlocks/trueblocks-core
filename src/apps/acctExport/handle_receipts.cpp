/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool receipts_Pre(CTraverser* trav, void* data);
extern bool receipts_Display(CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_receipts(void) {
    CTraverser trav(this, cout, "receipts");
    trav.preFunc = receipts_Pre;
    trav.displayFunc = receipts_Display;
    trav.dataFunc = loadData;

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
    if (!opt->freshen) {
        opt->markNeighbors(trav->trans1);
        opt->articulateAll(trav->trans1);
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans1.receipt.Format() << endl;
        opt->firstOut = false;
    }

    prog_Log(trav, data, trav->inCache1 ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool receipts_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;
    opt->firstOut = true;

    SHOW_FIELD(CReceipt, "blockNumber");
    SHOW_FIELD(CReceipt, "transactionIndex");
    SHOW_FIELD(CReceipt, "isError");

    start_Log(trav, data);
    return true;
}
