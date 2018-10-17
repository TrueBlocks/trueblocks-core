/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//----------------------------------------------------------------
int main(int argc, char *argv[])
{
    etherlib_init();
    forEveryFileInFolder(blockCachePath("traces/"), visitFile, NULL);
    forEveryBlockOnDisc(visitBlock, NULL, 0, getLatestBlockFromCache());
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

    cout << block << "\n";
    for (uint32_t tr = 0 ; tr < block.transactions.size() ; tr++) {
        cout << block.transactions[tr] << "\n";
    }
    return true;
}
