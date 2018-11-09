/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

bool filterForRemove(CAcctCacheItemArray& dataArray, const CAcctCacheItem& item);
//-----------------------------------------------------------------------
bool COptions::handleRemove(void) const {

    ASSERT(fileExists("./remove.txt"));
    ASSERT(removals.size());

    CAcctCacheItemArray dataArray;
    if (!handleRead("Reading", 1, dataArray))
        return false;

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    for (size_t i = 0 ; i < removals.size() ; i++)
        if (removals[i].blockNum > 0)
            cerr << "\tWill remove item " << removals[i] << " if found\n";

    if (!handleWrite(filenames[0], dataArray, filterForRemove))
        return false;

    return true;
}

//-----------------------------------------------------------------------
// Return true if we don't find it so we don't filter it (double negative)
bool filterForRemove(CAcctCacheItemArray& dataArray, const CAcctCacheItem& search) {
    CAcctCacheItemArray::iterator it = find(dataArray.begin(), dataArray.end(), search);
    return (it == dataArray.end());
}
