//3809807
//2224943
//0x5901Deb2C898D5dBE5923E05e510E95968a35067
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

bool extractAddresses(CBlock& block, void *data);
SFAddress addr = "0x5901Deb2C898D5dBE5923E05e510E95968a35067";
SFUintBN myBloom;
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    myBloom = makeBloom(addr);
    forEveryBlockOnDisc(extractAddresses, (void*)NULL, 2224943, 3809807);
    return 0;
}

int cnt=0;
//--------------------------------------------------------------
bool extractAddresses(CBlock& block, void *data) {
    SFStringArray addrs;
    for (txnum_t tx = 0 ; tx < block.transactions.getCount() ; tx++) {
        const CTransaction *t = &block.transactions[tx];
        if (addr % t->from ||
            addr % t->to ||
            addr % t->receipt.contractAddress ||
            isBloomHit(myBloom, t->receipt.logsBloom)) {
            cout << block.blockNumber << ";" << t->transactionIndex << ";4\n";
        }
    }
    cerr << block.blockNumber << "\r";
    cerr.flush();

    return true;
}
