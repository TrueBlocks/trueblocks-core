/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "acctlib.h"

extern void readCustomAddrs(CAddressArray& array);
//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    getCurlContext()->provider = "None";  // --named option runs without a node
    acctlib_init();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // while (!options.commandList.empty())
    {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CAddressArray addrs;
        if (options.fromScraper) {
            CToml toml("./config.toml");
            //            cout << toml << "\n";
            CAccountWatchArray watches;
            loadWatchList(toml, watches, "list");
            for (auto watch : watches)
                addrs.push_back(toLower(watch.address));
            if (options.fromNamed) {
                watches.clear();
                loadWatchList(toml, watches, "named");
                for (auto watch : watches)
                    addrs.push_back(toLower(watch.address));
            }

        } else if (options.fromNamed) {
            for (size_t i = 0 ; i < options.namedAccounts.size() ; i++)
                addrs.push_back(toLower(options.namedAccounts[i].addr));

        } else {
            getAccounts(addrs);
            readCustomAddrs(addrs);
            if (isTestMode()) {
                addrs.clear();
                addrs.push_back("0x0000000000000000000000000000000000000000");
                addrs.push_back("0x0000000000000000000000000000000000000001");
                addrs.push_back("0x0000000000000000000000000000000000000002");
                addrs.push_back("0x0000000000000000000000000000000000000003");
            }
        }

        for (size_t i = 0 ; i < addrs.size() ; i++) {
            CAccountName acct;
            bool found = verbose && options.getNamedAccount(acct, addrs[i]);
            cout << addrs[i] << (found ? acct.Format(" ([{NAME}])") : "") << "\n";
        }
    }

    acctlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
void readCustomAddrs(CAddressArray& array) {
    size_t n = getGlobalConfig()->getConfigInt("extra_accounts", "n", 0);
    for (size_t i = 0 ; i < n ; i++) {
        string_q addr = getGlobalConfig()->getConfigStr("extra_accounts", "ea_" + uint_2_Str(i), "");
        if (!isZeroAddr(addr))
            array.push_back(addr);
    }
}
