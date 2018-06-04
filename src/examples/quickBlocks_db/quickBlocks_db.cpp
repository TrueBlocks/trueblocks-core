/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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

#define NBLKS 100
#define FIRST 3456789

extern double testBuild(void);
extern double testRetrieve(void);
//--------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    double buildTime = testBuild();
    double retrieveTime = testRetrieve();

    cout << "Time to build:    " << buildTime    << " seconds\n";
    cout << "Time to retrieve: " << retrieveTime << " seconds\n";

    return 0;
}

//--------------------------------------------------------------------------
double testBuild(void) {
    double start = qbNow();
    establishFolder("./cache/");
    for (blknum_t b = FIRST ; b < FIRST+NBLKS ; b++) {
        SFString fileName = "./cache/" + asStringU(b) + ".bin";
        CBlock block;
        getBlock(block, b);
        if (!fileExists(fileName)) {
            block.finalized = isFinal(block.timestamp);
            writeToBinary(block, fileName);
        }
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
        SFString fileName = "./cache/" + asStringU(b) + ".bin";
        CBlock block;
        readFromBinary(block, fileName);
        cerr << "Reading " << block.blockNumber << "\r";
        cerr.flush();
    }
    cerr << SFString(' ',80) << "\r";
    return qbNow() - start;
}
