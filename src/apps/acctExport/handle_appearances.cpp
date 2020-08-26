/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::hanlde_appearances(void) {
    ENTER("hanlde_appearances");

    ASSERT(appearances);
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    bool first = true;
    for (size_t i = 0; i < items.size(); i++) {
        const CAppearance_base* item = &items[i];
        if (shouldQuit() || item->blk >= ts_cnt)
            break;
        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            nExported++;
            if (shouldDisplay) {
                CDisplayApp app(hackAppAddr, item->blk, item->txid);
                cout << ((isJson() && !first) ? ", " : "");
                cout << app.Format() << endl;
                first = false;
            }
        }
    }

    LOG_PROGRESS1("Reported", (first_record + nExported), nTransactions,
                  " appearances for address " + monitors[0].address + "\r");

    EXIT_NOMSG(true);
}
