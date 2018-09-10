/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleMerge(void) const {

    CAcctCacheItemArray dataArray;
    if (!handleRead("Merging", filenames.size(), dataArray))
        return false;

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    if (!handleWrite("./merged.bin", dataArray, NULL))
        return false;

    return true;
}
