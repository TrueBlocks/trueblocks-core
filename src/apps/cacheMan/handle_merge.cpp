/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleMerge(void) const {
    CAppearanceArray_base dataArray;
    if (!handleRead("Merging", monitors.size(), dataArray))
        return false;

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    if (!handleWrite("./merged.bin", dataArray, NULL))
        return false;

    return true;
}
