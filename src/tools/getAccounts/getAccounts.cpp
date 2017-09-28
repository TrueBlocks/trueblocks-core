/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

   // Tell the system where the blocks are and which version to use
    etherlib_init("fastest");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        SFAddressArray addrs;
        getAccounts(addrs);

        for (uint32_t i = 0 ; i < addrs.getCount() ; i++)
            cout << addrs[i] << "\n";
    }
    return 0;
}
