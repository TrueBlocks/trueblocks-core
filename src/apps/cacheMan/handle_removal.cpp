/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

bool filterForRemove(CAppearanceArray_base& dataArray, const CAppearance_base& item);
//-----------------------------------------------------------------------
bool COptions::handleRemove(void) const {
    ASSERT(fileExists("./remove.txt"));
    ASSERT(removals.size() == 1);

    CAppearanceArray_base dataArray;
    if (!handleRead("Reading", 1, dataArray))
        return false;

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    for (size_t i = 0; i < removals.size(); i++)
        if (removals[i].blk > 0)
            cerr << "\tWill remove item " << removals[i].blk << "." << removals[i].txid << " if found\n";

    if (!handleWrite(monitors[0].name, dataArray, filterForRemove))
        return false;

    return true;
}

//-----------------------------------------------------------------------
// Return true if we don't find it so we don't filter it (double negative)
bool filterForRemove(CAppearanceArray_base& dataArray, const CAppearance_base& search) {
    return (bsearch(&search, dataArray.data(), dataArray.size(), sizeof(CAppearance_base), findAppearance) == NULL);
}
