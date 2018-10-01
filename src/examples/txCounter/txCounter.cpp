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
#include <iostream>
#include <iomanip>
#include "etherlib.h"

//-------------------------------------------------------------------------
char sep = '\t';

//-------------------------------------------------------------------------
class CCounter {
public:
    uint64_t startBlock;
    uint64_t nFull;
    uint64_t nEmpty;
    uint64_t nTrans;
    uint64_t nTraces;
    CCounter(void) :
        startBlock(0), nFull(0), nEmpty(0), nTrans(0), nTraces(0)
        {}
    void countOne(const CBlock& block);
    void loadFromFile(void) {
        string_q contents;
        asciiFileToString("./data/countsByWeek.txt", contents);
        string_q last;
        while (!contents.empty()) {
            string_q str = trim(nextTokenClear(contents, '\n'));
            if (!str.empty())
                last = str;
        }
// 4305968 2017-09-24 00:00:17 UTC 1398084 2907884 0.675315    60868104    20.9321 14.1358 0
        string_q str;
        str = nextTokenClear(last, sep); startBlock = str_2_Uint(str);
              nextTokenClear(last, sep);
        str = nextTokenClear(last, sep); nEmpty     = str_2_Uint(str);
        str = nextTokenClear(last, sep); nFull      = str_2_Uint(str);
        str = nextTokenClear(last, sep);
        str = nextTokenClear(last, sep); nTrans     = str_2_Uint(str);
        str = nextTokenClear(last, sep);
        str = nextTokenClear(last, sep);
        str = nextTokenClear(last, sep); nTraces    = str_2_Uint(str);
    }
};

//-------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    etherlib_init();

    cout << "blockNum" << sep
        << "date" << sep
        << "nEmpty" << sep
        << "nFull" << sep
        << "pctFull" << sep
        << "nTrans" << sep
        << "tx/full blk" << sep
        << "tx/all blks" << sep
        << "nTraces" << "\n";

    CCounter counter;
    counter.loadFromFile();
    blknum_t latest = getLatestBlockFromCache();
    for (blknum_t i = counter.startBlock-1 ; i < latest ; i++) {
        string_q fileName = getBinaryFilename(i);
        if (fileExists(fileName)) {
            CBlock block;
            readFromBinary(block, fileName);
            counter.countOne(block);
        } else {
            if (verbose) {
                cerr << "scanning: " << fileName << "\r";
                cerr.flush();
            }
        }
    }
    return 0;
}

//-------------------------------------------------------------------------
void CCounter::countOne(const CBlock &block) {

#ifdef SUBTOTAL_EVERY_X_BLOCKS
    static blknum_t last = startBlock;
    blknum_t thisOne = (block.blockNumber / 10000) * 10000;
#else
    static time_q last = earliestDate;
#ifdef SUBTOTAL_BY_FIVE_MINS
    time_q thisOne = ts_2_Date(block.timestamp);
    thisOne = time_q(thisOne.GetYear(),
                        thisOne.GetMonth(),
                        thisOne.GetDay(),
                        thisOne.GetHour(),
                        thisOne.GetMinute()/5,
                        0);
#else
    time_q thisOne = SubtractOneDay(SubtractOneDay(BOW(ts_2_Date(block.timestamp))));
#endif
#endif

    uint64_t tCount = block.transactions.size();
    nFull++;
    nEmpty = (block.blockNumber - nFull);
    nTrans += tCount;

    ostream *os = &cerr;
    sep = ' ';
    if (last != thisOne) {
        os = &cout;
        sep = '\t';
        cerr << string_q(120, ' ') << "\r";
    }
    *os << block.blockNumber << sep
        << ts_2_Date(block.timestamp) << sep
        << nEmpty << sep
        << nFull << sep
        << ((double)nFull/(double)(block.blockNumber)) << sep  // NOLINT
        << nTrans << sep
        << ((double)nTrans/(double)(nFull)) << sep  // NOLINT
        << ((double)nTrans/(double)(block.blockNumber)) << sep  // NOLINT
        << nTraces;

    if (last != thisOne) {
        *os << "\n";
        last = thisOne;
    } else {
        *os << "\r";
    }
    os->flush();
}

