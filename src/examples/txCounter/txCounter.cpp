#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

//-------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
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
        SFString contents = asciiFileToString("./data/countsByWeek.txt");
        SFString last;
        while (!contents.empty()) {
            SFString str = Strip(nextTokenClear(contents,'\n'),' ');
            if (!str.empty())
                last = str;
        }
//4305968 2017-09-24 00:00:17 UTC 1398084 2907884 0.675315    60868104    20.9321 14.1358 0
        SFString str;
        str = nextTokenClear(last,sep); startBlock = toUnsigned(str);
        nextTokenClear(last,sep);
        str = nextTokenClear(last,sep); nEmpty     = toUnsigned(str);
        str = nextTokenClear(last,sep); nFull      = toUnsigned(str);;
        str = nextTokenClear(last,sep);
        str = nextTokenClear(last,sep); nTrans     = toUnsigned(str);;
        str = nextTokenClear(last,sep);
        str = nextTokenClear(last,sep);
        str = nextTokenClear(last,sep); nTraces    = toUnsigned(str);;
    }
};

//-------------------------------------------------------------------------
int main(int argc, char *argv[])
{
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
        SFString fileName = getBinaryFilename(i);
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
    static SFTime last = earliestDate;
#ifdef SUBTOTAL_BY_FIVE_MINS
    SFTime thisOne = dateFromTimeStamp(block.timestamp);
    thisOne = SFTime(thisOne.GetYear(),thisOne.GetMonth(),thisOne.GetDay(),thisOne.GetHour(),thisOne.GetMinute()/5,0);
#else
    SFTime thisOne = SubtractOneDay(SubtractOneDay(BOW(dateFromTimeStamp(block.timestamp))));
#endif
#endif

    uint64_t tCount = block.transactions.getCount();
    nFull++;
    nEmpty = (block.blockNumber - nFull);
    nTrans += tCount;

    ostream *os = &cerr;
    sep = ' ';
    if (last != thisOne) {
        os = &cout;
        sep = '\t';
        cerr << SFString(' ',120) << "\r";
    }
    *os << block.blockNumber << sep
        << dateFromTimeStamp(block.timestamp) << sep
        << nEmpty << sep
        << nFull << sep
        << ((double)nFull/(double)(block.blockNumber)) << sep
        << nTrans << sep
        << ((double)nTrans/(double)(nFull)) << sep
        << ((double)nTrans/(double)(block.blockNumber)) << sep
        << nTraces;

    if (last != thisOne) {
        *os << "\n";
        last = thisOne;
    } else {
        *os << "\r";
    }
    os->flush();
}

