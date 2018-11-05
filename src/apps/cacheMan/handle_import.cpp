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

//-------------------------------------------------------------------------
bool COptions::handleImport(void) const {

    ASSERT(fileExists("./import.txt"));

    CAcctCacheItemArray dataArray;
    if (!handleRead("Reading", fileExists(filenames[0]), dataArray))
        return false;

    CStringArray lines;
    string_q contents;
    asciiFileToString("./import.txt", contents);
    size_t nRecords = explode(lines, contents, '\n');
    for (auto line : lines) {
        CAcctCacheItem item(line);
        if (item.blockNum > 0) {
            dataArray.push_back(item);
            if (!(dataArray.size() % 13)) {
                cerr << "\tImporting record " << dataArray.size() << " of " << nRecords << "\r";
                cerr.flush();
            }
        }
    }
    cerr << "\n";
    if (!isTestMode()) {
        copyFile("./import.txt", "import.bak");
        remove("./import.txt");
    }

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    if (!handleWrite(filenames[0], dataArray, NULL))
        return false;

    return true;
}

