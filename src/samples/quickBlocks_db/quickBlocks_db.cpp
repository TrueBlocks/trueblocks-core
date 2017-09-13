/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#define NBLKS 100
#define FIRST 3456789

extern double testBuild(void);
extern double testRetrieve(void);
//--------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("binary");

    cout << "Time to build:    " << testBuild()    << " seconds\n";
    cout << "Time to retreive: " << testRetrieve() << " seconds\n";

    return 0;
}

//--------------------------------------------------------------------------
double testBuild(void) {
    double start = qbNow();
    establishFolder("./cache/");
    for (blknum_t b = FIRST ; b < FIRST+NBLKS ; b++) {
        SFString fileName = "./cache/" + asString(b) + ".bin";
        CBlock block;
        getBlock(block, b);
        if (!fileExists(fileName))
            writeToBinary(block, fileName);
        cerr << "Writing " << block.blockNumber << "\r";
        cerr.flush();
    }
    cerr << SFString(' ',80) << "\r";
    return qbNow() - start;
}

//--------------------------------------------------------------------------
double testRetrieve(void) {
    double start = qbNow();
    for (blknum_t b = FIRST ; b < FIRST+NBLKS ; b++) {
        SFString fileName = "./cache/" + asString(b) + ".bin";
        CBlock block;
        readOneBlock_fromBinary(block, fileName);
        cerr << "Reading " << block.blockNumber << "\r";
        cerr.flush();
    }
    cerr << SFString(' ',80) << "\r";
    return qbNow() - start;
}
