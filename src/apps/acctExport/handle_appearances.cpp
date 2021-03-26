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

    manageFields("CAccountName:all", false);
    manageFields("CAccountName:address,name", true);
    if (verbose) {
        SHOW_FIELD(CAppearanceDisplay, "timestamp");
        SHOW_FIELD(CAppearanceDisplay, "date");
    }

    bool shouldDisplay = !freshen;

    bool first = true;
    for (size_t i = 0; i < apps.size(); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= expContext().tsCnt)
            break;

        if (inRange((blknum_t)app->blk, exportRange.first, exportRange.second)) {
            nProcessed++;
            if (shouldDisplay) {
                CAppearanceDisplay dapp(hackAppAddr, hackAppName, app->blk, app->txid);
                cout << ((isJson() && !first) ? ", " : "");
                cout << dapp.Format() << endl;
                first = false;
            }
        } else if (app->blk > exportRange.second) {
            break;
        }
    }

    if (!isTestMode()) {
        if ((first_record + nProcessed) == nTransactions)
            LOG_PROGRESS((freshen ? "Finished updating" : "Finished reporting on"), (first_record + nProcessed),
                         nTransactions, " appearances for address " + allMonitors[0].address);
    }

    manageFields("CAccountName:all", true);
    manageFields(
        "CAccountName:deleted,showing,cname,schema,nAppearances,lastExport,firstAppearance,latestAppearance,path,"
        "sizeInBytes",
        false);
    EXIT_NOMSG(true);
}
