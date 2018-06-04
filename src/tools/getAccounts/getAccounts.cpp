#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

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

