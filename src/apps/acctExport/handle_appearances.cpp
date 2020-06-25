/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::exportAppearances(void) {
    ENTER("exportAppearances");

    bool isJson =
        (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);

    bool first = true;
    for (size_t i = 0; i < items.size(); i++) {
        const CAppearance_base* item = &items[i];
        if (shouldQuit() || item->blk >= ts_cnt)
            break;

        if (inRange((blknum_t)item->blk, scanRange.first, scanRange.second)) {
            CDisplayApp app(hackAppAddr, item->blk, item->txid);
            cout << ((isJson && !first) ? ", " : "");
            cout << app.Format() << endl;
            first = false;
            nExported++;
        }
    }

    EXIT_NOMSG(true);
}
