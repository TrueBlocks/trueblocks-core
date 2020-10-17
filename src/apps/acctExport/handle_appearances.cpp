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
    for (size_t i = 0; i < apps.size(); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= ts_cnt)
            break;
        if (inRange((blknum_t)app->blk, scanRange.first, scanRange.second)) {
            nProcessed++;
            if (shouldDisplay) {
                CAppearanceDisplay dapp(hackAppAddr, app->blk, app->txid);
                cout << ((isJson() && !first) ? ", " : "");
                cout << dapp.Format() << endl;
                first = false;
            }
        }
    }

    if (!isTestMode())
        LOG_PROGRESS1("Reported", (first_record + nProcessed), nTransactions,
                      " appearances for address " + monitors[0].address + "\r");

    EXIT_NOMSG(true);
}
