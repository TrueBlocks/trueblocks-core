/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::handle_appearances(void) {
    ENTER("handle_appearances");

    ASSERT(appearances);
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    bool first = true;
    for (size_t i = 0; i < apps.size(); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= ts_cnt)
            break;

        // LOG_TEST("passes", inRange((blknum_t)app->blk, exportRange.first, exportRange.second) ? "true" : "false");
        if (inRange((blknum_t)app->blk, exportRange.first, exportRange.second)) {
            nProcessed++;
            if (shouldDisplay) {
                CAppearanceDisplay dapp(hackAppAddr, app->blk, app->txid);
                cout << ((isJson() && !first) ? ", " : "");
                cout << dapp.Format() << endl;
                first = false;
            }
        } else if (app->blk > exportRange.second) {
            break;
        }
    }

    if (!isTestMode()) {
        LOG_PROGRESS((freshen ? "Updated" : "Reported"), (first_record + nProcessed), nTransactions,
                     " appearances for address " + allMonitors[0].address);
    }

    EXIT_NOMSG(true);
}
