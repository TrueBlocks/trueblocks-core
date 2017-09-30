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
        if (options.named) {
            for (uint32_t i = 0 ; i < options.namedAccounts.getCount() ; i++)
                addrs[addrs.getCount()] = options.namedAccounts[i].addr;

        } else {
            getAccounts(addrs);
            if (isTestMode()) {
               addrs.Clear();
               addrs[addrs.getCount()] = "0x0000000000000000000000000000000000000000";
               addrs[addrs.getCount()] = "0x0000000000000000000000000000000000000001";
               addrs[addrs.getCount()] = "0x0000000000000000000000000000000000000002";
               addrs[addrs.getCount()] = "0x0000000000000000000000000000000000000003";
            }
        }

        for (uint32_t i = 0 ; i < addrs.getCount() ; i++) {
            CAccountName acct;
            bool found = verbose && options.getNamedAccount(acct, addrs[i]);
            cout << addrs[i] << (found ? acct.Format(" ([{NAME}])") : "") << "\n";
        }
    }
    return 0;
}

