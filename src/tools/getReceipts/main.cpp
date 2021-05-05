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

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CTransaction));
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        opt->errors.push_back("Transaction " + hash + " not found.");
        return true;  // continue even with an invalid item
    }

    if (opt->isRaw || opt->isVeryRaw) {
        string_q result;
        queryRawReceipt(result, trans.getValueByName("hash"));
        if (!isText && !opt->first)
            cout << ",";
        cout << result;
        opt->first = false;
        return true;
    }

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    if (true) {
        if (opt->articulate) {
            opt->abi_spec.loadAbiFromEtherscan(trans.to);
            opt->abi_spec.articulateTransaction(&trans);
        }
        manageFields("CFunction:message", !trans.articulatedTx.message.empty());

        if (isText) {
            cout << trim(trans.receipt.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            if (!opt->first)
                cout << ",";
            cout << "  ";
            indent();
            trans.receipt.toJson(cout);
            unindent();
            opt->first = false;
        }
    }
    return true;
}
