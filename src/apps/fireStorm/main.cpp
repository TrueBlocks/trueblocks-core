/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
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
                // replace(url, "https://etherscan.io/block/", "http://localhost:3002/explorer/blocks/");
                // replace(url, "https://etherscan.io/tx/", "http://localhost:3002/transactions/");
                // replace(url, "https://etherscan.io/address/", "http://etherscan.io/address/");
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
