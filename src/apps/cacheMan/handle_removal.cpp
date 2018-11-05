/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
