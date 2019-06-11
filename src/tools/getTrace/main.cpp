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

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (options.first)
            cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CTrace));
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
    }
    cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {

    COptions *opt = reinterpret_cast<COptions *>(data);

    bool isText = (opt->exportFmt & (TXT1|CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        if (isText) {
            cout << cRed << "Transaction " << hash << " not found.\n" << cOff;
        } else {
            if (!opt->first)
                cout << ",";
            cout << cRed << "{ \"error\": \"Transaction " << hash << " not found.\" }\n" << cOff;
        }
        opt->first = false;
        return true; // continue even with an invalid item
    }

    if (opt->isRaw || opt->isVeryRaw) {
        string_q result;
        queryRawTrace(result, trans.getValueByName("hash"));
        if (!isText && !opt->first)
            cout << ",";
        replace(result, "[", "");
        replaceReverse(result, "]", "");
        cout << result;
        opt->first = false;
        return true;
    }

    if (opt->countOnly) {
        uint64_t cnt = getTraceCount(trans.hash);
        if (isText) {
            cout << trans.hash << "\t" << cnt << endl;
        } else {
            if (!opt->first)
                cout << ",";
            cout << "{ \"hash\": \"" << trans.hash << "\", \"count\": \"" << cnt << " }";
        }
        opt->first = false;
        return true;
    }

    getTraces(trans.traces, trans.getValueByName("hash"));

    if (opt->articulate) {
        opt->abi_spec.loadAbiByAddress(trans.to);
        opt->abi_spec.articulateTransaction(&trans);
    }
    manageFields("CFunction:message", !trans.articulatedTx.message.empty());

    for (auto trace : trans.traces) {
        if (isText) {
            cout << trim(trace.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            if (!opt->first)
                cout << ",";
            cout << "  ";
            incIndent();
            trace.doExport(cout);
            decIndent();
        }
        opt->first = false;
    }
    return true;
}
