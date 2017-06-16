/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

// #define ADDR "0x33990122638b9132ca29c723bdf037f1a891a70c"
// #define ADDR "0xc78310231aa53bd3d0fea2f8c705c67730929d8f"
// #define ADDR "0xaec2e87e0a235266d9c5adc9deb4b2e29b54d009"
#define ADDR "0xbb9bc244d798123fde783fcc1c72d3bb8c189413"
#define N 20
#define BL 4000
// 500000
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        SFUint32 start = 1428757;  // 2364414; // end - 100000;
        SFUint32 end = start + BL;  // getClientLatestBlk();
        for (uint32_t bn = start ; bn < end ; bn = bn + N) {
            SFString res;
            queryRawLogs(bn, bn+N, ADDR, res);
            if (!res.empty())
                cout << res << "\n";
            cerr << bn << ": " << "\r";
            cerr.flush();
        }
    }
    return 0;
}
