/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (fourByteFilter(trav->trans.input, opt)) {
        if (opt->accounting) {
            blknum_t next = trav->index < opt->apps.size() - 1 ? opt->apps[trav->index + 1].blk : NOPOS;
            process_reconciliation(opt, trav->trans, opt->prevStatements, next, opt->tokens);
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool acct_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (opt->apps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->apps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}
