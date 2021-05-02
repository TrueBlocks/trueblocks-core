/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::handle_counts(void) {
    for (auto cnt : counts) {
        if (shouldQuit())
            break;
        cout << ((isJson() && !firstOut) ? ", " : "");
        cout << cnt;
        firstOut = false;
    }
    return true;
}
