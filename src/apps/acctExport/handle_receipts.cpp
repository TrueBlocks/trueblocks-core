/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool receipts_Display(CTraverser* trav, void* data1) {
    COptions* opt = (COptions*)data1;

    trav->nProcessed++;
    if (opt->freshenOnly)
        return true;

    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << trav->trans.receipt;
    opt->firstOut = false;

    prog_Log(trav, data1);
    return !shouldQuit();
}
