/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#if 0
#define START_BLOCK 4030000
#define N_EMPTY 1396479
#define N_FULL 2633521
#define N_TRANS 38086242
#define N_TRACES 0
#else
#define START_BLOCK 1
#define N_FULL 0
#define N_EMPTY 0
#define N_TRANS 0
#define N_TRACES 0
#endif

//-------------------------------------------------------------------------
class CCounter {
public:
    uint64_t nFull;
    uint64_t nEmpty;
    uint64_t nTrans;
    uint64_t nTraces;
    CCounter(void) : nFull(N_FULL), nEmpty(N_EMPTY), nTrans(N_TRANS), nTraces(N_TRACES) {}
    void countOne(const CBlock& block);
};

#include <iostream>
#include <iomanip>
#define sep '|'
//-------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    etherlib_init("binary");

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
    blknum_t latest = getLatestBlockFromClient();
    for (blknum_t i = START_BLOCK - 1 ; i < latest ; i++) {
        SFString fileName = getBinaryFilename1(i);
        if (fileExists(fileName)) {
            CBlock block;
            readOneBlock_fromBinary(block, fileName);
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
    uint64_t tCount = block.transactions.getCount();
    nFull++;
    nEmpty = (block.blockNumber - nFull);
    nTrans += tCount;

#ifdef SUBTOTAL_EVERY_X_BLOCKS
    static blknum_t last = START_BLOCK;
    blknum_t thisOne = (block.blockNumber / 10000) * 10000;
#else
    static SFTime last = earliestDate;
    SFTime thisOne = SubtractOneDay(SubtractOneDay(BOW(dateFromTimeStamp(block.timestamp))));
#endif

    ostream *os = &cerr;
    if (last != thisOne)
        os = &cout;
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
        *os << "          \n";
        last = thisOne;
    } else {
        *os << "\r";
    }
    os->flush();
}

