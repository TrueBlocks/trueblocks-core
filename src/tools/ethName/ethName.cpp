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

extern const char* STR_NAME_DATA;
//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init("binary", quickQuitHandler);

    getCurlContext()->nodeRequired = false;  // This will run without a node

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool loaded = options.loadNames();

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        string_q fmt = (options.addrOnly ? "[{ADDR}]" : (options.data ? STR_NAME_DATA : ""));
        if (options.isEdit) {
            editFile(options.namesFile.getFullPath());

        } else if (!loaded) {
            options.usage(options.namesFile.getFullPath() + " is empty. Use ethName -e to add some names. Quitting...");

        } else if (options.list) {
            if (options.count)
                cout << options.namedAccounts.size() << " items\n";
            for (size_t i = 0 ; i < options.namedAccounts.size() ; i++)
                cout << substitute(substitute(options.namedAccounts[i].Format(fmt), "\n", " "), "  ", " ") << "\n";

        } else {
            string_q ret = options.showMatches();
            if (!ret.empty())
                cout << ret;
            else if (verbose)
                cout << "Address '" << options.addr << "' not found\n";
            cout.flush();
        }
    }

    return 0;
}

//-----------------------------------------------------------------------
string_q COptions::showMatches(void) {
    string_q ret;
    size_t hits = 0;
    string_q fmt = (addrOnly ? "[{ADDR}]" : "");
    for (size_t i = 0 ; i < namedAccounts.size() ; i++) {
        if (namedAccounts[i].Match(addr, name, source, matchCase, all)) {
            ret += (substitute(substitute(namedAccounts[i].Format(fmt), "\n", " "), "  ", " ") + "\n");
            hits++;
        }
    }

    if (count)
        ret = uint_2_Str(hits) + " match" + (hits == 1 ? "" : "es") + "\n" + (verbose ? ret : "");

    return ret;
}

//-----------------------------------------------------------------------
const char* STR_NAME_DATA =
"[{symbol}]\t[{name}]\t[{addr}]\t[{source}]\t[{description}]";
