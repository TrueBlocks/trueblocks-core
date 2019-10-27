/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleSort(void) const {

    ASSERT(filenmes.size() == 1);

    CAppearanceArray_base dataArray;
    if (!handleRead("Sorting", 1, dataArray))
        return false;

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    if (!handleWrite(monitors[0].name, dataArray, NULL))
        return false;

    return true;
}
