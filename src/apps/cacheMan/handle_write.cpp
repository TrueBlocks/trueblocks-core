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

class CWriteItem {
public:
    blknum_t bn;
    blknum_t tx;
    CWriteItem(blknum_t b, blknum_t t) : bn(b), tx(t) {}
};
typedef vector<CWriteItem> CWriteItemArray;
//-------------------------------------------------------------------------
bool COptions::handleWrite(const string_q& outputFilename, const CAcctCacheItemArray& dataArray, CACHEFILTERFUNC filterFunc) const {

    cerr << "\tWriting...";

    string_q contents;
    asciiFileToString("./cache/lastBlock.txt", contents);
    blknum_t currentLastItem = str_2_Uint(contents);

    CArchive txCache(WRITING_ARCHIVE);
    if (!txCache.Lock(outputFilename, binaryWriteCreate, LOCK_WAIT))
        return usage("Could not open merge file: " + outputFilename + ". Quitting.");

    // Now that we know we can write to the file, we can make a write array. We do this for two reasons: filtering
    // removals and removing duplicates. It's just easier that way.
    blknum_t newLastItem = currentLastItem;
    CWriteItemArray writeArray;
    for (uint32_t i = 0 ; i < dataArray.size() && !shouldQuit() ; i++) {
        // filterFunc (if present) returns true if we should include the record
        if (!filterFunc || (*filterFunc)(((COptions*)this)->removals, dataArray[i])) {
            if (i == 0 || dataArray[i-1] != dataArray[i]) {  // removes dups
                if (dataArray[i].blockNum > currentLastItem)  // update last item
                    newLastItem = dataArray[i].blockNum;
                writeArray.push_back(CWriteItem(dataArray[i].blockNum, dataArray[i].transIndex));
                cerr << (!(writeArray.size() % 5000) ? "." : "");
            }
        }
    }

    // Now write the entire array in a single write to the hard drive
    if (!shouldQuit()) {
        lockSection(true);
        txCache.Write(writeArray.data(), sizeof(CWriteItem), writeArray.size());
        if (!filterFunc)  // we only write the last block marker if we're not removing records
            writeLastBlock(newLastItem);
        lockSection(false);
        cerr << cYellow << writeArray.size() << cOff << " records written, ";
        cerr << cYellow << (dataArray.size() - writeArray.size()) << cOff << " records ignored.\n";
        cerr << "\tWritten to " << cTeal << outputFilename << cOff << "\n";
    } else {
        cerr << "\tRecords not written\n";
    }
    txCache.Release();

    return !shouldQuit();
}

