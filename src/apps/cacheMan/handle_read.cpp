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
bool COptions::handleRead(const string_q& msg, size_t filesToUse, CAcctCacheItemArray& dataArray) const {

    dataArray.reserve(2000000); // just a guess, but makes the read very much faster
    for (uint32_t i = 0 ; i < filesToUse ; i++) {
        cerr << "\t" << msg << " cache: " << cTeal << filenames[i] << cOff << "...";
        CArchive txCache(READING_ARCHIVE);
        if (!txCache.Lock(filenames[i], binaryReadOnly, LOCK_NOWAIT))
            return usage("Could not open file: " + filenames[i] + ". Quitting.");
        uint64_t nRead = 0;
        while (!txCache.Eof() && !shouldQuit()) {
            CAcctCacheItem item;
            txCache >> item.blockNum >> item.transIndex;
            if (item.blockNum > 0) {
                dataArray.push_back(item);
                nRead++;
                cerr << (!(nRead % 10000) ? "." : "");
            }
        }
        txCache.Release();
        cerr << cYellow << nRead << cOff << " existing records read.\n";
    }
    return !shouldQuit();
}

