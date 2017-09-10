/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    // Tell the system where the blocks are and which version to use
    etherlib_init("binary");

    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    SFAddress addrs[] = { "", "0x", "0x0", "0x0000000000000000000000000000000000000000" };
    uint32_t nAddrs = sizeof(addrs) / sizeof(SFAddress);
    for (uint32_t i=0;i<nAddrs;i++) {
        cout << addrs[i] << " : " << addr2BN(addrs[i]) << " : " << sizeof(addrs[i]) << " : " << sizeof(addr2BN(addrs[i])) << "\n";
    }
    return 0;
}
