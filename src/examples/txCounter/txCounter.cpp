/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
    CCounter(void) : startBlock(0), nFull(0), nEmpty(0), nTrans(0), nTraces(0) {
    }
    void countOne(const CBlock& block);
    void loadFromFile(void) {
        string_q contents;
        asciiFileToString("./data/countsByWeek.txt", contents);
        if (contents.empty()) {
            cerr << "Error: /data/countsByWeek.txt contents is empty" << '\n';
            return;
        }
        CStringArray lines;
        explode(lines, contents, '\n');

        // 4305968 2017-09-24 00:00:17 UTC 1398084 2907884 0.675315    60868104    20.9321 14.1358 0
        //    0             1                 2      3        4           5           6       7    8
        CStringArray fields;
        explode(fields, lines[lines.size() - 1], sep);
        startBlock = str_2_Uint(fields[0]);
        nEmpty = str_2_Uint(fields[2]);
        nFull = str_2_Uint(fields[3]);
        nTrans = str_2_Uint(fields[5]);
        nTraces = str_2_Uint(fields[8]);
    }
};

//-------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    loadEnvironmentPaths();

    etherlib_init(quickQuitHandler);

    cout << "blockNum" << sep << "date" << sep << "nEmpty" << sep << "nFull" << sep << "pctFull" << sep << "nTrans"
         << sep << "tx/full blk" << sep << "tx/all blks" << sep << "nTraces"
         << "\n";

    CCounter counter;
    counter.loadFromFile();
    blknum_t latest = getLatestBlock_client();
    for (blknum_t i = counter.startBlock - 1; i < latest; i++) {
        string_q fileName = getBinaryCacheFilename(CT_BLOCKS, i);
        if (fileExists(fileName)) {
            CBlock block;
            readBlockFromBinary(block, fileName);
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
void CCounter::countOne(const CBlock& block) {
#ifdef SUBTOTAL_EVERY_X_BLOCKS
    thread_local blknum_t last = startBlock;
    blknum_t thisOne = (block.blockNumber / 10000) * 10000;
#else
    thread_local time_q last = earliestDate;
#ifdef SUBTOTAL_BY_FIVE_MINS
    time_q thisOne = ts_2_Date(block.timestamp);
    thisOne =
        time_q(thisOne.GetYear(), thisOne.GetMonth(), thisOne.GetDay(), thisOne.GetHour(), thisOne.GetMinute() / 5, 0);
#else
    time_q thisOne = SubtractOneDay(SubtractOneDay(BOW(ts_2_Date(block.timestamp))));
#endif
#endif

    uint64_t tCount = block.transactions.size();
    nFull++;
    nEmpty = (block.blockNumber - nFull);
    nTrans += tCount;

    ostream* os = &cerr;
    sep = ' ';
    if (last != thisOne) {
        os = &cout;
        sep = '\t';
        cerr << string_q(120, ' ') << "\r";
    }
    *os << block.blockNumber << sep << ts_2_Date(block.timestamp) << sep << nEmpty << sep << nFull << sep
        << ((double)nFull / (double)(block.blockNumber)) << sep        // NOLINT
        << nTrans << sep << ((double)nTrans / (double)(nFull)) << sep  // NOLINT
        << ((double)nTrans / (double)(block.blockNumber)) << sep       // NOLINT
        << nTraces;

    if (last != thisOne) {
        *os << "\n";
        last = thisOne;
    } else {
        *os << "\r";
    }
    os->flush();
}
