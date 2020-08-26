/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::handle_counts(void) {
    ENTER("handle_counts");

    bool first = true;
    for (auto cnt : counts) {
        if (shouldQuit())
            break;
        cout << ((isJson() && !first) ? ", " : "");
        cout << cnt.Format() << endl;
        first = false;
    }

    EXIT_NOMSG(true);
}
