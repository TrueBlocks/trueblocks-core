/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        string_q orig = command;
        if (!options.parseArguments(command))
            return 0;

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], "CPinnedChunk");

        for (auto term : options.terms) {
            string_q lower = toLower(term);
            string url = "https://etherscan.io/";
            if (isAddress(lower)) {
                url += "address/" + lower;

            } else if (isHash(lower)) {
                CTransaction trans;
                getTransaction(trans, lower);
                if (trans.hash == lower) {
                    url += "tx/" + lower;
                } else {
                    url += "block/" + lower;
                }

            } else if (isFourByte(lower)) {
                if (options.local)
                    LOG_WARN("Local fourbyte is not implemented");
                url = "https://www.4byte.directory/signatures/?bytes4_signature=" + lower;

            } else {
                if (endsWith(lower, ".eth")) {
                    url = "https://etherscan.io/enslookup-search?search=" + lower;

                } else if (contains(lower, ".")) {
                    CUintArray parts;
                    explode(parts, lower, '.');
                    CTransaction trans;
                    getTransaction(trans, parts[0], parts[1]);
                    url += "tx/" + trans.hash;

                } else {
                    url += "block/" + lower;
                }
            }

            if (options.local) {
                string_q base =
                    getGlobalConfig("fireStorm")->getConfigStr("settings", "baseURL", "http://localhost:1234/");
                if (!endsWith(base, "/"))
                    base += "/";
                replace(url, "https://etherscan.io/block/", base + "explorer/blocks/");
                replace(url, "https://etherscan.io/tx/", base + "explorer/transactions/");
                replace(url, "https://etherscan.io/address/", base + "dashboard/accounts/");
                replace(url, "https://www.4byte.directory/signatures/?bytes4_signature=", base + "/");
            }

            cerr << "Opening " << url << endl;
            if (!isTestMode())
                doCommand("open " + url);
        }
        once = false;
    }

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    etherlib_cleanup();

    return 0;
}
