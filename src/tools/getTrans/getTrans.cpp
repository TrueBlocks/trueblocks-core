/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "etherlib.h"
#include "options.h"

extern bool visitTransaction(CTransaction& trans, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    options.nCmds = countOf('\n', options.commandList);
    if (options.nCmds > 1 && verbose)
        cout << "[\n";
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
    }
    if (options.nCmds > 1 && verbose)
        cout << "]\n";
    cout.flush();
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    COptions *opt = (COptions*)data;
    opt->nVisited++;

    bool badHash = !isHash(trans.hash);
    bool isBlock = trans.hash.Contains("block");
    trans.hash = trans.hash.Substitute("-block_not_found","").Substitute("-trans_not_found","");
    if (opt->isRaw) {
        if (badHash) {
            cerr << "{\"jsonrpc\":\"2.0\",\"result\":{\"hash\":\"" << trans.hash.Substitute(" ","") << "\",\"result\":\"";
            cerr << (isBlock ? "block " : "");
            cerr << "hash not found\"},\"id\":-1}" << "\n";
            return true;
        }

        // Note: this call is redundant. The transaction is already populated (if it's valid), but we need the raw data)
        SFString results;
        queryRawTransaction(results, trans.getValueByName("hash"));
        cout << results;
        return true;
    }

    if (badHash) {
        cerr << cRed << "Warning:" << cOff << " The " << (isBlock ? "block " : "") << "hash " << cYellow << trans.hash << cOff << " was not found.\n";
        return true;
    }

    if (verbose) {
        trans.doExport(cout);
        if (opt->nVisited <= opt->nCmds && verbose)
            cout << ",";
        cout << "\n";
    } else {
        cout << trans.Format(opt->format);
    }

    if (opt->incTrace) {
        uint64_t nTr = getTraceCount(trans.hash);
        CTraceArray traces;
        getTraces(traces, trans.hash);
        if (traces.getCount()) {
            uint32_t dTs = 0;
            cout << "[";
            for (uint32_t i = 0 ; i < traces.getCount() ; i++) {
                traces[i].doExport(cout);
                dTs = max(dTs, traces[i].traceAddress.getCount());
            }
            cout << "]\n";
            if (opt->nTraces) {
                SFString fmt = ",{ \"nTraces\": [N]-[NN], \"depth\": [D] }";
                cout << fmt.Substitute("[N]", asStringU(nTr))
                            .Substitute("[NN]", asStringU(traces.getCount()))
                            .Substitute("[D]", asStringU(dTs));
            }
        }
    }

    return true;
}
