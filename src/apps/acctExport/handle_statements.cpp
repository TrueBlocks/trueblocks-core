/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool statements_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;
    opt->process_reconciliation(trav);

    LOG_INFO("I am here: ", trav->trans.statements.size());
    for (auto statement : trav->trans.statements) {
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << statement;
        opt->firstOut = false;
    }

    prog_Log(trav, data);
    return !shouldQuit();
}
