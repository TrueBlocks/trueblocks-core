/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleSort(void) const {

    CAcctCacheItemArray dataArray;
    if (!handleRead("Sorting", 1, dataArray))
        return false;

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    if (!handleWrite(filenames[0], dataArray, NULL))
        return false;

    return true;
}
