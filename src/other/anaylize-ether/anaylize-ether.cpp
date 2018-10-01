/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

double start = 0;
double end = 0;

CAddressArray addrs;
//----------------------------------------------------------------
int main(int argc, char *argv[])
{
    etherlib_init();

    string_q contents;
    asciiFileToString("genesis.txt", contents);
    uint64_t cnt = 0;
    while (!contents.empty()) {
        string_q line = nextTokenClear(contents, '\n');
        addrs.push_back(nextTokenClear(line, ','));
        if (cnt++ > 100)
            break;
    }

//    for (auto addr : addrs)
//        cout << addr << "\n";

//    forEveryFileInFolder(blockCachePath("traces/"), visitFile, NULL);
    start = qbNow();
    forEveryBlockOnDisc(visitBlock, NULL, 0, 5500000, 10000);
    return 1;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {
        cout << path << "\n";
    }
    return true;
}

//----------------------------------------------------------------
bool visitBlock(CBlock& block, void *data) {

    cerr << block.blockNumber << "\r";
    string_q str;
    wei_t bal = 0;
    for (auto addr : addrs) {
        wei_t b = getBalance(addr, block.blockNumber, false);
        cerr << "\t" << addr << "\t" << b << "                  \r";
        bal += b;
    }
    double diff = qbNow() - start;
    cout << block.blockNumber << "\t" << bal << "\t\t\t" << diff << "                                          \n";
    start = qbNow();
    return true;
}
